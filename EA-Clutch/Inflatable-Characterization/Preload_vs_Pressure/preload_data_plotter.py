import csv
import matplotlib.pyplot as plt

# ----- Config -----
filename = "automated.csv"
label = "New Inflatable"
time_offset = 30  # seconds to subtract (delay before recording started)

# ----- Read data -----
time_values = []
mass_values = []

with open(filename, 'r') as file:
    reader = csv.reader(file)
    next(reader)  # Skip header
    for row in reader:
        if len(row) != 2:
            continue
        time_values.append(float(row[0]))
        mass_values.append(float(row[1]))

# Normalize time and apply time offset
start_time = time_values[0]
time_values = [(t - start_time) - time_offset for t in time_values]

# Negate mass for positive preload
mass_values = [-1 * m for m in mass_values]

# ----- Plot -----
plt.figure(figsize=(12, 6))
plt.plot(time_values, mass_values, linewidth=2, label=label)

# Pressure steps (adjusted to your timeline)
pressure_steps = [0, 50, 80, 104, 124, 138, 154]  # In seconds (after correction)
pressures = [0, 5, 10, 15, 20, 25, 30]

for t, p in zip(pressure_steps, pressures):
    plt.axvline(x=t, color='black', linestyle='--', linewidth=1)
    plt.text(t + 1, max(mass_values) * 0.9, f"{p} mmHg", rotation=90, color='red', va='top')

# Labels and formatting
plt.xlabel("Time (s)")
plt.ylabel("Preload (g)")
plt.title("Preload vs Time with Pressure Markers")
plt.xlim(0, max(time_values))
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
