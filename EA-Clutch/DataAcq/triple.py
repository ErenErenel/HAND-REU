import numpy as np
import matplotlib.pyplot as plt

# ------------------------------
# Config
# ------------------------------
FNAME = "motor_load_daaaata.txt"
COUNTS_PER_REV = 375000  # must match your firmware

# ------------------------------
# Load data
# ------------------------------
data = np.loadtxt(FNAME, skiprows=1)

time = data[:, 0] / 1000.0            # seconds
encoder = data[:, 1]                  # counts
preload_g = data[:, 2]                # actually PRELOAD (g)
transmitted_g = data[:, 3]            # actually TRANSMITTED (g)

# Convert grams -> newtons
g_to_N = 9.80665 / 1000.0
preload = preload_g * g_to_N
transmitted = transmitted_g * g_to_N

# Clamp transmitted to ±5 N
transmitted = np.clip(transmitted, -5.0, 5.0)

# ------------------------------
# Find revolution markers
# ------------------------------
rev_lines = []
if encoder.size > 1:
    max_rev = int(np.floor(encoder.max() / COUNTS_PER_REV))
    thresholds = (np.arange(1, max_rev + 1) * COUNTS_PER_REV).astype(float)
    enc_prev = np.r_[encoder[0], encoder[:-1]]

    for thr in thresholds:
        hits = np.where((enc_prev < thr) & (encoder >= thr))[0]
        if hits.size:
            i = hits[0]
            if i == 0 or encoder[i] == enc_prev[i]:
                t_cross = time[i]
            else:
                frac = (thr - enc_prev[i]) / (encoder[i] - enc_prev[i])
                t_cross = time[i-1] + frac * (time[i] - time[i-1])
            rev_lines.append(t_cross)

# ------------------------------
# Plotting
# ------------------------------
fig, axs = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

# Preload vs Time
axs[0].plot(time, preload, label="Preload (N)")
axs[0].set_ylabel("Preload (N)")
axs[0].legend()
axs[0].grid(True)

# Voltage vs Time (placeholder)
axs[1].set_ylabel("Voltage (V)")
axs[1].set_ylim([-250, 250])
axs[1].legend(["Voltage (blank)"])
axs[1].grid(True)

# Transmitted vs Time (clamped)
axs[2].plot(time, transmitted, label="Transmitted (N)")
axs[2].set_ylabel("Transmitted (N)")
axs[2].set_xlabel("Time (s)")
axs[2].legend()
axs[2].grid(True)

# Add revolution markers
for t in rev_lines:
    for ax in axs:
        ax.axvline(x=t, linestyle='--', color='gray', alpha=0.6)

plt.tight_layout()
plt.show()
