import sys

SCREEN_HEIGHT = 200
SCREEN_WIDTH  = 200

out = ""

def set_color(x, y):
    global out

    if y < 0 or y >= SCREEN_HEIGHT or x < 0 or x >= SCREEN_WIDTH:
        return

    out += "push 255\n"
    out += "pop [" + str(SCREEN_WIDTH * y + x) + "]\n"


def draw_line(x0, y0, x1, y1):
    #steep
    steep = False
    if abs(x0 - x1) < abs(y0 - y1):
        x0, y0 = y0, x0
        x1, y1 = y1, x1

        steep = True

    #x0 to the left
    if x0 > x1:
        x0, x1 = x1, x0
        y0, y1 = y1, y0

    dx = x1 - x0

    error2dx = 0
    derror2dy = 2 * abs(y1 - y0)

    y = y0

    y_incr = (y0 < y1) if 1 else -1
    for x in range(x0, x1 + 1):
        if steep:
            set_color(y, x)
        else:
            set_color(x, y)

        error2dx += derror2dy

        if error2dx > dx:
            error2dx -= 2*dx
            y += y_incr


draw_line(1, 1, 100, 190)
draw_line(150, 180, 30, 40)

out += "shw\n"

out += "in\n"


file = open(sys.argv[1], "w")

file.write(out)

file.close()
