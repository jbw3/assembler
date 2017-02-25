#!/usr/bin/env python3

import filecmp, os, subprocess

TEST_DIR = os.path.dirname(__file__)
BIN_DIR = os.path.abspath(os.path.join(TEST_DIR, '..', 'bin'))
ASM_EXE = os.path.join(BIN_DIR, 'asm')

def asm(iSet, inFilename, outFile, errFile):
    subprocess.run([ASM_EXE, '--no-color', inFilename, '-i', iSet], stdout=outFile, stderr=errFile)

class Test(object):
    def __init__(self, name):
        self._name = name
        self._errorMsg = ''

    def run(self):
        return False

    def getName(self):
        return self._name

    name = property(getName)

    def getError(self):
        return self._errorMsg

class FileTest(Test):
    def __init__(self, name, iSet):
        super(FileTest, self).__init__(name)
        self._iSet = iSet

    def run(self):
        passed = True

        inFilename = os.path.join(TEST_DIR, self.name + '.s')
        outFilename = os.path.join(TEST_DIR, 'temp.out')
        errFilename = os.path.join(TEST_DIR, 'temp.err')
        expectedOut = os.path.join(TEST_DIR, self.name + '.out')
        expectedErr = os.path.join(TEST_DIR, self.name + '.err')

        with open(outFilename, 'w') as outFile, open(errFilename, 'w') as errFile:
            asm(self._iSet, inFilename, outFile, errFile)

        equal = filecmp.cmp(outFilename, expectedOut, False)
        if not equal:
            passed = False
            with open(outFilename, 'r') as outFile:
                self._errorMsg += 'Unexpected stdout:\n'
                self._errorMsg += outFile.read()

        equal = filecmp.cmp(errFilename, expectedErr, False)
        if not equal:
            passed = False
            with open(errFilename, 'r') as errFile:
                self._errorMsg += 'Unexpected stderr:\n'
                self._errorMsg += errFile.read()

        self._rmFile(outFilename)
        self._rmFile(errFilename)

        return passed

    def _rmFile(self, filename):
        try:
            os.remove(filename)
        except OSError:
            pass

class StringTest(Test):
    def __init__(self, name, iSet, inStr, outStr=b'', errStr=b'', args=None):
        super(StringTest, self).__init__(name)
        self._args = [ASM_EXE, '--no-color', '-i', iSet]
        if args is not None:
            self._args += args
        self._inStr = inStr
        self._outStr = outStr
        self._errStr = errStr

    def run(self):
        passed = True

        p = subprocess.Popen(self._args, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate(self._inStr)

        if out != self._outStr:
            passed = False
            self._errorMsg += 'Unexpected stdout:\n' + str(out) + '\n'

        if err != self._errStr:
            passed = False
            self._errorMsg += 'Unexpected stderr:\n' + str(err) + '\n'

        return passed

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
                print(test.name, 'failed:')
                print(test.getError())

        numTests = len(self.tests)
        print('{} of {} test{} passed.'.format(numPassed, numTests, '' if numTests == 1 else 's'))

def main():
    tester = Tester()

    # file tests
    tester.add(FileTest('W8_1', 'W8'))
    tester.add(FileTest('W8_2', 'W8'))
    tester.add(FileTest('W16_1', 'W16'))
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

    # string tests
    tester.add(StringTest('Immediate Bin Lower', 'W16', b'jmp 0b1010011', outStr=b'c053\n'))
    tester.add(StringTest('Immediate Bin Upper', 'W16', b'jmp 0B101101', outStr=b'c02d\n'))
    tester.add(StringTest('Immediate Oct Lower', 'W16', b'jmp 0o107', outStr=b'c047\n'))
    tester.add(StringTest('Immediate Oct Upper', 'W16', b'jmp 0O6443', outStr=b'cd23\n'))
    tester.add(StringTest('Immediate Dec 1',     'W16', b'jmp 19', outStr=b'c013\n'))
    tester.add(StringTest('Immediate Dec 2',     'W16', b'jmp 0123', outStr=b'c07b\n'))
    tester.add(StringTest('Immediate Hex Lower', 'W16', b'jmp 0xafe', outStr=b'cafe\n'))
    tester.add(StringTest('Immediate Hex Upper', 'W16', b'jmp 0X6B', outStr=b'c06b\n'))

    tester.run()

if __name__ == '__main__':
    main()
