#!/usr/bin/env python3

import fileinput
import sys

def parse_one_line(line):
    line = line.rstrip()
    pairs = line.split()

    for one_pair in pairs:
        key_value = one_pair.split(':')
        if (key_value[0] == "len"):
            payload = sys.stdin.read(int(key_value[1]))

            if (len(payload) != int(key_value[1])):
                raise Exception('payload size expected and got not equals')

            sys.stderr.write(payload)

        sys.stderr.write(key_value[0])
        sys.stderr.write(':')
        sys.stderr.write(key_value[1])
        sys.stderr.flush()
    
    sys.stderr.write('\n\n')
    sys.stderr.flush()

def main():
    while True:
        # start receiving events
        sys.stdout.write('READY\n')
        sys.stdout.flush()

        line = sys.stdin.readline()
        if not line:
            break
        
        line = line.rstrip()
        parse_one_line(line)

        sys.stdout.write('RESULT 2\nOK')
        sys.stdout.flush()
        sys.stdout.write('READY\n')
        sys.stdout.flush()

if __name__ == '__main__':
    main()