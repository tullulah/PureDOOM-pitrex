#!/usr/bin/env python3
"""Read serial port at 921600 baud with XON/XOFF flow control."""
import sys
import termios
import tty
import os
import time

PORT = sys.argv[1] if len(sys.argv) > 1 else "/dev/cu.usbserial-BG00VYKD"
BAUD = int(sys.argv[2]) if len(sys.argv) > 2 else 921600

BAUD_MAP = {}
for _b in [9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600]:
    _flag = getattr(termios, f"B{_b}", None)
    if _flag is not None:
        BAUD_MAP[_b] = _flag

# macOS doesn't expose high baud constants in termios — use IOSSIOSPEED ioctl
import struct, ctypes
IOSSIOSPEED = 0x80045402  # macOS-specific ioctl to set arbitrary baud

print(f"Opening {PORT} at {BAUD} baud (XON/XOFF, 8N1)...")
print("Press Ctrl+C to stop.")
print("---")

fd = os.open(PORT, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)

# Get current settings
attrs = termios.tcgetattr(fd)

baud_flag = BAUD_MAP.get(BAUD)

# Set standard baud flag (use B9600 as placeholder if not in map)
attrs[4] = baud_flag if baud_flag else termios.B9600
attrs[5] = baud_flag if baud_flag else termios.B9600

# c_cflag: 8N1, no RTS/CTS
attrs[2] = termios.CS8 | termios.CREAD | termios.CLOCAL

# c_iflag: XON/XOFF flow control (matching PuTTY Windows settings)
attrs[0] = termios.IXON | termios.IXOFF

# c_lflag: raw mode
attrs[3] = 0

# c_cc: VMIN=0, VTIME=1 (non-blocking reads with 0.1s timeout)
attrs[6][termios.VMIN] = 0
attrs[6][termios.VTIME] = 1

termios.tcsetattr(fd, termios.TCSANOW, attrs)
termios.tcflush(fd, termios.TCIOFLUSH)

# On macOS, use IOSSIOSPEED ioctl to set arbitrary baud rate (921600, 460800, etc.)
if BAUD not in BAUD_MAP:
    import fcntl, struct
    IOSSIOSPEED = 0x80045402
    fcntl.ioctl(fd, IOSSIOSPEED, struct.pack("I", BAUD))
    print(f"Set baud via IOSSIOSPEED ioctl: {BAUD}")
else:
    print(f"Set baud via termios: {BAUD}")

# Set back to blocking mode
import fcntl
flags = fcntl.fcntl(fd, fcntl.F_GETFL)
fcntl.fcntl(fd, fcntl.F_SETFL, flags & ~os.O_NONBLOCK)

try:
    buf = b""
    while True:
        chunk = os.read(fd, 256)
        if chunk:
            # Print as text, replacing non-printable chars with dots
            sys.stdout.buffer.write(chunk)
            sys.stdout.flush()
except KeyboardInterrupt:
    print("\n---\nStopped.")
finally:
    os.close(fd)
