import numpy as np
import pandas as pd
from scipy.optimize import fsolve
import matplotlib.pyplot as plt

# EDGE and MIDDLE data extracted manually from the image
lo_values = np.array([12, 13, 14, 15, 16])  # in mm
pressures = np.array([50, 100, 150, 200, 250, 300])  # in mmHg

# EDGE: rows = Lo values, columns = pressure levels
edge_L = np.array([
    [11.75, 11.68, 11.62, 11.56, 11.54, 11.5],
    [12.54, 12.28, 12.2, 12.15, 12.12, 12.12],
    [13.43, 13.28, 13.15, 13.15, 13.13, 13.12],
    [14.71, 14.56, 14.45, 14.39, 14.3, 14.25],
    [15.76, 15.58, 15.48, 15.41, 15.33, 15.25]
])

# MIDDLE: rows = Lo values, columns = pressure levels
middle_L = np.array([
    [10.88, 9.68, 8.8, 8.59, 8.36, 8.08],
    [11.67, 10.33, 9.57, 9.17, 8.86, 8.6],
    [12.54, 11.94, 10.7, 10.2, 9.72, 9.56],
    [13.61, 12.35, 11.45, 10.9, 10.63, 10.42],
    [14.53, 12.98, 11.9, 11.61, 11.02, 10.7]
])

# Function to compute omega for a pair of Lo and L
def compute_omega(L0, L):
    target_ratio = L0 / L
    func = lambda omega: omega / np.sin(omega) - target_ratio
    return fsolve(func, 1.0)[0]

# Compute omega for EDGE and MIDDLE data

omega_edge = np.zeros_like(edge_L)
omega_middle = np.zeros_like(middle_L)


for i, L0 in enumerate(lo_values):
    for j in range(edge_L.shape[1]):
        omega_edge[i, j] = compute_omega(L0, edge_L[i, j])
        omega_middle[i, j] = compute_omega(L0, middle_L[i, j])

degree_edge = np.degrees(omega_edge)
degree_middle = np.degrees(omega_middle)

# Create DataFrames for display
df_edge = pd.DataFrame(degree_edge, index=[f"{lo} mm" for lo in lo_values], columns=[f"{p} mmHg" for p in pressures])
df_middle = pd.DataFrame(degree_middle, index=[f"{lo} mm" for lo in lo_values], columns=[f"{p} mmHg" for p in pressures])

print(df_edge)
print(df_middle)

import matplotlib.pyplot as plt

# Add 0 mmHg baseline with 0° angle for all thicknesses
zero_column = np.zeros((len(lo_values), 1))  # 0 degrees at 0 mmHg
omega_edge_full = np.hstack((zero_column, degree_edge))
omega_middle_full = np.hstack((zero_column, degree_middle))
pressure_labels = np.insert(pressures, 0, 0)  # add 0 mmHg

# Plot for EDGE
plt.figure(figsize=(10, 6))
for i, lo in enumerate(lo_values):
    plt.plot(pressure_labels, omega_edge_full[i], label=f"{lo} mm")
plt.title("Edge: Inflation Angle vs Pressure")
plt.xlabel("Pressure (mmHg)")
plt.ylabel("Inflation Angle θ (degrees)")
plt.legend(title="Thickness (Lo)")
plt.grid(True)
plt.tight_layout()
plt.show()

# Plot for MIDDLE
plt.figure(figsize=(10, 6))
for i, lo in enumerate(lo_values):
    plt.plot(pressure_labels, omega_middle_full[i], label=f"{lo} mm")
plt.title("Middle: Inflation Angle vs Pressure")
plt.xlabel("Pressure (mmHg)")
plt.ylabel("Inflation Angle θ (degrees)")
plt.legend(title="Thickness (Lo)")
plt.grid(True)
plt.tight_layout()
plt.show()