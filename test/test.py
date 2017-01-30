#!/usr/bin/env python3

import filecmp, os, subprocess

def asm(iSet, inFile, outFile):
    exe = '../bin/asm'
    subprocess.call([exe, inFile, '-i', iSet, '-o', outFile])

class Tester(object):
    def __init__(self):
        self.numTests = 0
        self.numPassed = 0
        self.testArgs = []

    def add(self, iSet, inFile, expectedFile):
        self.testArgs.append((iSet, inFile, expectedFile))
        self.numTests += 1

    def run(self):
        for args in self.testArgs:
            passed = self._runTest(args[0], args[1], args[2])
            if passed:
                self.numPassed += 1

        print('{} of {} test{} passed.'.format(self.numPassed, self.numTests, '' if self.numTests == 1 else 's'))

    def _runTest(self, iSet, inFile, expectedFile):
        outFile = 'temp.out'
        asm(iSet, inFile, outFile)

        equal = filecmp.cmp(outFile, expectedFile, False)

        try:
            os.remove(outFile)
        except OSError:
            pass

        return equal

def main():
    tester = Tester()

    tester.add('W8', 'test1.s', 'test1.out')
    tester.add('W8', 'test2.s', 'test2.out')

    tester.run()

if __name__ == '__main__':
    main()
