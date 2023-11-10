"""
mp4 video to assembler code encoder

console args: <input filename> <ouput filename>
"""

SCREEN_WIDTH  = 200
SCREEN_HEIGHT = 200

VIDEO_WIDTH   = 200
VIDEO_HEIGTH  = 150
TARGET_FPS = 6

import cv2
import math
import sys

def calc_color(bgr):
    ret = 0
    ret += bgr[0] << 0
    ret += bgr[1] << 8
    ret += bgr[2] << 16
    return ret

cap = cv2.VideoCapture(sys.argv[1])

out = ""
out += "fps " + str(TARGET_FPS) + "\n"

FPS = int(cap.get(cv2.CAP_PROP_FPS))

FRAME_COUNT = int(cap.get(cv2.CAP_PROP_FRAME_COUNT)) - 10 # last frames are usually broken

pixel_arr = [0] * (SCREEN_HEIGHT * SCREEN_WIDTH)

bef_frame = 0
for i in range(1, FRAME_COUNT, math.ceil(FPS / TARGET_FPS)):
    if i - bef_frame >= 100:
        print(i, "/", FRAME_COUNT)
        bef_frame = i

    cap.set(1, i)
    res, frame = cap.read() #frame has your pixel values

    #Get frame height and width to access pixels
    height, width, channels = frame.shape
    frame = cv2.resize(frame, (VIDEO_WIDTH, VIDEO_HEIGTH), interpolation = cv2.INTER_LANCZOS4)

    #Accessing BGR pixel values
    for x in range(0, VIDEO_WIDTH) :
        for y in range(0, VIDEO_HEIGTH) :
            color = calc_color(frame[y][x])
            if color != pixel_arr[y * SCREEN_WIDTH + x]:
                out += "push " + str(color) + "\n"
                out += "pop [" + str(y * SCREEN_WIDTH + x) + "]\n"
                pixel_arr[y * SCREEN_WIDTH + x] = color

    out += "shw\n"

out += "hlt\n"

file = open(sys.argv[2], "w")

file.write(out)

file.close()
