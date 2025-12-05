#!/usr/bin/env python3
import os
import sys

if len(sys.argv) != 2:
    print("Usage: calc_sectors.py <kernel_bin_file>")
    sys.exit(1)

kernel_bin = sys.argv[1]

if not os.path.exists(kernel_bin):
    print("1")
    sys.exit(0)

kernel_size = os.path.getsize(kernel_bin)
kernel_sectors = (kernel_size + 511) // 512
if kernel_sectors == 0:
    kernel_sectors = 1

print(kernel_sectors)

