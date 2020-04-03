#!/usr/bin/env python
# coding: UTF-8

from PIL import Image, ImageEnhance

import Tkinter
from ScrolledText import *

###############################################################################
def draw_bitmap(canvas, rgb, max_xy, bmp_xy, start_xy, mask):
    max_x = max_xy[0]
    max_y = max_xy[1]
    for y in range(max_y):
        for x in range(max_x):
            r,g,b,a = rgb.getpixel((x, y))
            ofs_x = 4 * x + start_xy[0]
            ofs_y = 4 * y + start_xy[1]

            if mask:
                if r == 0 and g == 0 and b == 0:
                    if a == 0:
                        canvas.create_rectangle(ofs_x, ofs_y,
                                                (ofs_x + 3), (ofs_y + 3),
                                                fill='black')
                    else:
                        canvas.create_rectangle(ofs_x, ofs_y,
                                                (ofs_x + 3), (ofs_y + 3),
                                                fill='white')
                else:
                    canvas.create_rectangle(ofs_x, ofs_y,
                                            (ofs_x + 3), (ofs_y + 3),
                                            fill='black')
            else:
                if r == 0 and g == 0 and b == 0:
                    canvas.create_rectangle(ofs_x, ofs_y,
                                            (ofs_x + 3), (ofs_y + 3),
                                            fill='black')
                else:
                    canvas.create_rectangle(ofs_x, ofs_y,
                                            (ofs_x + 3), (ofs_y + 3),
                                            fill='white')

    for y in range(0, max_y+1, bmp_xy[1]):
        canvas.create_line(start_xy[0], y * 4 + start_xy[1],
                           max_x * 4 + start_xy[0], y * 4 + start_xy[1],
                           fill='blue', width=2)
    for x in range(0, max_x+1, bmp_xy[0]):
        canvas.create_line(x * 4 + start_xy[0], start_xy[1],
                           x * 4 + start_xy[0], max_y * 4 + start_xy[1],
                           fill='blue', width=2)

###############################################################################
def show_bitmap(title, rgb, max_xy, bmp_xy, bitmaps, mask=False):
    root = Tkinter.Tk()
    root.title(title)
    root.geometry('640x480')

    max_x = max_xy[0]
    max_y = max_xy[1]

    #
    # make image
    #
    canvas = Tkinter.Canvas(root, width=4*128, height=4*64 * 2)

    draw_bitmap(canvas, rgb, max_xy, bmp_xy, (4, 4), False)

    if mask:
        draw_bitmap(canvas, rgb, max_xy, bmp_xy, (4, max_y * 4 + 4), True)

    canvas.place(x=0,y=0)

    lcd_text = ''
    for bmp in bitmaps:
        for data in bmp:
            lcd_text += '0x%02x,' % data
        lcd_text += '\n'

    text_box = ScrolledText(width=84, height=8, bd=4)
    text_box.insert('end', lcd_text)
    text_box.place(x=0,y=280)

    def copy(event):
        root.clipboard_clear()
        root.clipboard_append(lcd_text)

    btn_copy = Tkinter.Button(text='Copy')
    btn_copy.bind('<Button-1>', copy)
    btn_copy.place(x=20,y=440)

    def done(event):
        root.quit()

    btn_done = Tkinter.Button(text='Done')
    btn_done.bind('<Button-1>', done)
    btn_done.place(x=100,y=440)

    root.mainloop()


###############################################################################
def img2bmp(rgb, bmp_xy, ofs_xy, mask=False):
    bmp = []
    for y in range(ofs_xy[1], ofs_xy[1] + bmp_xy[1], 8):
        for x in range(ofs_xy[0], ofs_xy[0] + bmp_xy[0]):
            data = 0
            for bit in range(y, y+8):
                data >>= 1
                if bit >= (ofs_xy[1] + bmp_xy[1]):
                    continue

                r,g,b,a = rgb.getpixel((x, bit))

                if mask:
                    if r == 0 and g == 0 and b == 0:
                        if a == 0:
                            data |= 0x00
                        else:
                            data |= 0x80
                    else:
                        data |= 0x00
                else:
                    if r == 0 and g == 0 and b == 0:
                        data |= 0x00
                    else:
                        data |= 0x80
            bmp.append(data)

    return bmp

###############################################################################
def trans_images(rgb, max_xy, bmp_xy, mask=False):
    bitmaps = []
    for ofs_y in range(0, max_xy[1], bmp_xy[1]):
        for ofs_x in range(0, max_xy[0], bmp_xy[0]):
            bmp = img2bmp(rgb, bmp_xy, (ofs_x, ofs_y))
            if bmp is not None:
                bitmaps.append(bmp)
            if mask:
                bmp = img2bmp(rgb, bmp_xy, (ofs_x, ofs_y), mask)
                if bmp is not None:
                    bitmaps.append(bmp)

    return bitmaps

###############################################################################
from optparse import OptionParser
import sys
import os.path

if __name__ == '__main__':
    parser = OptionParser('usage: %prog (options) [Image File]')

    parser.add_option('-v', '--verbose',
                      action='store_true', dest='verbose', default=False,
                      help='verbose mode')
    parser.add_option('-x', '--width',
                      type='int', dest='bmp_w', default=0, metavar='WIDTH',
                      help='devide image to bitmap with WIDTH')
    parser.add_option('-y', '--height',
                      type='int', dest='bmp_h', default=0, metavar='HEIGHT',
                      help='devide image to bitmap with HEIGHT')
    parser.add_option('-f', '--fit',
                      action='store_true', dest='fit', default=False,
                      help='fit image size to (128x64)')
    parser.add_option('-d', '--dither',
                      action='store_true', dest='dither', default=False,
                      help='use dither')
    parser.add_option('-g', '--gui',
                      action='store_true', dest='gui', default=False,
                      help='show GUI')
    parser.add_option('-2', '--two-dimension',
                      action='store_true', dest='two', default=False,
                      help='output two-dimensional array')
    parser.add_option('-b', '--black',
                      action='store_true', dest='mask', default=False,
                      help='make black layer bitmap')

    (options, args) = parser.parse_args()

    if (len(args) < 1):
        parser.error('need [Image File]')
        quit()

    # ---------------------------------------------------------- Load Image ---
    im = Image.open(args[0])

    filename = os.path.basename(im.filename)

    if options.fit:
        im.thumbnail((128, im.size[1] / (im.size[0] / 128)), Image.ANTIALIAS)

    if options.dither:
        eim = ImageEnhance.Brightness(im)
        rgb = eim.enhance(0.8).convert('1').convert('RGBA')
    else:
        rgb = im.convert('RGBA')

    # ------------------------------------------------------------ Get Size ---
    size = rgb.size

    img_x = size[0]
    img_y = size[1]
    #if (img_y % 8) != 0:
    #    img_y -= (img_y % 8)
    if img_x > 128:
        img_x = 128
    if img_y > 64:
        img_y = 64

    bmp_w = options.bmp_w
    if bmp_w == 0:
        bmp_w = img_x
    bmp_h = options.bmp_h
    if bmp_h == 0:
        bmp_h = img_y

    #print 'extract bitmap(%dx%d) from image(%dx%d)' % (bmp_w, bmp_h, img_x, img_y)

    num_bmps = (img_x / bmp_w) * (img_y / bmp_h)
    bitmaps = trans_images(rgb, (img_x, img_y), (bmp_w, bmp_h), options.mask)

    # ------------------------------------------- Show Bitmap Data with GUI ---
    if options.gui:
        show_bitmap(filename, rgb, (img_x, img_y), (bmp_w, bmp_h), bitmaps,
                    options.mask)

    # ------------------------------------------------------- Output Result ---
    print '// %d bitmap(s) in this byte array' % num_bmps
    print '// each bitmap size is (%d x %d)' % (bmp_w, bmp_h)
    if options.two:
        mul = bmp_h / 8
        if (bmp_h % 8) != 0:
            mul += 1
        print 'PROGMEM const uint8_t %s_bmp[][%d] = {' % (filename.split('.')[0], bmp_w * mul)
    else:
        print 'PROGMEM const uint8_t %s_bmp[] = {' % filename.split('.')[0]

    lcd_text = ''
    for bmp in bitmaps:
        lcd_text += '  '
        if options.two:
            lcd_text += '{\n    '
        for data in bmp:
            lcd_text += '0x%02x,' % data
        if options.two:
            lcd_text += '\n  },'
        lcd_text += '\n'
    print lcd_text,
    print '};'
