import matplotlib.pyplot as plt
import numpy as np

# Pressures
pressures = np.array([50, 100, 150, 200, 250, 300])

# Area data (mm^2)
area_12mm = np.array([371.201, 377.536, 355.112, 289.616, 257.946, 225.920])
area_13mm = np.array([517.406, 515.591, 454.563, 397.188, 338.595, 328.685])
area_14mm = np.array([786.625, 670.424, 485.324, 388.431, 353.530, 295.324])
area_15mm = np.array([942.703, 704.423, 561.504, 464.995, 377.304, 330.272])
area_16mm = np.array([944.008, 612.691, 505.418, 446.836, 360.184, 326.158])

# Total possible area for each thickness
A_12 = np.pi * (20**2 - 8**2)   # ≈1055.58
A_13 = np.pi * (21**2 - 8**2)   # ≈1184.27
A_14 = np.pi * (22**2 - 8**2)   # ≈1319.47
A_15 = np.pi * (23**2 - 8**2)   # ≈1460.59
A_16 = np.pi * (24**2 - 8**2)   # ≈1609.73

# Normalize to get percentages
norm_12 = area_12mm / A_12 * 100
norm_13 = area_13mm / A_13 * 100
norm_14 = area_14mm / A_14 * 100
norm_15 = area_15mm / A_15 * 100
norm_16 = area_16mm / A_16 * 100

# Plot
plt.figure(figsize=(9, 6))

plt.plot(pressures, norm_12, marker='o', linestyle='-', color='red', label="12 mm")
plt.plot(pressures, norm_13, marker='s', linestyle='-', color='blue', label="13 mm")
plt.plot(pressures, norm_14, marker='^', linestyle='-', color='green', label="14 mm")
plt.plot(pressures, norm_15, marker='D', linestyle='-', color='purple', label="15 mm")
plt.plot(pressures, norm_16, marker='v', linestyle='-', color='orange', label="16 mm")

plt.xlabel("Pressure (mmHg)")
plt.ylabel("Contact Area (% of maximum)")
plt.title("Normalized Contact Area vs Pressure")
plt.legend(title="Thickness")
plt.grid(True)
plt.tight_layout()
plt.show()
