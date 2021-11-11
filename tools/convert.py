import numpy as np
from PIL import ImageFont, ImageDraw, Image
import cv2
import math
import argparse

## Make canvas and set the color
img = np.zeros((16,16,3),np.uint8)
b,g,r,a = 255,255,255,0

parser = argparse.ArgumentParser()
parser.add_argument("--height", help="The height of the font", required=True)
parser.add_argument("--width", help="The width of the font", required=True)
parser.add_argument("--font", help="The truetype or otf font", required=True)
parser.add_argument("--font-size", help="Fontsize where to render (pt)", required=True)
parser.add_argument("--output", help="Fontsize where to render (pt)", default="out.bin")
args = parser.parse_args()


font = ImageFont.truetype(args.font, args.font_size)

size = (args.width, args.height)
get_bin = lambda x, z: format(x, 'b').zfill(z)

with open(args.output, "wb") as file:
    file.write(size[0].to_bytes(1, "big"))
    file.write(size[1].to_bytes(1, "big"))
    for pos in range(0x20, 0xFF + 1):
        if pos > 0x7F and pos < 0xA0:
            continue

        c = bytes([pos]).decode('ISO-8859-15')
        img_pil = Image.fromarray(img)
        draw = ImageDraw.Draw(img_pil)
        draw.text((0,0),  c, font = font, fill = (b, g, r, a))
        i = np.array(img_pil)[:, :, 2]
        for w in range(size[0]):
            n = 0
            for h in range(size[1]):
                n |= (1 if i[h, w] > 200 else 0) << (math.ceil((8-size[1]) / 2) + h)
            file.write(n.to_bytes(1, "big"))
