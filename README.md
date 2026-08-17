# MWFrameBuf

A small retro-style 8-bit framebuffer for Python, powered by C++, SDL3 and pybind11.

## Example

```python
import mwframebuf

fb = mwframebuf.Framebuffer(320, 200)

vram = fb.buffer()

while fb.running():
    vram[:] = 0
    vram[50:100, 100:200] = 2
    fb.flip()

fb.quit()
