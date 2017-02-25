#!/usr/bin/env python3

import filecmp, os, subprocess

TEST_DIR = os.path.dirname(__file__)
BIN_DIR = os.path.abspath(os.path.join(TEST_DIR, '..', 'bin'))

def asm(iSet, inFilename, outFile, errFile):
    exe = os.path.join(BIN_DIR, 'asm')
    subprocess.run([exe, '--no-color', inFilename, '-i', iSet], stdout=outFile, stderr=errFile)

class Test(object):
    def __init__(self, name):
        self._name = name

    def run(self):
        return False

    def getName(self):
        return self._name

    name = property(getName)

class FileTest(Test):
    def __init__(self, name, iSet):
        super(FileTest, self).__init__(name)
        self._iSet = iSet

    def run(self):
        inFilename = os.path.join(TEST_DIR, self.name + '.s')
        outFilename = os.path.join(TEST_DIR, 'temp.out')
        errFilename = os.path.join(TEST_DIR, 'temp.err')
        expectedOut = os.path.join(TEST_DIR, self.name + '.out')
        expectedErr = os.path.join(TEST_DIR, self.name + '.err')

        with open(outFilename, 'w') as outFile, open(errFilename, 'w') as errFile:
            asm(self._iSet, inFilename, outFile, errFile)

        equal = filecmp.cmp(outFilename, expectedOut, False)
        equal &= filecmp.cmp(errFilename, expectedErr, False)

        self._rmFile(outFilename)
        self._rmFile(errFilename)

        return equal

    def _rmFile(self, filename):
        try:
            os.remove(filename)
        except OSError:
            pass

class Tester(object):
    def __init__(self):
        self.tests = []

    def add(self, test):
        self.tests.append(test)

    def run(self):
        numPassed = 0
        for test in self.tests:
            passed = test.run()
            if passed:
                numPassed += 1
            else:
                print(test.name, 'failed.')

        numTests = len(self.tests)
        print('{} of {} test{} passed.'.format(numPassed, numTests, '' if numTests == 1 else 's'))

def main():
    tester = Tester()

    tester.add(FileTest('W8_1', 'W8'))
    tester.add(FileTest('W8_2', 'W8'))
    tester.add(FileTest('W16_1', 'W16'))
    tester.add(FileTest('numbers', 'W16'))
    tester.add(FileTest('invalid_token', 'W16'))
    tester.add(FileTest('invalid_token_eol', 'W16'))
    tester.add(FileTest('no_arg_sep', 'W16'))
    tester.add(FileTest('no_arg', 'W16'))
    tester.add(FileTest('no_arg_eol', 'W16'))
    tester.add(FileTest('invalid_instruction', 'W16'))
    tester.add(FileTest('warn_large_imm', 'W16'))
    tester.add(FileTest('labels', 'W16'))
    tester.add(FileTest('warn_large_labels', 'W8'))
    tester.add(FileTest('redefined_label', 'W16'))
    tester.add(FileTest('constants', 'W16'))

    tester.run()

if __name__ == '__main__':
    main()
