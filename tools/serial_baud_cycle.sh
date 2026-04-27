#!/usr/bin/env bash
set -euo pipefail

# Interactive serial monitor cycling through multiple baud rates.
# Press ENTER (or any key) to stop current monitor and move to next speed.
# Press q to quit.

PORT="${1:-/dev/cu.usbserial-BG00VYKD}"
LOG_DIR="${2:-./serial_logs}"

BAUD_RATES=(
  921600
  460800
  230400
  115200
  57600
  38400
  19200
  9600
)

if [[ ! -e "$PORT" ]]; then
  echo "Error: serial device not found: $PORT"
  echo "Usage: $0 [/dev/cu.usbserial-XXXX] [log_dir]"
  exit 1
fi

mkdir -p "$LOG_DIR"

monitor_pid=""
CONTROL_TTY="/dev/tty"
advance_requested=0

if [[ ! -e "$CONTROL_TTY" ]]; then
  echo "Error: no controlling tty available ($CONTROL_TTY)."
  exit 1
fi

cleanup_monitor() {
  if [[ -n "${monitor_pid}" ]] && kill -0 "$monitor_pid" 2>/dev/null; then
    # Kill monitor reader. tee exits when its stdin closes.
    kill "$monitor_pid" 2>/dev/null || true
    wait "$monitor_pid" 2>/dev/null || true
  fi
  monitor_pid=""
}

cleanup_all() {
  cleanup_monitor
  stty sane 2>/dev/null || true
}

trap cleanup_all EXIT INT TERM
trap 'advance_requested=1' INT

echo "Serial baud cycle"
echo "Device : $PORT"
echo "Logs   : $LOG_DIR"
echo ""
echo "Controls while cycling:"
echo "  ENTER/any key: next baud"
echo "  q            : quit"
echo ""

for baud in "${BAUD_RATES[@]}"; do
  timestamp="$(date +%Y%m%d_%H%M%S)"
  logfile="$LOG_DIR/serial_${baud}_${timestamp}.log"

  echo "===================================================="
  echo "Baud: $baud"
  echo "Log : $logfile"
  echo ""
  echo "Now power on/reset the Pi and watch output below..."
  echo ""

  # Configure serial line (8N1, raw, no flow control)
  stty -f "$PORT" "$baud" cs8 -cstopb -parenb -ixon -ixoff -crtscts raw -echo

  # Start monitor for this baud and store output in log.
  cat "$PORT" > >(tee "$logfile") &
  monitor_pid=$!

  echo "[ENTER/any key: next, q: quit, Ctrl+C: next]" > "$CONTROL_TTY"

  key=""
  advance_requested=0
  while true; do
    if ! kill -0 "$monitor_pid" 2>/dev/null; then
      break
    fi

    if [[ "$advance_requested" -eq 1 ]]; then
      break
    fi

    if IFS= read -r -n 1 -s -t 1 key < "$CONTROL_TTY"; then
      break
    fi
  done

  cleanup_monitor

  if [[ "${key:-}" == "q" || "${key:-}" == "Q" ]]; then
    echo "Quit requested."
    exit 0
  fi

  echo ""
  echo "Moving to next baud..."
  echo ""
done

echo "Finished all baud rates."
