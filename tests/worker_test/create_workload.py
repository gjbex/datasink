#!/usr/bin/env python

import itertools

if __name__ == '__main__':
    from argparse import ArgumentParser
    arg_parser = ArgumentParser(description='create workload for '
                                            'datasink tests')
    arg_parser.add_argument('--n', type=int, default=10,
                            help='number of character in a single write')
    arg_parser.add_argument('--repeat', type=int, default=1,
                            help='number of writes per work item')
    arg_parser.add_argument('--sleep', type=float, default=1.0,
                            help='time to sleep (s) between writes')
    arg_parser.add_argument('--var', type=float, default=0.5,
                            help='fraction of sleep time for variation')
    arg_parser.add_argument('--nr_items', type=int, default=20,
                            help='numer of work items to generate')
    options = arg_parser.parse_args()
    columns = ['id', 'char', 'n', 'repeat', 'sleep', 'var']
    alphabet = 'abcdefghijklmnopqrst'
    data_fmt = '{0:d},{1:s},{2:d},{3:d},{4:.6f},{5:.6f}'
    print ','.join(columns)
    for id, char in itertools.izip(xrange(options.nr_items),
                                   itertools.cycle(alphabet)):
        print data_fmt.format(id, char, options.n, options.repeat,
                              options.sleep, options.var)
