#!/usr/bin/env python3

import filecmp, os, re, subprocess, sys

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
            try:
                passed = test.run()
                if not passed:
                    errorMsg += '{} failed:\n{}\n'.format(test.name, test.getError())
            except Exception as e:
                passed = False
                errorMsg += '{} failed:\n{}\n'.format(test.name, e)

            if passed:
                numPassed += 1
                print('.', end='', flush=True)
            else:
                print('F', end='', flush=True)

        # print newline
        print()

        if errorMsg != '':
            print(errorMsg, end='')

        numTests = len(self.tests)
        print('{} of {} test{} passed.'.format(numPassed, numTests, '' if numTests == 1 else 's'))

        return numPassed == numTests

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
    tester.add(FileTest('expressions', 'W16'))

    # MIPS
    tester.add(FileTest('mips32', 'MIPS32'))

    ### string tests ###

    # numbers
    tester.add(StringTest('Immediate Bin Lower', 'W16', b'jmp 0b1010010', outStr=b'c029\n'))
    tester.add(StringTest('Immediate Bin Upper', 'W16', b'jmp 0B101100', outStr=b'c016\n'))
    tester.add(StringTest('Immediate Oct Lower', 'W16', b'jmp 0o106', outStr=b'c023\n'))
    tester.add(StringTest('Immediate Oct Upper', 'W16', b'jmp 0O6442', outStr=b'c691\n'))
    tester.add(StringTest('Immediate Dec 1',     'W16', b'jmp 18', outStr=b'c009\n'))
    tester.add(StringTest('Immediate Dec 2',     'W16', b'jmp 0234', outStr=b'c075\n'))
    tester.add(StringTest('Immediate Hex Lower', 'W16', b'jmp 0x15fc', outStr=b'cafe\n'))
    tester.add(StringTest('Immediate Hex Upper', 'W16', b'jmp 0X6c', outStr=b'c036\n'))

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
    tester.add(SymbolTest('Modulo 1', 'W16', b'x = 113 % 25', 13))
    tester.add(SymbolTest('AND 1', 'W16', b'x = 0x3e & 0B110111 & 47', 0x26))
    tester.add(SymbolTest('OR 1', 'W16', b'x = 0o74 | 3', 0o77))
    tester.add(SymbolTest('XOR 1', 'W16', b'x = 0x7b ^ 0Xf ^ 16', 0x64))
    tester.add(SymbolTest('SHIFT LEFT 1', 'W16', b'x = 0xa << 3', 0x50))
    tester.add(SymbolTest('SHIFT RIGHT 1', 'W16', b'x = 12 >> 2', 3))
    tester.add(SymbolTest('Operator Precedence 1', 'W16', b'x = 2 + 2 * 3', 8))
    tester.add(SymbolTest('Operator Precedence 2', 'W16', b'x = 2 + -12 / 4 - 5', -6))
    tester.add(SymbolTest('Operator Precedence 3', 'W16', b'x = 9 & -2 + 5', 1))
    tester.add(SymbolTest('Operator Precedence 4', 'W16', b'x = 0x18 >> -6 - -8 ^ 10 - 5 + 7', 10))
    tester.add(SymbolTest('Parentheses 1', 'W16', b'x = (2 + 2) * 3', 12))
    tester.add(SymbolTest('Parentheses 2', 'W16', b'x = (10)', 10))
    tester.add(SymbolTest('Parentheses 3', 'W16', b'x = (((10)))', 10))
    tester.add(SymbolTest('Parentheses 4', 'W16', b'x = 2 * -(4 + 8)', -24))
    tester.add(SymbolTest('Parentheses 5', 'W16', b'x = -( ( (1 << 3) + (0xac & 0xf) ) / 10 )', -2))

    # invalid expressions
    tester.add(StringTest('Undefined Constant', 'W16', b'x = 2 + y', errStr=b'ERROR: line: 1, col: 9\n"y" has not been defined.\n'))
    tester.add(StringTest('No Closing Parenthesis', 'W16', b'x = ( (2 + 2) * 3', errStr=b'ERROR: line: 1, col: 5\nCould not find closing parenthesis.\n'))
    tester.add(StringTest('No Opening Parenthesis', 'W16', b'x = (2 + 2) * 3)', errStr=b'ERROR: line: 1, col: 16\nExtra closing parenthesis.\n'))
    tester.add(StringTest('Missing Operator', 'W16', b'x = 12 + 32 41', errStr=b'ERROR: line: 1, col: 13\nExpected operator before "41".\n'))
    tester.add(StringTest('Empty Expression', 'W16', b'x = ', errStr=b'ERROR: line: 1, col: 3\nExpected an expression after "=".\n'))
    tester.add(StringTest('Empty Parentheses Expression', 'W16', b'x = 2 + ()', errStr=b'ERROR: line: 1, col: 9\nNo expression after "(".\n'))
    tester.add(StringTest('Instruction Constant Upper', 'W16', b'MOV = 2', errStr=b"ERROR: line: 1, col: 1\nA constant's name cannot be an instruction.\n"))
    tester.add(StringTest('Instruction Constant Lower', 'W16', b'mov = 2', errStr=b"ERROR: line: 1, col: 1\nA constant's name cannot be an instruction.\n"))
    tester.add(StringTest('Register Constant Upper', 'W16', b'R0 = 2', errStr=b"ERROR: line: 1, col: 1\nA constant's name cannot be a register.\n"))
    tester.add(StringTest('Register Constant Lower', 'W16', b'r0 = 2', errStr=b"ERROR: line: 1, col: 1\nA constant's name cannot be a register.\n"))

    # truncation warnings
    tester.add(StringTest('Positive Number Truncation 1', 'W16', b'addi r0, 200', outStr=b'b0c8\n', errStr=b'WARNING: line: 1, col: 10\nImmediate value was truncated.\n'))
    tester.add(StringTest('Positive Number Truncation 2', 'W16', b'addi r0, 5000', outStr=b'b088\n', errStr=b'WARNING: line: 1, col: 10\nImmediate value was truncated.\n'))
    tester.add(StringTest('Negative Number Truncation 1', 'W16', b'addi r0, -200', outStr=b'b038\n', errStr=b'WARNING: line: 1, col: 10\nImmediate value was truncated.\n'))
    tester.add(StringTest('Negative Number Truncation 2', 'W16', b'addi r0, -3000', outStr=b'b048\n', errStr=b'WARNING: line: 1, col: 10\nImmediate value was truncated.\n'))
    tester.add(StringTest('LSB Truncation', 'W16', b'jmp 3', outStr=b'c001\n', errStr=b'WARNING: line: 1, col: 5\nImmediate value was truncated.\n'))

    # format
    tester.add(StringTest('Format Text 1', 'W8', b'nop\nmov r0\nadd r4', outStr=b'00\n08\n44\n', args=['-f', 'text']))
    tester.add(StringTest('Format Text 2', 'W16', b'nop\nmov r0, r1\nnot r4, r7', outStr=b'0000\n0101\n0247\n', args=['-f', 'text']))
    tester.add(StringTest('Format Binary 1', 'W8', b'nop\nmov r0\nadd r4', outStr=b'\x00\x08\x44', args=['-f', 'bin']))
    tester.add(StringTest('Format Binary 2', 'W16', b'nop\nmov r12, r1\nnot r4, r7', outStr=b'\x00\x00\x01\xc1\x02\x47', args=['-f', 'bin']))

    # start_address
    tester.add(StringTest('Start Address', 'MIPS32', b'start_address = 4\naddi t0, zero, here', outStr=b'04000820\n'))
    tester.add(StringTest('Multiple Start Addresses', 'MIPS32', b'start_address = 4\nstart_address = 4\naddi t0, zero, here',
                          errStr=b'ERROR: line: 2, col: 1\nThe start address has already been defined.\n'))
    tester.add(StringTest('Constant Before Start Address', 'MIPS32', b'x = 4\nstart_address = 4\naddi t0, zero, here',
                          errStr=b'ERROR: line: 2, col: 1\nThe start address must be defined before other constants.\n'))
    tester.add(StringTest('Word-Aligned Start Address', 'MIPS32', b'start_address = 3\naddi t0, zero, here',
                          errStr=b'ERROR: line: 1, col: 1\nThe start address is not word aligned.\n'))
    tester.add(StringTest('Start Address No here', 'MIPS32', b'start_address = here + 4',
                          errStr=b'ERROR: line: 1, col: 17\n"here" cannot be used in this expression.\n'))
    tester.add(StringTest('Start Address No start_address', 'MIPS32', b'start_address = start_address + 4',
                          errStr=b'ERROR: line: 1, col: 17\n"start_address" cannot be used in this expression.\n'))

    allPassed = tester.run()

    rc = 0 if allPassed else 1
    sys.exit(rc)

if __name__ == '__main__':
    main()
