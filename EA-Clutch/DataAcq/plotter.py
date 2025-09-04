import matplotlib.pyplot as plt
import numpy as np

# Pressure values (mmHg)
pressures = [0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50]

# Preload mean values (g) for each gap
preloads = {
    0.0762: [2.0136, 9.0136, 16.9808, 34.3125, 42.3748, 60.6842, 76.7047, 93.2013, 110.5389, 129.3597, 145.1242],
    0.1762: [0.5234, 6.2034, 11.9421, 26.7843, 37.1820, 56.3049, 70.2047, 90.4723, 118.2354, 126.0981, 143.0337],
    0.2762: [0.2021, 2.4051, 8.3135, 21.1152, 32.0952, 50.1302, 64.0592, 84.1822, 104.0047, 122.0318, 140.9652],
    0.3762: [0.0292, 1.0353, 6.9834, 17.2519, 28.6552, 45.9032, 60.6263, 81.2531, 100.8724, 118.4487, 138.4982],
    0.4762: [0.0031, 0.0512, 4.4527, 13.8515, 23.8881, 40.7222, 58.1187, 76.0042, 96.0076, 116.2021, 136.5319]
}


std_devs = {
    0.0762: [0.5722, 0.6283, 0.2354, 0.7307, 0.2490, 0.4132, 0.4705, 0.1794, 0.4070, 0.6494, 0.7223],
    0.1762: [0.2849, 0.3905, 0.5392, 0.7883, 0.5547, 0.3385, 0.8794, 0.6128, 0.2985, 0.4689, 0.6753],
    0.2762: [0.0224, 0.1623, 0.9086, 0.5611, 0.4732, 0.7851, 0.3984, 0.7869, 0.1813, 0.3861, 0.1942],
    0.3762: [0.0722, 0.1925, 0.6250, 0.7351, 0.2348, 0.8124, 0.2221, 0.4529, 0.2130, 0.2992, 0.3275],
    0.4762: [0.1742, 0.9109, 0.1227, 0.9151, 0.4965, 0.1124, 0.9568, 0.3541, 0.2849, 0.8641, 0.4581]
}

plt.figure(figsize=(10, 6))
for gap, values in preloads.items():
    errors = std_devs[gap]
    values_N = [v * 0.00980665 for v in values]
    errors_N = [e * 0.00980665 for e in errors]
    plt.errorbar(pressures, values_N, yerr=errors_N, label=f"{gap:.4f} mm gap", marker='o', linestyle='none', capsize=4)

plt.title("Preload vs. Pressure for Different Gaps (with Error Bars)", fontsize=13)
plt.xlabel("Pressure (mmHg)", fontsize=11)
plt.ylabel("Preload (N)", fontsize=11)
plt.legend(title="Clutch-Substrate Gap")
plt.grid(True)
plt.tight_layout()
plt.show()
