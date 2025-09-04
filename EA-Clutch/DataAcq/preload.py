import serial
import time
import statistics

# ---- CONFIG ----
port = '/dev/tty.usbmodem155641601'  # Update this as needed
baud = 115200
num_samples = 100

# ---- SETUP ----
ser = serial.Serial(port, baud, timeout=1)
time.sleep(2)  # Allow board to reset

print(f"Collecting {num_samples} gram readings...")

gram_values = []

while len(gram_values) < num_samples:
    try:
        line = ser.readline().decode().strip()
        if line and ',' in line:
            parts = line.split(',')
            if len(parts) >= 3:
                gram = float(parts[2])  # 3rd column
                gram_values.append(gram)
    except Exception as e:
        print("Error:", e)

ser.close()

# ---- RESULTS ----
mean_gram = sum(gram_values) / len(gram_values)
std_gram = statistics.stdev(gram_values)

print(f"\nSummary over {num_samples} samples:")
print(f"Mean:\n{mean_gram:.4f} g")
print(f"SD:\n{std_gram:.4f} g")


