#!/usr/bin/env python3
"""
generate a binary representation of a test signal.
"""

import argparse
import math
import numpy as np

SIGNAL_AMPLITUDE: int = 255


def f(x: float) -> float:
    return 127


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process some integers.")
    parser.add_argument("-n", metavar="n", dest="n", type=int, help="signal length (bytes)")
    parser.add_argument("-o", metavar="o", dest="osignal", type=str, help="output signal")
    args = parser.parse_args()

    signal = args.osignal
    SIGNAL_LENGTH = args.n
    fd = open(signal, "wb")

    xs = np.linspace(0, SIGNAL_LENGTH - 1, SIGNAL_LENGTH)
    ys = [f(x) for x in xs]

    fd.write(bytearray([int(y) for y in ys]))
