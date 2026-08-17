import mwframebuffer2
import numpy as np

WIDTH = 320
HEIGHT = 200

fb = mwframebuffer2.Framebuffer(
    WIDTH,
    HEIGHT,
    scale=3,
    fps=60
)

vram = fb.buffer()
palette = fb.palette()

# Palette
palette[0] = 0xFF000000  # schwarz
palette[1] = 0xFFFFFFFF  # weiß
palette[2] = 0xFFFF0000  # rot
palette[3] = 0xFF00FF00  # grün
palette[4] = 0xFF0000FF  # blau
palette[5] = 0xFFFFFF00  # gelb
palette[6] = 0xFFFF00FF  # magenta
palette[7] = 0xFF00FFFF  # cyan


x = 0
direction = 1

while fb.running():

    # kompletten Framebuffer löschen
    vram[:] = 0

    # --------------------------------
    # bewegtes rotes Rechteck
    # --------------------------------

    vram[20:50, x:x+60] = 2


    # --------------------------------
    # grüner Balken
    # --------------------------------

    vram[70:80, 20:300] = 3


    # --------------------------------
    # blaues Quadrat
    # --------------------------------

    vram[100:150, 30:80] = 4


    # --------------------------------
    # gelber Bereich
    # --------------------------------

    vram[120:180, 200:300] = 5


    # --------------------------------
    # Streifenmuster mit NumPy
    # --------------------------------

    vram[160:180, ::2] = 6


    # Bewegung
    x += direction * 2

    if x >= WIDTH - 60:
        x = WIDTH - 60
        direction = -1

    if x <= 0:
        x = 0
        direction = 1


    fb.flip()


fb.quit()
