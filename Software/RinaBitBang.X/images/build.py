#!/usr/local/bin/python3
"""
Convert each image to a raw binary format
"""

import numpy as np
import cv2
import glob
import os

for file in glob.glob("*.png"):
    name = os.path.splitext(file)[0]
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
    with open(f'{name}.bin', 'wb') as f:
        f.write(eyes.flatten())
        f.write(mouth.flatten())
        assert f.tell() == 660

