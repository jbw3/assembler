#!/usr/bin/env python3

import filecmp, os, subprocess

def asm(iSet, inFilename, outFile, errFile):
    exe = '../bin/asm'
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
        inFilename = testName + '.s'
        outFilename = 'temp.out'
        errFilename = 'temp.err'
        expectedOut = testName + '.out'
        expectedErr = testName + '.err'

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
    tester.add('W16', 'W16_2')

    tester.run()

if __name__ == '__main__':
    main()
