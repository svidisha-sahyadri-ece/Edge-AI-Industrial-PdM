import os
import numpy as np
import pandas as pd
import tensorflow as tf
import random
SEED = 42
random.seed(SEED)
np.random.seed(SEED)
tf.random.set_seed(SEED)
from sklearn.preprocessing import StandardScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.callbacks import EarlyStopping

# ================= CONFIG =================
DATA_FILE   = "data/machine_01/healthy.csv"
MODEL_DIR   = "models"

TFLITE_FILE = os.path.join(MODEL_DIR, "ae_model.tflite")
THRESH_FILE = os.path.join(MODEL_DIR, "threshold.npy")
MEAN_FILE   = os.path.join(MODEL_DIR, "scaler_mean.npy")
STD_FILE    = os.path.join(MODEL_DIR, "scaler_std.npy")

os.makedirs(MODEL_DIR, exist_ok=True)

# ================= LOAD DATA =================
print("[INFO] Loading data...")

df = pd.read_csv(DATA_FILE)

# Remove timestamp column
if "timestamp" in df.columns:
    df = df.drop(columns=["timestamp"])

# Remove accidental index column
if df.shape[1] == 15: 
     df = df.iloc[:, 1:]   

print("[OK] Data shape:", df.shape)

if df.shape[1] != 14:
    raise ValueError(f"Expected 14 features, got {df.shape[1]}")

print(df.head())

# ================= NORMALIZATION =================
# Shuffle dataset first
df = df.sample(frac=1, random_state=42).reset_index(drop=True)

scaler = StandardScaler()
X = scaler.fit_transform(df.values)
np.save(MEAN_FILE, scaler.mean_)
np.save(STD_FILE,  scaler.scale_)
print("[OK] Scaler saved")

# ================= AUTOENCODER =================
print("[INFO] Building autoencoder...")

model = Sequential([
    Dense(16, activation="relu", input_shape=(14,)),
    Dense(8,  activation="relu"),
    Dense(4,  activation="relu"),
    Dense(8,  activation="relu"),
    Dense(16, activation="relu"),
    Dense(14, activation="linear")
])

model.compile(optimizer="adam", loss="mse")
model.summary()

model.fit(
    X, X,
    epochs=100,
    batch_size=8,
    validation_split=0.2,
    callbacks=[
    EarlyStopping(
        monitor="val_loss",
        patience=15,
        restore_best_weights=True
    )
],
    verbose=1
)

# ================= THRESHOLD =================
print("[INFO] Calculating anomaly threshold...")

recon = model.predict(X)
mse   = np.mean(np.square(X - recon), axis=1)
np.save(
    os.path.join(MODEL_DIR, "healthy_mse.npy"),
    mse
)
healthy_mean = np.mean(mse)
healthy_std = np.std(mse)
healthy_stats = {
    "mean": float(healthy_mean),
    "std": float(healthy_std),
    "min": float(np.min(mse)),
    "max": float(np.max(mse))
}

np.save(
    os.path.join(MODEL_DIR, "healthy_stats.npy"),
    healthy_stats,
    allow_pickle=True
)

threshold = healthy_mean + 3.0 * healthy_std
np.save(THRESH_FILE, threshold)

print(f"[OK] Threshold: {threshold:.6f}")

# ================= PRINT C VALUES =================
print("\n========== COPY THESE INTO main.c ==========")

mean = np.load(MEAN_FILE)
std  = np.load(STD_FILE)

print("static const float SCALER_MEAN[14] = {")
print("   ", ", ".join([f"{v:.6f}f" for v in mean]))
print("};")

print("static const float SCALER_STD[14] = {")
print("   ", ", ".join([f"{v:.6f}f" for v in std]))
print("};")

print(f"static const float THRESHOLD = {threshold:.6f}f;")
print()
print("\nHealthy MSE Statistics")

print("-------------------------")

print(f"Mean      : {healthy_mean:.6f}")

print(f"Std       : {healthy_std:.6f}")

print(f"Minimum   : {np.min(mse):.6f}")

print(f"Maximum   : {np.max(mse):.6f}")

print(f"Threshold : {threshold:.6f}")

print("=============================================\n")

# ================= QUANTIZATION =================
print("[INFO] Quantizing model...")

def representative_dataset():
    for sample in X:
        yield [sample.reshape(1, -1).astype(np.float32)]

converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations          = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = representative_dataset

# REQUIRED for STM32
converter.inference_input_type   = tf.float32
converter.inference_output_type  = tf.float32

tflite_model = converter.convert()

with open(TFLITE_FILE, "wb") as f:
    f.write(tflite_model)

print(f"[OK] Model saved → {TFLITE_FILE}")
print(f"[OK] Model size  : {len(tflite_model)} bytes")

print("\n[DONE] Training complete!")
print("[NEXT] Load ae_model.tflite in X-CUBE-AI")
print("[NEXT] Paste MEAN, STD, THRESHOLD into main.c")