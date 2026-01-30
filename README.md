LifeSync – Offline Emergency Alert System

Team Sentinel

Overview

LifeSync is an offline wearable emergency alert system designed for elderly and cardiac-risk patients. A smart bracelet continuously monitors heart activity and automatically alerts a caregiver through a smart ring when abnormal patterns are detected. The system works without internet or smartphones, ensuring reliability during emergencies.

Problem Statement

During cardiac emergencies, patients may be unable to seek help themselves. Existing solutions rely on smartphones, internet connectivity, or manual user action, which can delay caregiver response when every second matters.

Proposed Solution

LifeSync consists of:

Patient Bracelet: Monitors heart rate and SpO₂ using a PPG sensor and detects sustained abnormal patterns.

Caregiver Ring: Receives instant offline alerts via RF communication, vibrates for attention, and displays a QR code to access the patient’s location.

Key Features

Fully offline operation (no internet or cloud dependency)

Automatic caregiver alert without patient action

QR-based location access (opens directly in Google Maps)

Elderly-friendly, low-power, and wearable design

System Architecture

Bracelet monitors heart activity continuously

Abnormal pattern detected using rule-based logic

Alert sent via RF (NRF24) to caregiver ring

Ring vibrates and displays QR code

Caregiver scans QR to view patient location

Hardware Components

MAX30100 Pulse Oximeter & Heart Rate Sensor

NRF24L01 RF Module

GPS Module

Microcontroller (ESP32 / Arduino)

OLED Display (SSD1306)

Vibration Motor

CR2450 Coin-Cell Battery

Detection Logic

LifeSync uses rule-based detection with thresholds and sustained instability checks to reduce false alerts. The system focuses on early emergency warning, not medical diagnosis.

Power & Safety

Non-rechargeable coin-cell battery for reliability and easy waterproofing

Electrically insulated, skin-safe materials

Designed to be water-resistant, with waterproofing planned in final product design

Current Limitations

Breadboard-level prototype

Rule-based detection (ML planned as future enhancement)

GPS reliability indoors may vary

Future Scope

ML-based personalized detection

Fall detection and additional vitals

Multi-caregiver alerting

Medical-grade enclosure and certifications

Disclaimer

LifeSync is an assistive emergency alert system and not a medical diagnostic device.
