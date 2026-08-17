import mwframebuffer2
import numpy as np
import time


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


print("VRAM:", vram.shape, vram.dtype)
print("Palette:", palette.shape, palette.dtype)


# eigene Farben setzen
palette[0] = 0xFF000000
palette[1] = 0xFFFFFFFF
palette[2] = 0xFFFF0000
palette[3] = 0xFF00FF00
palette[4] = 0xFF0000FF
palette[5] = 0xFFFFFF00
palette[6] = 0xFFFF00FF
palette[7] = 0xFF00FFFF


x = 0
direction = 1

frames = 0
start = time.perf_counter()


while fb.running():

    # Farbe 0 = schwarz
    vram[:] = 0

    # Rechteck mit Farbindex 2 = rot
    vram[80:120, x:x+40] = 2

    # kleiner grüner Balken
    vram[20:30, 20:300] = 3

    x += direction * 2

    if x >= WIDTH - 40:
        x = WIDTH - 40
        direction = -1

    if x <= 0:
        x = 0
        direction = 1

    fb.flip()

    frames += 1

    now = time.perf_counter()

    if now - start >= 1.0:
        print("FPS:", frames)
        frames = 0
        start = now


fb.quit()
