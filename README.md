

# 🌡️ IoT-Based Dual Mode Set Point Control System

## 📌 Overview
This project implements an IoT-based temperature monitoring and control system using **LPC2148 microcontroller** and **ThingSpeak Cloud**. It supports both **local control (keypad)** and **remote control (cloud)** for setting temperature thresholds.

The system continuously monitors temperature and sends data to the cloud. Alerts are generated when temperature exceeds a predefined set point.

---

## 🎯 Objective
To design a smart system that allows:
- Real-time temperature monitoring
- Local and remote set point control
- Cloud-based data visualization and alerts

As described in your project file, the system reads temperature, compares it with a set point, and triggers alerts if exceeded :contentReference[oaicite:0]{index=0}.

---

## ⚙️ Hardware Components
- LPC2148 Microcontroller  
- LM35 Temperature Sensor  
- ESP01 Wi-Fi Module  
- 4x4 Matrix Keypad  
- LCD Display  
- EEPROM (AT25LC512)  
- Buzzer / LED  
- Switch  

---

## 💻 Software Requirements
- Embedded C  
- Keil uVision  
- Flash Magic  
- ThingSpeak Cloud  

---

## ☁️ ThingSpeak Integration
This project uses **ThingSpeak Cloud** for:

- 📊 Temperature Data Monitoring (Field 1)
- 🚨 Alert System (Field 2)
- 🔁 Remote Set Point Update (Separate Channel)

### 🔗 Features:
- Sends temperature data every 2–3 minutes  
- Reads updated set point from cloud periodically  
- Triggers alert when temperature > set point  

---

## 🔄 Working Principle

1. 🌡️ LM35 reads temperature
2. 📟 Value displayed on LCD
3. ☁️ Data sent to ThingSpeak
4. ⚖️ Compare with set point
5. 🚨 If exceeded:
   - Buzzer ON
   - Alert sent to cloud
6. 🔁 Set point can be updated:
   - Locally via keypad  
   - Remotely via ThingSpeak  

The system also stores set point in EEPROM to retain values during power failure :contentReference[oaicite:1]{index=1}.

---

## 🧠 Key Features
- Dual Mode Control (Local + Remote)
- Real-time Monitoring
- EEPROM Data Storage
- Cloud Alerts
- Interrupt-based Local Input

---

## 📊 ThingSpeak Setup

1. Create a channel on ThingSpeak
2. Add fields:
   - Field 1 → Temperature
   - Field 2 → Alert
3. Copy:
   - Write API Key
   - Read API Key
4. Use HTTP commands in ESP01:
https://api.thingspeak.com/update?api_key=YOUR_KEY&field1=VALUE


---

## 🚀 How to Run

1. Connect all hardware components
2. Flash code using Flash Magic
3. Configure ESP01 Wi-Fi
4. Send test data to ThingSpeak
5. Monitor data on dashboard

---

## 📌 Future Enhancements
- Mobile App Integration
- Multiple Sensor Support
- AI-based Temperature Prediction
- Email/SMS Alerts

---

## 🙌 Conclusion
This project demonstrates how IoT enables smart monitoring and control systems using cloud platforms like ThingSpeak.

---
