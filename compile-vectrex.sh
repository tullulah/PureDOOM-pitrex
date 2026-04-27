#!/usr/bin/env bash
set -e
export PATH="/opt/arm-toolchain/bin:$PATH"
cd "$(dirname "$0")"
make doom_pitrex
echo "=== Vectrex build OK: copy kernel7l.img + doom1.wad to SD card root ==="
