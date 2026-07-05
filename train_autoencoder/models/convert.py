import numpy as np

# ================= LOAD FILES =================

mean = np.load("scaler_mean.npy")
std = np.load("scaler_std.npy")
threshold = np.load("threshold.npy")

healthy_stats = np.load(
    "healthy_stats.npy",
    allow_pickle=True
).item()

# ================= PRINT =================

print("\n========== COPY INTO STM32 ==========\n")

print("static const float SCALER_MEAN[14] = {")
print("    " + ", ".join(f"{x:.6f}f" for x in mean))
print("};\n")

print("static const float SCALER_STD[14] = {")
print("    " + ", ".join(f"{x:.6f}f" for x in std))
print("};\n")

print(f"static const float THRESHOLD = {float(threshold):.6f}f;\n")

print("Healthy MSE Statistics")
print("----------------------")
print(f"Mean      : {healthy_stats['mean']:.6f}")
print(f"Std       : {healthy_stats['std']:.6f}")
print(f"Minimum   : {healthy_stats['min']:.6f}")
print(f"Maximum   : {healthy_stats['max']:.6f}")

print("\n=====================================")