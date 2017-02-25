#!/usr/bin/env python3

import filecmp, os, re, subprocess

TEST_DIR = os.path.dirname(__file__)
BIN_DIR = os.path.abspath(os.path.join(TEST_DIR, '..', 'bin'))
ASM_EXE = os.path.join(BIN_DIR, 'asm')

def asmFile(iSet, inFilename, outFile, errFile):
    subprocess.run([ASM_EXE, '--no-color', inFilename, '-i', iSet], stdout=outFile, stderr=errFile)

def asmPipe(iSet, inStr, args=None):
    cmd = [ASM_EXE, '--no-color', '-i', iSet]
    if args is not None:
        cmd += args
    p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return p.communicate(inStr)

class Test:
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
            asmFile(self._iSet, inFilename, outFile, errFile)

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
        self._iSet = iSet
        self._inStr = inStr
        self._outStr = outStr
        self._errStr = errStr
        self._args = args

    def run(self):
        passed = True

        out, err = asmPipe(self._iSet, self._inStr, self._args)

        if out != self._outStr:
            passed = False
            self._errorMsg += 'Unexpected stdout:\n' + out.decode('utf-8') + '\n'

        if err != self._errStr:
            passed = False
            self._errorMsg += 'Unexpected stderr:\n' + err.decode('utf-8') + '\n'

        return passed

class SymbolTest(Test):
    def __init__(self, name, iSet, inStr, value):
        super(SymbolTest, self).__init__(name)
        self._iSet = iSet
        self._inStr = inStr
        self._value = value

    def _checkStdout(self, out):
        outStr = str(out)
        pattern = r'.*\s+{}'.format(self._value)
        match = re.search(pattern, outStr)

        return match is not None

    def run(self):
        passed = True

        out, err = asmPipe(self._iSet, self._inStr, ['-s'])

        if not self._checkStdout(out):
            passed = False
            self._errorMsg += 'Unexpected stdout:\n' + out.decode('utf-8') + '\n'

        if err != b'':
            passed = False
            self._errorMsg += 'Unexpected stderr:\n' + err.decode('utf-8') + '\n'

        return passed

class Tester:
    def __init__(self):
        self.tests = []

    def add(self, test):
        self.tests.append(test)

    def run(self):
        numPassed = 0
        errorMsg = ''
        for test in self.tests:
            passed = test.run()
            if passed:
                numPassed += 1
                print('.', end='', flush=True)
            else:
                errorMsg += '{} failed:\n{}\n'.format(test.name, test.getError())
                print('E', end='', flush=True)

        print()

        if errorMsg != '':
            print(errorMsg, end='')

        numTests = len(self.tests)
        print('{} of {} test{} passed.'.format(numPassed, numTests, '' if numTests == 1 else 's'))

def main():
    tester = Tester()

    ### file tests ###

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

    ### string tests ###

    # numbers
    tester.add(StringTest('Immediate Bin Lower', 'W16', b'jmp 0b1010011', outStr=b'c053\n'))
    tester.add(StringTest('Immediate Bin Upper', 'W16', b'jmp 0B101101', outStr=b'c02d\n'))
    tester.add(StringTest('Immediate Oct Lower', 'W16', b'jmp 0o107', outStr=b'c047\n'))
    tester.add(StringTest('Immediate Oct Upper', 'W16', b'jmp 0O6443', outStr=b'cd23\n'))
    tester.add(StringTest('Immediate Dec 1',     'W16', b'jmp 19', outStr=b'c013\n'))
    tester.add(StringTest('Immediate Dec 2',     'W16', b'jmp 0123', outStr=b'c07b\n'))
    tester.add(StringTest('Immediate Hex Lower', 'W16', b'jmp 0xafe', outStr=b'cafe\n'))
    tester.add(StringTest('Immediate Hex Upper', 'W16', b'jmp 0X6B', outStr=b'c06b\n'))

    # signs
    tester.add(StringTest('Positive Sign 1', 'W16', b'addi r0, +7', outStr=b'b007\n'))
    tester.add(StringTest('Positive Sign 2', 'W16', b'addi r0, +++39', outStr=b'b027\n'))
    tester.add(StringTest('Negative Sign 1', 'W16', b'addi r0, -7', outStr=b'b0f9\n'))
    tester.add(StringTest('Negative Sign 2', 'W16', b'addi r0, -0b10001', outStr=b'b0ef\n'))
    tester.add(StringTest('Negative Sign 3', 'W16', b'addi r0, --10', outStr=b'b00a\n'))
    tester.add(StringTest('Mixed Signs 1', 'W16', b'addi r0, +-+10', outStr=b'b0f6\n'))
    tester.add(StringTest('Mixed Signs 2', 'W16', b'addi r0, --+100', outStr=b'b064\n'))

    # expressions
    tester.add(SymbolTest('Single Value', 'W16', b'x = 2', 2))
    tester.add(SymbolTest('Negative Value', 'W16', b'x = -102943', -102943))
    tester.add(StringTest('Complement 1', 'W16', b'addi r0, ~0x6a', outStr=b'b095\n'))
    tester.add(SymbolTest('Addition 1', 'W16', b'x = 193 + 32 + 7', 232))
    tester.add(SymbolTest('Addition 2', 'W16', b'x = -1000 + 3002 + 0 + 5', 2007))
    tester.add(SymbolTest('Addition 3', 'W16', b'x = --3 + -0xA + 0b101', -2))
    tester.add(SymbolTest('Subtraction 1', 'W16', b'x = 100 - 25 - 7', 68))
    tester.add(SymbolTest('Subtraction 2', 'W16', b'x = 100 - -0O31 - 0', 125))
    tester.add(SymbolTest('Subtraction 3', 'W16', b'x = 100 - 1 - 1 - --1', 97))
    tester.add(SymbolTest('Multiplication 1', 'W16', b'x = 5 * 100 * 2', 1000))
    tester.add(SymbolTest('Division 1', 'W16', b'x = 99 / 3', 33))
    tester.add(SymbolTest('Modulo 1', 'W16', b'x = 0xa0 / 4', 0x28))
    tester.add(SymbolTest('AND 1', 'W16', b'x = 0x3e & 0B110111 & 47', 0x26))
    tester.add(SymbolTest('OR 1', 'W16', b'x = 0o74 | 3', 0o77))
    tester.add(SymbolTest('XOR 1', 'W16', b'x = 0x7b ^ 0Xf ^ 16', 0x64))
    tester.add(SymbolTest('Operator Precedence 1', 'W16', b'x = 2 + 2 * 3', 8))
    tester.add(SymbolTest('Operator Precedence 1', 'W16', b'x = 2 + -12 / 4 - 5', -6))

    tester.run()

if __name__ == '__main__':
    main()
