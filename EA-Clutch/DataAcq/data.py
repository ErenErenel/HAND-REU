import serial
import csv
import time

# ---- CONFIG ----
port = '/dev/tty.usbmodem155641601'  # Use your port (e.g., '/dev/ttyACM0' on Linux/macOS)
baud = 115200
output_file = 'loadcell_data1.csv'
duration_sec = 35  # How long to record

# ---- SETUP ----
ser = serial.Serial(port, baud, timeout=1)
time.sleep(2)  # Let Arduino reset
start = time.time()

with open(output_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    print(f"Recording to {output_file} for {duration_sec} seconds...")
    
    while time.time() - start < duration_sec:
        try:
            line = ser.readline().decode().strip()
            if line and ',' in line:
                row = line.split(',')
                writer.writerow(row)
                print(','.join(row))
        except Exception as e:
            print("Error:", e)

ser.close()
print("Recording complete.")
