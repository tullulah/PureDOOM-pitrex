# piTrex DOOM — wireframe renderer

DOOM on Raspberry Pi Zero W (baremetal), rendering wireframe vectors
on the Vectrex analog display via piTrex.

## Architecture

```
PureDOOM (single header)
  ↓ BSP traversal
R_StoreWallRange() ← overridden in renderer.c
  ↓ screen-space quad (4 corners)
draw_vec()
  ↓
piTrex framework → Vectrex VIA 6522 → DAC → beam
```

## Required one-line patch to PureDOOM.h

Because PureDOOM is a single header that defines `R_StoreWallRange`
as a normal (strong) symbol, and we redefine it in `renderer.c`,
the linker will emit a duplicate-symbol error.

**Fix**: in `PureDOOM.h`, find the line:

```c
void R_StoreWallRange(int start, int stop)
{
```

and change it to:

```c
__attribute__((weak)) void R_StoreWallRange(int start, int stop)
{
```

This makes PureDOOM's version a weak symbol that our `renderer.c`
version overrides at link time.  One line, no other changes needed.

## SD card layout

```
/
├── kernel7l.img    ← doom binary (Pi Zero 2 W, 32-bit ARMv7)
├── bootcode.bin    ← Pi bootloader  (from rpi firmware repo)
├── start.elf       ← Pi firmware    (from rpi firmware repo)
├── fixup.dat       ← Pi firmware    (from rpi firmware repo)
├── config.txt      ← see below
└── doom1.wad       ← shareware or full IWAD
```

### config.txt

```ini
gpu_mem=64
arm_64bit=0
kernel=kernel7l.img
```

> **Pi Zero W vs Pi Zero 2 W**: the original Pi Zero W uses ARMv6 (`arm1176jzf-s`).
> The Pi Zero 2 W uses Cortex-A53 (ARMv8 chip, running here in 32-bit ARMv7 mode).
> The Makefile targets ARMv7 (`-march=armv7-a -mtune=cortex-a53 -DRASPPI=3`) for
> the Zero 2 W. If you have an original Zero W, revert those flags to
> `-march=armv6zk -mtune=arm1176jzf-s -DRASPPI=1` and rename the output to `kernel.img`.

### Firmware files

Download from the official Raspberry Pi firmware repository:
```
https://github.com/raspberrypi/firmware/tree/master/boot
```
You need: `bootcode.bin`, `start.elf`, `fixup.dat`.

## Build

```bash
# Baremetal (requires arm-none-eabi toolchain + piTrex SDK)
make PITREX_SDK=/path/to/pitrex-sdk

# Host SDL (renderer logic only, no piTrex hardware needed)
make run_sdl
```

## TODOs

- [ ] Implement `platform.c` stubs with real piTrex API calls
- [ ] Replace `draw_vec()` stub with `pitrex_draw_line()`
- [ ] Apply `__attribute__((weak))` patch to PureDOOM.h
- [ ] Tune `MAX_VECTORS_PER_FRAME` for brightness vs. detail tradeoff
- [ ] `main_sdl.c` — SDL host version for testing renderer on desktop
- [ ] Vertical edge culling (skip left/right edges when adjacent segs share them)
- [ ] Audio (optional — Vectrex has a PSG but it's secondary priority)
