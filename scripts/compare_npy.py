#!/usr/bin/bash python3
"""Compares 2 npy files to make sure they are (almost) equal."""

import sys
import argparse

import numpy


def main():
    # Setup argument parser
    parser = argparse.ArgumentParser(description='Compares 2 npy files to make sure that they are (almost) equal')
    parser.add_argument('a.npy', type=argparse.FileType('rb'), help='first file to compare')
    parser.add_argument('b.npy', type=argparse.FileType('rb'), help='second file to compare')
    parser.add_argument('--exact', action='store_true', help='must be exactly equal instead of almost equal')

    # Parse the command-line arguments
    args = parser.parse_args()

    # Load the matrices
    a = numpy.load(getattr(args, 'a.npy'))
    b = numpy.load(getattr(args, 'b.npy'))

    # Check matrices
    if a.shape != b.shape:
        print("unequal shapes")
        sys.exit(2)
    if not args.exact and numpy.allclose(a, b):
        if numpy.all(numpy.equal(a, b)):
            print("equal")
        else:
            print("allclose")
        sys.exit(0)
    elif args.exact and numpy.all(numpy.equal(a, b)):
        print("equal")
        sys.exit(0)
    else:
        print("not equal/allclose")
        diff = abs(a - b).max()
        print("max absolute difference is", diff)
        print("max relative difference is", (diff / abs(b)).max())
        sys.exit(1)


if __name__ == "__main__":
    main()
