#!/usr/bin/env python3
"""
Simple addr2line helper for riscv32-esp builds.
Usage:
  python tools/addr2line.py .pio/build/xiao_esp32c3/firmware.elf 0x4001234 0x4005678

The script will try to find `riscv32-esp-elf-addr2line` in PATH. If not found
it will print the exact command you can run once you have the toolchain in PATH.
"""
import shutil
import subprocess
import sys

if len(sys.argv) < 3:
    print("Usage: addr2line.py <elf-file> <addr1> [addr2 ...]")
    sys.exit(2)

elf = sys.argv[1]
addrs = sys.argv[2:]

tool ='C:\\Users\\bossg\\.platformio\\packages\\toolchain-riscv32-esp\\bin\\riscv32-esp-elf-addr2line.exe'

cmd = [tool, '-pfia', '-e', elf] + addrs
try:
    out = subprocess.check_output(cmd, stderr=subprocess.STDOUT)
    print(out.decode('utf-8'))
except subprocess.CalledProcessError as e:
    print('addr2line failed:')
    print(e.output.decode('utf-8'))
    sys.exit(1)
