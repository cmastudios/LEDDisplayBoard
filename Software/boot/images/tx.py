#!/usr/local/bin/python3
"""
Convert each image to a raw binary format
"""

import numpy as np
import cv2
import glob
import os
import sys
import serial
import time

def encode_file(file):
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

    eyes_m = eyes[:,:,0]>0
    mouth_m = mouth[:,:,0]>0

    eyes_b = np.packbits(eyes_m.flatten(), axis=None, bitorder='little')
    mouth_b = np.packbits(mouth_m.flatten(), axis=None, bitorder='little')

    eyes_b = ''.join([f'{i:02x}' for i in eyes_b]).encode("ascii")
    mouth_b = ''.join([f'{i:02x}' for i in mouth_b]).encode("ascii")

    out = b'TX0200' + eyes_b + mouth_b + b'\r'
    return out

ANIME = 0.5

with serial.Serial('COM7', 115200, timeout=1) as ser:
    # time.sleep(3.0)
    if ANIME is not None:
        while True:
            for file in sys.argv[1:]:
                ser.write(encode_file(file))
                print(ser.readline())
                time.sleep(ANIME)
