import serial
import csv
import os
import time
import threading

PORT = '/dev/tty.usbmodem155641601'
BAUD = 115200
filename = 'automated.csv'

ser = serial.Serial(PORT, BAUD, timeout=1)
buffer = []
recording = False
stop_recording = False

def keyboard_listener():
    global recording, stop_recording
    input("Press ENTER to start recording...\n")
    recording = True
    print("Recording started. Press 'q' then ENTER to stop.\n")
    while True:
        key = input()
        if key.strip().lower() == 'q':
            stop_recording = True
            break

# Start keyboard listener thread
listener_thread = threading.Thread(target=keyboard_listener)
listener_thread.start()

print("Saving to:", os.path.abspath(filename))

while not stop_recording:
    if recording:
        try:
            line = ser.readline().decode('utf-8').strip()
            if not line:
                continue
            print(line)
            if "," in line:
                t_str, mass_str = line.split(",")
                t = float(t_str.strip())
                mass = float(mass_str.strip())
                buffer.append((t, mass))
        except Exception as e:
            print("Error:", e)

print("Done. Saving CSV...")

with open(filename, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Time (s)", "Mass (g)"])
    writer.writerows(buffer)

print("CSV saved to:", filename)
