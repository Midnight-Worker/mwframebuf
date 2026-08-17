import mwframebuf

fb = mwframebuf.Framebuffer(320, 200, 3, 60)

vram = fb.buffer()
palette = fb.palette()

palette[0] = 0xFF000000
palette[2] = 0xFFFF0000

x = 0

while fb.running():
    vram[:] = 0
    vram[80:120, x:x+40] = 2

    x += 2
    if x > 280:
        x = 0

    fb.flip()

fb.quit()
