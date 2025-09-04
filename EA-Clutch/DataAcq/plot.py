import csv
import matplotlib.pyplot as plt

# -------- CONFIG --------
csv_filename = 'loadcell_data.csv'  # Make sure this matches your saved file
time_col = 0     # Column index for time (seconds)
grams_col = 2    # Column index for grams (filtered weight)

# -------- LOAD DATA --------
time_vals = []
gram_vals = []

with open(csv_filename, 'r') as csvfile:
    reader = csv.reader(csvfile)
    header = next(reader)  # Skip header row
    for row in reader:
        try:
            time_vals.append(float(row[time_col]))
            gram_vals.append(float(row[grams_col]))
        except:
            pass  # Skip any malformed rows

# -------- PLOT --------
plt.figure(figsize=(10, 5))
plt.plot(time_vals, gram_vals, label='Weight (g)', color='blue')
plt.title("Load Cell Data")
plt.xlabel("Time (s)")
plt.ylabel("Weight (grams)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
