import serial
import time
import csv
import os
import signal

PORT="COM10"
BAUD=115200

NUM_SAMPLES=200          # increased for ML

OUT_FILE="data/machine_01/healthy.csv"

WARMUP_SLEEP=5           # increased warmup

HEADER = [
    "timestamp",
    "rms",
    "peak",
    "crest",
    "variance",
    "kurtosis",
    "temp",
    "tempRise",
    "voltage",
    "current",
    "rpm",
    "dominant_freq",
    "harmonic_score",
    "peak_count",
    "vib_power_corr"
]

_interrupted=False


def ctrlc(sig,frame):

    global _interrupted

    print("\nStopping early...")

    _interrupted=True


signal.signal(signal.SIGINT,ctrlc)


def readline_safe(ser):

    try:
        return ser.readline().decode(errors="ignore").strip()

    except:
        return ""


def open_port():

    ser=serial.Serial()

    ser.port=PORT
    ser.baudrate=BAUD
    ser.timeout=2

    ser.dtr=False
    ser.rts=False

    ser.open()

    print("Opened",PORT)

    return ser


def wait_ready(ser):

    print("Waiting STM32_READY")

    timeout=time.time()+60

    while time.time()<timeout:

        line=readline_safe(ser)

        if not line:
            continue

        print("STM32:",line)

        if "STM32_READY" in line:

            print("STM32 ready detected")

            time.sleep(2)

            return True

    return False


def collect(ser):

    data=[]

    print("Warmup wait...")
    time.sleep(WARMUP_SLEEP)

    print("Collecting",NUM_SAMPLES,"samples")

    timeout=time.time()+180   # collection timeout
    start_time = time.time()

    while len(data)<NUM_SAMPLES and not _interrupted:
        line=readline_safe(ser)

        if not line:
            continue

        if not line.startswith("DATA"):
            continue

        try:

            parts=line.split(",")

            # if you add packet counter later use parts[2:]
            values=list(map(float,parts[1:]))

        except:
            continue

        if len(values)!=14:
            continue

        # sanity filtering (improves ML quality)

        if values[0] <= 0:
            continue

        if values[7] <150 or values[7] >260:
            continue
        if values[1] < values[0]:
            continue

        if values[3] < 0:
            continue

        if values[4] < 0:
            continue

        if values[9] < 0:
            continue

        #if values[9] <100:
        #    continue

        timestamp = time.time() - start_time

        data.append([timestamp] + values)

        n=len(data)

        print(n,"/",NUM_SAMPLES,end="\r")

    print()

    return data


def save(data):

    if not data:

        print("No data collected")

        return

    os.makedirs(os.path.dirname(OUT_FILE),exist_ok=True)

    exists=os.path.isfile(OUT_FILE)

    mode="a" if exists else "w"

    with open(OUT_FILE,mode,newline="") as f:

        writer=csv.writer(f)

        if not exists:
            writer.writerow(HEADER)

        writer.writerows(data)

    print("Saved",len(data),"samples")


def main():

    print("STM32 Dataset Collector")

    ser=open_port()

    time.sleep(3)

    # critical buffer cleanup
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    if not wait_ready(ser):

        print("STM32_READY not detected")

        ser.close()

        return

    print("Clearing buffer")

    time.sleep(2)

    ser.reset_input_buffer()

    data=collect(ser)

    ser.close()

    save(data)

    print("Done")


if __name__=="__main__":

    main()