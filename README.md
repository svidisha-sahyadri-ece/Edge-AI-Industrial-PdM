# Autoencoder-Based Edge AI System for Real-Time Multi-Sensor Industrial Predictive Maintenance

## Overview

This project presents an **Edge AI-based Predictive Maintenance (PdM)** system for real-time industrial machine health monitoring using an **STM32F401RE** microcontroller. The system continuously acquires data from multiple sensors, performs on-device signal processing and AI-based anomaly detection using an **Autoencoder**, and displays real-time diagnostics through an **ESP32-hosted web dashboard**.

Unlike conventional cloud-based monitoring systems, all signal processing and AI inference are performed locally on the STM32, enabling low-latency, reliable, and cloud-independent machine health monitoring.

---

## Features

- Real-time machine health monitoring
- Edge AI-based anomaly detection
- On-device signal processing
- Multi-sensor data acquisition
- 14-feature extraction
- Health Score computation
- Confidence estimation
- Remaining Useful Life (RUL) estimation
- ESP32 web dashboard
- Low-cost and scalable architecture

---

# System Architecture

The system consists of two stages:

## Stage 1 – Offline Model Development

- Sensor data acquisition using STM32
- Feature extraction
- Dataset collection using Python
- Data normalization
- Autoencoder training in TensorFlow
- TensorFlow Lite model conversion
- Deployment using STM32 X-CUBE-AI

## Stage 2 – Real-Time Edge Deployment

- Continuous sensor acquisition
- Feature extraction on STM32
- Feature normalization
- Embedded AI inference
- MSE-based anomaly detection
- Health assessment
- UART communication to ESP32
- Live dashboard visualization

---

# Hardware Components

- STM32F401RE Nucleo Board
- ESP32 Development Board
- ADXL345 Accelerometer
- DS18B20 Temperature Sensor
- ACS712 Current Sensor
- ZMPT101B Voltage Sensor
- Hall Effect RPM Sensor
- Single Phase AC Motor

---

# Software & Tools

- STM32CubeIDE
- STM32 X-CUBE-AI
- TensorFlow
- TensorFlow Lite
- Python
- Arduino IDE
- HTML
- CSS
- JavaScript
- Git

---

# Signal Processing

The STM32 performs complete on-chip signal processing.

### Time Domain Features

- RMS
- Peak
- Crest Factor
- Variance
- Kurtosis

### Frequency Domain Features

- Dominant Frequency
- Harmonic Score
- Peak Count

### Additional Features

- Temperature
- Temperature Rise
- Voltage
- Current
- RPM
- Vibration Power Correlation

A total of **14 features** are extracted and used as input to the embedded Autoencoder model.

---

# Embedded AI Workflow

1. Acquire sensor data
2. Pre-process signals
3. Extract 14-feature vector
4. Normalize features
5. Run Autoencoder inference
6. Compute reconstruction error (MSE)
7. Detect anomalies
8. Estimate Health Score
9. Estimate Confidence
10. Estimate Trend
11. Estimate Remaining Useful Life (RUL)
12. Send results to ESP32
13. Display diagnostics on the web dashboard

---

# Results

The developed system successfully performs:

- Real-time machine health monitoring
- Embedded AI inference on STM32
- Multi-sensor feature extraction
- Anomaly detection using reconstruction error
- Real-time Health Score estimation
- Remaining Useful Life prediction
- Live web dashboard visualization

The complete solution operates without cloud dependency, making it suitable for industrial edge computing applications.

---

# Applications

- Predictive Maintenance
- Industrial IoT
- Machine Health Monitoring
- Smart Manufacturing
- Rotating Machine Diagnostics
- Industry 4.0
- Embedded AI

---

# Skills Demonstrated

- Embedded Systems
- STM32 Firmware Development
- Edge AI
- Embedded Machine Learning
- Signal Processing
- Sensor Interfacing
- UART Communication
- ESP32 Development
- TensorFlow Lite
- X-CUBE-AI
- Embedded C
- Industrial IoT

---

# Future Enhancements

- Support for multiple industrial machines
- TinyML optimization
- OTA firmware updates
- Cloud analytics integration
- Mobile monitoring application
- Advanced fault classification

---


