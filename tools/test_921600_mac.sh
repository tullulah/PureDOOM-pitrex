#!/usr/bin/env bash

# Test 921600 baud on macOS using optimized stty settings for FT232RNL

PORT="${1:-/dev/cu.usbserial-BG00VYKD}"

if [[ ! -e "$PORT" ]]; then
  echo "❌ Serial device not found: $PORT"
  exit 1
fi

echo "🔧 Configuring $PORT at 921600 baud..."

# Backup original settings
ORIG_SETTINGS=$(stty -f "$PORT" -a 2>/dev/null || echo "")

# Configure with optimized flags for FT232RNL at 921600
# These flags match PuTTY Windows defaults
stty 921600 cs8 -cstopb -parenb -ixon -ixoff -crtscts -hupcl raw -echo < "$PORT"

echo "✅ Port configured. Reading from $PORT..."
echo "Press Ctrl+C to stop."
echo "---"

# Use dd to read with large buffer for better throughput at high baud
timeout 30 dd if="$PORT" bs=1024 2>/dev/null | cat -v

RESULT=$?

echo "---"
if [ $RESULT -eq 124 ]; then
  echo "⏱️  Timeout after 30 seconds (expected)"
else
  echo "⏹️  Stopped"
fi

# Restore original settings
if [[ -n "$ORIG_SETTINGS" ]]; then
  stty "$ORIG_SETTINGS" < "$PORT" 2>/dev/null || true
else
  stty sane < "$PORT" 2>/dev/null || true
fi

echo "✨ Done"
