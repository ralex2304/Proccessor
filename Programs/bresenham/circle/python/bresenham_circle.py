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


def draw_circle(x0, y0, R):
    x = 0
    y = R
    R2 = R**2
    while x >= 0 and y >= 0:
        set_color(x + x0, y + y0)
        set_color(-x + x0, -y + y0)
        set_color(x + x0, -y + y0)
        set_color(-x + x0, y + y0)

        r2 = (x + 1)**2 + (y - 1)**2 - R2
        if r2 < 0:
            d2 = 2*(r2 + y) - 1
            x += 1
            if d2 > 0:
                y -= 1
        elif r2 > 0:
            d2 = 2*(r2 - x) - 1
            y -= 1
            if d2 <= 0:
                x += 1
        else:
            x += 1
            y -= 1

draw_circle(79, 102, 69)

out += "shw\n"

out += "in\n"


file = open(sys.argv[1], "w")

file.write(out)

file.close()
