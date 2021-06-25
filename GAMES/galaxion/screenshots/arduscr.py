#!/usr/bin/env python
# coding: UTF-8

from PIL import Image, ImageEnhance, ImageDraw

import Tkinter
from ScrolledText import *
import tkFileDialog;

import serial

#==============================================================================
def draw_img(canvas, img_bin, disp):
    ptr = 0
    image = Image.new('RGB', (128 * 4, 64 * 4))
    draw = ImageDraw.Draw(image)
    canvas.create_rectangle(0, 0, 128 * 4 + 4, 64 * 4 + 4, fill='black')
    for y in range(0, 64, 8):
        for x in range(128):
            data = ord(img_bin[ptr])
            ptr += 1
            for bit in range(8):
                if (data & (1 << bit)) == 0:
                    color = 'black'
                    dcolor = (0,0,0)
                else:
                    color = 'white'
                    dcolor = (255,255,255)

                if disp == 0:
                    canvas.create_rectangle(4 * x + 4, 4 * (y + bit) + 4,
                                            4 * x + 7, 4 * (y + bit) + 7,
                                            fill=color)
                    draw.rectangle([4 * x, 4 * (y + bit),
                                    4 * x + 3, 4 * (y + bit) + 3],
                                   fill=dcolor)
                elif disp == 1:
                    canvas.create_rectangle(4 * x + 4, 4 * (y + bit) + 4,
                                            4 * x + 7, 4 * (y + bit) + 6,
                                            fill=color, outline=color)
                    draw.rectangle([4 * x, 4 * (y + bit),
                                    4 * x + 3, 4 * (y + bit) + 2],
                                   fill=dcolor, outline=dcolor)
                else:
                    canvas.create_rectangle(4 * x + 4, 4 * (y + bit) + 4,
                                            4 * x + 7, 4 * (y + bit) + 7,
                                            fill=color, outline=color)
                    draw.rectangle([4 * x, 4 * (y + bit),
                                    4 * x + 3, 4 * (y + bit) + 3],
                                   fill=dcolor, outline=dcolor)
    del draw

    canvas.place(x=0,y=0)

    return image

#==============================================================================
def show_dialog(title, ser):
    root = Tkinter.Tk()
    root.title(title)
    root.geometry('520x320')

    canvas = Tkinter.Canvas(root, width=4*128, height=4*64)

    image = None

    def capture(event):
        '''capture Arduboy screen'''
        global image
        ser.write(chr(0))
        img_bin = ser.read(1024)
        image = draw_img(canvas, img_bin, 1)

        # ptr = 0
        # for y in range(0, 64, 8):
        #     for x in range(128):
        #         data = ord(img_bin[ptr])
        #         ptr += 1
        #         for bit in range(8):
        #             if (data & (1 << bit)) == 0:
        #                 color = 'black'
        #             else:
        #                 color = 'white'
        #             canvas.create_rectangle(4 * x + 4, 4 * (y + bit) + 4,
        #                                     4 * x + 7, 4 * (y + bit) + 7,
        #                                     fill=color)
        # canvas.place(x=0,y=0)

    btn_capture = Tkinter.Button(text='Capture')
    btn_capture.bind('<Button-1>', capture)
    btn_capture.place(x=20, y=280)

    def save(event):
        '''save captured screen'''
        global image
        if image is not None:
            opts = {'filetypes': [('image files', '.png')], 'initialdir': './'}
            #filename = tkFileDialog.asksaveasfilename(title='Save Image',initialdir='./',filetypes=(('image file','*.png')))
            filename = tkFileDialog.asksaveasfilename(**opts)
            if filename is not None:
                print 'save to %s' % filename
                image.save(filename)

    btn_save = Tkinter.Button(text='Save')
    btn_save.bind('<Button-1>', save)
    btn_save.place(x=100, y=280)

    def done(event):
        root.quit()

    btn_done = Tkinter.Button(text='Done')
    btn_done.bind('<Button-1>', done)
    btn_done.place(x=180,y=280)

    root.mainloop()

###############################################################################
from optparse import OptionParser
import sys
import os.path

if __name__ == '__main__':
    parser = OptionParser('usage: %prog (options) [TTY device file]')

    (options, args) = parser.parse_args()

    if (len(args) < 1):
        parser.error('need [TTY device file]')
        quit()

    ser = serial.Serial(args[0], 115200, timeout=3.0)

    show_dialog('Capture Arduboy [%s]' % args[0], ser)

    ser.close()
