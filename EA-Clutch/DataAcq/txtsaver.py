import serial
import time
import re

# ---- CONFIG ----
PORT = '/dev/tty.usbmodem180126501'   # <-- change to your Teensy port
BAUD = 115200
FNAME = 'motor_load_data.txt'

# Regex for a line with exactly 5 numeric fields (ints/floats) separated by whitespace
NUMERIC5 = re.compile(
    r'^\s*'                                   # leading space
    r'([+-]?\d+(?:\.\d+)?)\s+'                # col 1
    r'([+-]?\d+(?:\.\d+)?)\s+'                # col 2
    r'([+-]?\d+(?:\.\d+)?)\s+'                # col 3
    r'([+-]?\d+(?:\.\d+)?)\s+'                # col 4
    r'([+-]?\d+(?:\.\d+)?)\s*'                # col 5
    r'$'
)

def is_numeric_row(line: str) -> bool:
    # Ignore obvious non-data lines quickly
    if not line or line[0].isalpha():
        return False
    if "PULSE" in line or "Motor" in line or "System ready" in line or "Initializing" in line:
        return False
    if "Time (ms)" in line:
        return False
    # Full regex match for 5 numeric fields
    return NUMERIC5.match(line) is not None

print(f"Opening {PORT} @ {BAUD}…")
with serial.Serial(PORT, BAUD, timeout=1) as ser:
    # Give Teensy time to reset on port open
    time.sleep(2)

    print(f"Recording numeric rows to {FNAME} (Ctrl+C to stop)\n")

    with open(FNAME, 'w', buffering=1) as f:  # line-buffered
        # Write a header that matches your firmware’s print order:
        # Time(ms) | Encoder | Transmitted(g) | Preload(g) | %Cycle(cumulative)
        f.write("time_ms\tencoder\ttransmitted_g\tpreload_g\tpercent_cycle\n")

        try:
            while True:
                raw = ser.readline()
                if not raw:
                    continue
                line = raw.decode('utf-8', errors='ignore').strip()
                if not line:
                    continue

                # Always echo to console
                print(line)

                # Only persist numeric data rows
                if is_numeric_row(line):
                    f.write(line + '\n')

        except KeyboardInterrupt:
            print("\nStopped by user.")
