import cv2
import math

TARGET_FPS = 6

def count_color(bgr):
    ret = 0
    ret += bgr[0] << 0
    ret += bgr[1] << 8
    ret += bgr[2] << 16
    return ret

cap = cv2.VideoCapture("./Programs/badApple/badApple.mp4")

out = ""
out += "fps " + str(TARGET_FPS) + "\n"

FPS = int(cap.get(cv2.CAP_PROP_FPS))

FRAME_COUNT = int(cap.get(cv2.CAP_PROP_FRAME_COUNT)) - 10

bef_frame = 0
for i in range(1, FRAME_COUNT, math.ceil(FPS / TARGET_FPS)):
    if i - bef_frame >= 100:
        print(i, "/", FRAME_COUNT)
        bef_frame = i

    cap.set(1, i)
    res, frame = cap.read() #frame has your pixel values

    #Get frame height and width to access pixels
    height, width, channels = frame.shape
    frame = cv2.resize(frame, (96, 72), interpolation = cv2.INTER_LANCZOS4)

    #Accessing BGR pixel values
    for x in range(0, 96) :
        for y in range(0, 72) :
            out += "push " + str(count_color(frame[y][x])) + "\n"
            out += "pop [" + str(y * 100 + x) + "]\n"

    out += "shw\n"


out += "hlt\n"

file = open("./Programs/badApple/main.code", "w")

file.write(out)

file.close()
