#!/usr/bin/env python3

import filecmp, os, subprocess

TEST_DIR = os.path.dirname(__file__)
BIN_DIR = os.path.abspath(os.path.join(TEST_DIR, '..', 'bin'))

def asm(iSet, inFilename, outFile, errFile):
    exe = os.path.join(BIN_DIR, 'asm')
    subprocess.run([exe, '--no-color-output', inFilename, '-i', iSet], stdout=outFile, stderr=errFile)

class Tester(object):
    def __init__(self):
        self.numTests = 0
        self.numPassed = 0
        self.testArgs = []

    def add(self, iSet, testName):
        self.testArgs.append((iSet, testName))
        self.numTests += 1

    def run(self):
        for args in self.testArgs:
            iSet, name = args
            passed = self._runTest(iSet, name)
            if passed:
                self.numPassed += 1
            else:
                print(name, 'failed.')

        print('{} of {} test{} passed.'.format(self.numPassed, self.numTests, '' if self.numTests == 1 else 's'))

    def _runTest(self, iSet, testName):
        inFilename = os.path.join(TEST_DIR, testName + '.s')
        outFilename = os.path.join(TEST_DIR, 'temp.out')
        errFilename = os.path.join(TEST_DIR, 'temp.err')
        expectedOut = os.path.join(TEST_DIR, testName + '.out')
        expectedErr = os.path.join(TEST_DIR, testName + '.err')

        with open(outFilename, 'w') as outFile, open(errFilename, 'w') as errFile:
            asm(iSet, inFilename, outFile, errFile)

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

def main():
    tester = Tester()

    tester.add('W8', 'W8_1')
    tester.add('W8', 'W8_2')
    tester.add('W16', 'W16_1')
    tester.add('W16', 'numbers')
    tester.add('W16', 'invalid_token')
    tester.add('W16', 'invalid_token_eol')
    tester.add('W16', 'no_arg_sep')
    tester.add('W16', 'no_arg')
    tester.add('W16', 'no_arg_eol')
    tester.add('W16', 'warn_large_imm')
    tester.add('W16', 'labels')
    tester.add('W8', 'warn_large_labels')

    tester.run()

if __name__ == '__main__':
    main()
