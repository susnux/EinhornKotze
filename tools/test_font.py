#!/usr/bin/env python3
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--font", help="The binary font file", required=True)
parser.add_argument("--text", help="Fontsize where to render (pt)", default="Hello World")
args = parser.parse_args()

with open(args.font, "rb") as file:
    width, height = file.read(2)
    print(f"Loaded font (width: {width}, height: {height})")

    lines = ['']*height
    for c in args.text:
        a = ord(c)
        file.seek(2 + width * a, 0)
        char = file.read(width)
        for w in range(width):
            for h in range(height): 
                lines[h] += '#' if char[w] & (1<<h) else ' '
    for l in lines:
        print(l)