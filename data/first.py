import os
import numpy as np
import tifffile as tiff
#import cv2
import matplotlib.pyplot as plt
#from skimage.segmentation import slic, mark_boundaries

def stretch_8bit(bands, lower_percent=2, higher_percent=98):
    out = np.zeros_like(bands)
    for i in range(3):
        a = 0 #np.min(band)
        b = 255  #np.max(band)
        c = np.percentile(bands[:,:,i], lower_percent)
        d = np.percentile(bands[:,:,i], higher_percent)        
        t = a + (bands[:,:,i] - c) * (b - a) / (d - c)    
        t[t<a] = a
        t[t>b] = b
        out[:,:,i] =t
    return out.astype(np.uint8)    
    
def RGB(image_id):
    filename = os.path.join('..', 'input', 'three_band', '{}.tif'.format(image_id))
    img = tiff.imread(filename)
    img = np.rollaxis(img, 0, 3)    
    return img
    
def M(image_id):
    filename = os.path.join('..', 'input', 'sixteen_band', '{}_M.tif'.format(image_id))
    img = tiff.imread(filename)    
    img = np.rollaxis(img, 0, 3)
    return img

