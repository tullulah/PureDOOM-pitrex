#!/usr/bin/env bash

PORT="${1:-/dev/cu.usbserial-BG00VYKD}"

if [[ ! -e "$PORT" ]]; then
  echo "❌ Serial device not found: $PORT"
  exit 1
fi

echo "🔧 Testing 921600 on macOS..."
echo "Make sure Pi Zero 2 W is powered ON with uart_diag running"
echo ""

# Configure port matching PuTTY Windows settings (XON/XOFF flow control enabled)
stty 921600 cs8 -cstopb -parenb ixon ixoff -crtscts raw -echo < "$PORT"

echo "✅ Reading from $PORT (press Ctrl+C to stop)..."
echo "---"

# Read directly with timeout
timeout 15 cat < "$PORT" 2>/dev/null

EXIT=$?
echo ""
echo "---"

if [ $EXIT -eq 124 ]; then
  echo "⏱️  Timeout after 15 seconds"
elif [ $EXIT -eq 0 ]; then
  echo "✅ Stream ended cleanly"
else
  echo "⏹️  Stopped (exit: $EXIT)"
fi
