#!/usr/local/bin/python3
"""
Convert each image to a raw binary format
"""

import numpy as np
import cv2
import glob
import os
import sys

hdr_name = sys.argv[2]
f = open(sys.argv[1], "w")
h = open(hdr_name, "w")

header = """
#pragma once
#include <stdint.h>
typedef union image {
    struct
    {
        uint8_t eyes[20 * 8 * 3];
        uint8_t mouth[10 * 6 * 3];
    };
    uint8_t raw[220 * 3];
} image_U;
"""
h.write(header)
source = f'\n#include "{os.path.basename(hdr_name)}"\n'
f.write(source)
variables = []

for file in sys.argv[3:]:
    name = os.path.splitext(os.path.basename(file))[0]
    img = cv2.imread(file)
    assert img.shape == (14, 20, 3)
    assert img.dtype == np.uint8
    #convert bgr to grb (WS2812 order)
    B,G,R = img.T
    img = np.array((G,R,B)).T
    #replace pure white with blank (the screen will make dark pixels appear white)
    mask = (img[:,:,0]>230) & (img[:,:,1]>230) & (img[:,:,2]>230)
    img[np.where(mask)] = 0 
    # set brightness to 10% (prevent killing the LEDs)
    img //= 10
    assert img.max() < 26
    assert img.dtype == np.uint8

    eyes = img[:8, 0:20]
    mouth = img[8:, 5:15]

    h.write(f"extern const image_U image_{name};\n")
    f.write(f"const image_U image_{name} = {{ .raw = {{\n")
    f.write(','.join(f'0x{i:02x}U' for i in eyes.flatten()))
    f.write(',')
    f.write(','.join(f'0x{i:02x}U' for i in mouth.flatten()))
    f.write("} };\n")

h.close()
f.close()
