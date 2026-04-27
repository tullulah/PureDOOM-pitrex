#!/usr/bin/env bash
set -e
cd "$(dirname "$0")"
make doom_sdl
echo "=== SDL build OK: ./doom_sdl ==="
