import sys
import os

INPUT_FILE = "models/ae_model.tflite"
OUTPUT_FILE = "models/ae_model.cc"

var_name = "ae_model"

with open(INPUT_FILE, "rb") as f:
    data = f.read()

with open(OUTPUT_FILE, "w") as f:
    f.write(f"const unsigned char {var_name}[] = {{\n")

    for i, b in enumerate(data):
        if i % 12 == 0:
            f.write("  ")
        f.write(f"0x{b:02x}, ")
        if i % 12 == 11:
            f.write("\n")

    f.write("\n};\n")
    f.write(f"const unsigned int {var_name}_len = {len(data)};\n")

print("✅ C model generated:", OUTPUT_FILE)
