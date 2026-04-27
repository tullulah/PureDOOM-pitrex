# Makefile — piTrex DOOM
#
# Usage:
#   make doom_sdl         — host SDL build (macOS/Linux, for development)
#   make run_sdl          — build & run SDL
#   make doom_pitrex      — cross-compile for Pi Zero W baremetal
#   make clean

# -----------------------------------------------------------------------
# Paths
# -----------------------------------------------------------------------
PUREDOOM_DIR   := puredoom
PITREX_SDK     ?= $(HOME)/projects/pitrex-baremetal

# -----------------------------------------------------------------------
# Toolchain (baremetal ARM — must be GCC 10.3 gnu-rm, NOT 12.x/14.x)
# Install: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
# -----------------------------------------------------------------------
ARM_CC      = /opt/arm-toolchain/bin/arm-none-eabi-gcc
ARM_OBJCOPY = /opt/arm-toolchain/bin/arm-none-eabi-objcopy

PITREX_BASE_DIR := $(PITREX_SDK)
PITREX_LIB_DIR  := $(PITREX_SDK)/pitrex/lib7
PITREX_INC      := $(PITREX_SDK)/pitrex
PITREX_INC_USPI := $(PITREX_SDK)/pitrex/vectrex/uspi/include

ARM_CFLAGS = \
    -Ofast \
    -ffreestanding \
    -nostartfiles \
    -fuse-ld=bfd \
    -mhard-float -mfloat-abi=hard -mfpu=neon-fp-armv8 -march=armv8-a -mtune=cortex-a53 \
    -DRASPPI=3 -DUSE_PL011_UART=1 \
    -DPITREX_DEBUG \
    -I$(PUREDOOM_DIR) \
    -I$(PITREX_INC) \
    -I$(PITREX_INC_USPI)

ARM_LIBS = \
    -L$(PITREX_LIB_DIR) \
    -lvectrexInterface -luspi -lm -lc \
    $(PITREX_LIB_DIR)/linkerHeapDefBoot.ld \
    -lbaremetal

PITREX_SRCS = src/main.c src/platform.c src/renderer.c src/logger.c
PITREX_OBJS = $(patsubst src/%.c, build_pitrex/%.o, $(PITREX_SRCS))

# -----------------------------------------------------------------------
# Baremetal target
# -----------------------------------------------------------------------
doom_pitrex: $(PITREX_OBJS) src/weapons_assets.h src/sprites_assets.h src/hud_assets.h src/doom_logo.h
	$(ARM_CC) $(ARM_CFLAGS) $(PITREX_OBJS) $(ARM_LIBS) -o build_pitrex/doom.elf
	$(ARM_OBJCOPY) build_pitrex/doom.elf -O binary kernel7l.img
	@echo "=== Build OK: copy kernel7l.img + doom1.wad to SD card root ==="

build_pitrex/%.o: src/%.c | build_pitrex src/weapons_assets.h src/sprites_assets.h src/hud_assets.h src/doom_logo.h
	$(ARM_CC) $(ARM_CFLAGS) -c -o $@ $<

build_pitrex:
	mkdir -p build_pitrex

# -----------------------------------------------------------------------
# Host SDL build — for iterating on renderer logic without real hardware
# -----------------------------------------------------------------------
SDL_CC     = gcc
SDL_CFLAGS = -O0 -g -Wall -I$(PUREDOOM_DIR) \
             -DHOST_SDL \
             $(shell sdl2-config --cflags)
SDL_LIBS   = $(shell sdl2-config --libs)

run_sdl: doom_sdl
	./doom_sdl -iwad $(PUREDOOM_DIR)/doom1.wad


VEC_ASSETS = assets/weapons/pistol.vec assets/weapons/pistol_side.vec assets/weapons/pistol_flash.vec \
             assets/weapons/shotgun.vec assets/weapons/shotgun_side.vec assets/weapons/shotgun_flash.vec
src/weapons_assets.h: $(VEC_ASSETS)
	python3 tools/vec_to_c.py $(VEC_ASSETS) -o $@

SPRITE_ASSETS = \
	assets/sprites/enemy_humanoid.vec \
	assets/sprites/enemy_imp.vec \
	assets/sprites/enemy_caco.vec \
	assets/sprites/enemy_skull.vec \
	assets/sprites/enemy_demon.vec \
	assets/sprites/enemy_baron.vec \
	assets/sprites/enemy_cyber.vec \
	assets/sprites/enemy_spider.vec \
	assets/sprites/pickup_health.vec \
	assets/sprites/pickup_armor.vec \
	assets/sprites/pickup_ammo_clip.vec \
	assets/sprites/pickup_rocket.vec \
	assets/sprites/pickup_cell.vec \
	assets/sprites/pickup_weapon.vec \
	assets/sprites/pickup_key.vec \
	assets/sprites/pickup_powerup.vec \
	assets/sprites/pickup_barrel.vec \
	assets/sprites/unknown_enemy.vec \
	assets/sprites/unknown_pickup.vec \
	assets/sprites/corpse.vec \
	assets/sprites/decoration_pillar.vec \
	assets/sprites/barrel_explosion.vec
src/sprites_assets.h: $(SPRITE_ASSETS)
	python3 tools/vec_to_c.py $(SPRITE_ASSETS) -o $@
HUD_ASSETS = assets/hud/digit_0.vec assets/hud/digit_1.vec assets/hud/digit_2.vec assets/hud/digit_3.vec assets/hud/digit_4.vec \
	assets/hud/digit_5.vec assets/hud/digit_6.vec assets/hud/digit_7.vec assets/hud/digit_8.vec assets/hud/digit_9.vec \
	assets/hud/percent.vec \
	assets/hud/face_normal.vec assets/hud/face_pain.vec assets/hud/face_evil.vec \
	assets/hud/face_god.vec assets/hud/face_dead.vec \
	assets/hud/key_card.vec assets/hud/key_skull.vec
src/hud_assets.h: $(HUD_ASSETS)
	python3 tools/vec_to_c.py $(HUD_ASSETS) -o $@


LOGO_ASSETS = assets/doom_logo.vec
src/doom_logo.h: $(LOGO_ASSETS)
	python3 tools/vec_to_c.py $(LOGO_ASSETS) -o $@

doom_sdl: src/main_sdl.c src/renderer.c src/weapons_assets.h src/sprites_assets.h src/hud_assets.h src/doom_logo.h
	$(SDL_CC) $(SDL_CFLAGS) -o $@ src/main_sdl.c src/renderer.c $(SDL_LIBS)

clean:
	rm -rf build_pitrex doom.img src/*.o doom_sdl src/doom_logo.h

.PHONY: all clean run_sdl doom_pitrex
