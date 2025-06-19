# washing_machines_PUT
# Washing Machine Monitoring System 🚀

This project is an IoT-based monitoring system for washing machines in dormitories at **Poznań University of Technology**. It uses ESP32 microcontrollers and MPU6050 sensors to detect vibration patterns and determine the operational status of washing machines (e.g., idle, washing, spinning, finished).Now im switching to the option of only on/off classification,estimator of time in the code is written just for the test(didnt work at the end). Data is sent to a Flask backend and displayed in real-time via a web interface.

---

## 📦 Features

- 🧠 Detects washing machine state using vibration data (MPU6050)
- 📡 Sends real-time updates over Wi-Fi (ESP32)
- 🌐 RESTapi Flask backend 
- 🖼️ Web interface with dynamic images/GIFs for each machine status
- 🔌 Designed for 24/7 operation with durable power supply setup

---

## 🛠️ Hardware Components

- ESP32 wi-fi Dev Module
- MPU6050 Accelerometer + Gyroscope
- USB Power Adapter 
- Optional: 3D-printed casing 

---

