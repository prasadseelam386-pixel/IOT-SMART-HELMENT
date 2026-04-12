# IOT-SMART-HELMENT
Here’s a clean and professional **GitHub README.md** file for your Smart Helmet project based on your code + document:

---

# 🪖 Smart Helmet with IoT 🚀

An **IoT-Based Smart Helmet System** designed to improve bike rider safety by integrating sensors, real-time monitoring, and emergency alert mechanisms using **NodeMCU (ESP8266)** and the **Blynk platform**.

---

## 📌 Features

* ✅ Helmet detection (prevents riding without helmet)
* 🚨 Accident detection using MPU6050 (gyro + accelerometer)
* 🍺 Alcohol detection using gas sensor
* 📍 GPS-based location tracking
* 📲 Real-time alerts via Blynk app
* 🔊 Buzzer alert system for emergencies
* ⚙️ Motor control (engine ON/OFF based on safety conditions)

---

## 🛠️ Hardware Components

* NodeMCU (ESP8266)
* MPU6050 Gyroscope & Accelerometer
* Gas Sensor (Alcohol detection)
* GPS Module (NEO-6M)
* LCD Display (16x2 I2C)
* Buzzer
* Motor Driver / Relay
* Switch (Helmet detection)
* Power Supply (Battery)

---

## 💻 Software Requirements

* Arduino IDE
* Blynk IoT Platform
* Embedded C Programming

---

## 🔌 Circuit Connections

| Component   | NodeMCU Pin |
| ----------- | ----------- |
| MPU6050 SDA | D2          |
| MPU6050 SCL | D1          |
| GPS RX/TX   | D5, D6      |
| Buzzer      | D3          |
| Motor       | D8          |
| Switch      | D7          |
| Gas Sensor  | A0          |

---

## ⚙️ Working Principle

1. **Helmet Detection**

   * If helmet is not worn → motor OFF + alert triggered

2. **Alcohol Detection**

   * If alcohol level > threshold → motor OFF + alert

3. **Accident Detection**

   * MPU6050 detects sudden motion/impact
   * Triggers:

     * Buzzer
     * Blynk alert
     * Location tracking

4. **IoT Communication**

   * Data sent to Blynk app via WiFi
   * Real-time notifications to user

---

## 📱 Blynk Setup

1. Create a project in Blynk
2. Add:

   * Virtual Pin V0 → Sensor Data
3. Copy **Auth Token**
4. Update in code:

   ```cpp
   #define BLYNK_AUTH_TOKEN "YOUR_TOKEN"
   ```

---

## 🚀 Installation Steps

1. Install Arduino IDE
2. Add ESP8266 Board Manager
3. Install required libraries:

   * Blynk
   * TinyGPS++
   * LiquidCrystal_I2C
   * Wire
4. Upload code to NodeMCU
5. Connect hardware components
6. Run and monitor via Serial + Blynk App

---

## 📊 System Architecture

```
Sensors → NodeMCU → WiFi → Blynk App
        ↓
     Buzzer / Motor Control
```

---

## 🔔 Alerts Generated

* 🚫 Helmet not worn
* 🍺 Alcohol detected
* 💥 Accident detected

---

## 📸 Output

* LCD Display shows:

  * Helmet status
  * Alcohol level
  * Temperature
* Mobile App shows:

  * Alerts & notifications

---

## 🧪 Testing

✔ Helmet Detection – Accurate
✔ Accident Detection – Reliable
✔ Notifications – Real-time
✔ System Stability – High

---

## 🔮 Future Scope

* 📍 GPS live tracking on map
* 🤖 AI-based accident prediction
* 📱 Dedicated mobile app
* 🔋 Power optimization
* 🚑 Emergency contact integration

---

## 📎 GitHub Repository

👉 [https://github.com/prasadseelam386-pixel/Smart-Helmet-With-IOT-.git](https://github.com/prasadseelam386-pixel/Smart-Helmet-With-IOT-.git)

---

## 👨‍💻 Authors

* Project developed as part of IoT/DBMS academic work

---

## 📜 License

