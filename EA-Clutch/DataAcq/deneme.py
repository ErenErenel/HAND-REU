import numpy as np
import matplotlib.pyplot as plt

# ---- LOAD DATA ----
fname = "motor_load_data.txt"
data = np.loadtxt(fname, skiprows=1)  # Skip header

# Columns from file
time_ms        = data[:, 0]
encoder_counts = data[:, 1]
wrong_trans_g  = data[:, 2]  # actually preload
wrong_preload_g= data[:, 3]  # actually transmitted
percent_cycle  = data[:, 4]

# Swap meanings without touching the Arduino code
preload_g      = wrong_trans_g
transmitted_g  = wrong_preload_g

# Convert to seconds
time_s = time_ms / 1000.0

# Convert grams → newtons
g_to_N = 9.80665 / 1000.0
preload_N     = preload_g * g_to_N
transmitted_N = transmitted_g * g_to_N

# ---- PLOT ----
fig, axs = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

# Preload
axs[0].plot(time_s, preload_N, color='blue', label="Preload (N)")
axs[0].set_ylabel("Preload (N)")
axs[0].legend()
axs[0].grid(True)

# Transmitted load
axs[1].plot(time_s, transmitted_N, color='orange', label="Transmitted (N)")
axs[1].set_ylabel("Transmitted (N)")
axs[1].legend()
axs[1].grid(True)

# Percent cycle
axs[2].plot(time_s, percent_cycle, color='green', label="% Cycle (cumulative)")
axs[2].set_ylabel("% of Cycle")
axs[2].set_xlabel("Time (s)")
axs[2].legend()
axs[2].grid(True)

plt.tight_layout()
plt.show()
