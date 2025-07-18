import matplotlib.pyplot as plt
import numpy as np

# Pressures
pressures = np.array([50, 100, 150, 200, 250, 300])

# Area data
area_12mm = np.array([371.201, 377.536, 355.112, 289.616, 257.946, 225.92])
area_13mm = np.array([517.406, 515.591, 454.563, 397.188, 338.595, 328.685])
area_14mm = np.array([786.625, 670.424, 485.324, 388.431, 353.53, 295.324])
area_15mm = np.array([942.703, 704.423, 561.504, 464.995, 377.304, 330.272])
area_16mm = np.array([944.008, 612.691, 505.418, 446.836, 360.184, 326.158])

# Create plot
plt.figure(figsize=(8, 6))
plt.plot(pressures, area_12mm, 'r-', label="12 mm")
plt.plot(pressures, area_13mm, 'b-', label="13 mm")
plt.plot(pressures, area_14mm, 'o-', label="14 mm")
plt.plot(pressures, area_15mm, 's-', label="15 mm")
plt.plot(pressures, area_16mm, '^-', label="16 mm")

plt.xlabel("Pressure (mmHg)")
plt.ylabel("Contact Area (mm²)")
plt.title("Contact Area vs Pressure")
plt.legend(title="Thickness")
plt.grid(True)
plt.tight_layout()
plt.show()
