import mwframebuffer
import numpy as np
import time

WIDTH = 320
HEIGHT = 200

fb = mwframebuffer.Framebuffer(
    WIDTH,
    HEIGHT,
    scale=3,
    fps=60
)

pixels = fb.buffer()

x = 0
direction = 1

frame_count = 0
start_time = time.perf_counter()

while fb.running():
    # Bildschirm löschen
    pixels[:] = 0xFF000000

    # roten Block direkt in den Framebuffer schreiben
    pixels[80:120, x:x+40] = 0xFFFF0000

    # bewegen
    x += direction * 2

    if x >= WIDTH - 40:
        x = WIDTH - 40
        direction = -1

    if x <= 0:
        x = 0
        direction = 1

    fb.flip()

    # FPS messen
    frame_count += 1

    now = time.perf_counter()

    if now - start_time >= 1.0:
        print("FPS:", frame_count)

        frame_count = 0
        start_time = now

fb.quit()
