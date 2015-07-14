#!/usr/bin/env python

import random
import sys
import time


def write_burst(character, nr_chars):
    sys.stdout.write(character*nr_chars)
    sys.stdout.flush()


def bursts(options):
    for _ in xrange(options.repeat):
        min_time = max(0.0, options.sleep - options.variation*options.sleep)
        max_time = options.sleep + options.variation*options.sleep
        sleep_time = random.uniform(min_time, max_time)
        time.sleep(sleep_time)
        write_burst(options.char, options.n)


if __name__ == '__main__':
    from argparse import ArgumentParser
    arg_parser = ArgumentParser(description='write-intensive workload '
                                            'for datasink testing')
    arg_parser.add_argument('--char', type=str, default='a',
                            help='character to write')
    arg_parser.add_argument('--n', type=int, default=1024,
                            help='number of characters to write')
    arg_parser.add_argument('--repeat', type=int, default=1,
                            help='number of repetitions')
    arg_parser.add_argument('--sleep', type=float, default=1.0,
                            help='time (s) to sleep between repetitions')
    arg_parser.add_argument('--variation', type=float, default=0.1,
                            help='width (fraction) around sleep time')
    options = arg_parser.parse_args()
    bursts(options)
