#!/usr/bin/env python3

import argparse
from pickletools import uint8
from turtle import width
import numpy as np
from PIL.BdfFontFile import BdfFontFile


parser = argparse.ArgumentParser()
parser.add_argument("--font", help="The bdf font", required=True)
parser.add_argument("--output", help="Fontsize where to render (pt)", default="out.bin")
args = parser.parse_args()

with open(args.font, "rb") as fp:
    bdf_file = BdfFontFile(fp)
    bdf_file.compile()
    _, _, bounding_box, _ = bdf_file[ord('Z')]
    _, _, width, height = bounding_box

    print(
        f"Opened font: {args.font}\n"
        f"Output file: {args.output}\n"
        f"Output size: {width * 256 + 2} bytes\n"
        f"Font size  : {width} * {height} (width * height)\n"
    )

    with open(args.output, "wb") as out:
        out.write(width.to_bytes(1, "big"))
        out.write(height.to_bytes(1, "big"))
        for i in range(256):
            bdf_char = bdf_file[i]
            if bdf_char is None:
                image = np.zeros((8,5), np.uint8)
            else:
                _, _, _, image = bdf_char
                image = (np.array(image.convert('L')) / 255).astype(np.uint8)
            for x in range(width):
                c = 0
                for y in range(height):
                    c = c | ((int)(image[y,x])<<y)
                out.write(c.to_bytes(1, "big"))