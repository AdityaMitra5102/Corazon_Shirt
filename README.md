# Corazon_Shirt

# Corazon Shirt
A shirt that can read the wearer’s ECG, SpO2, heart rate and is able to notify a family member in case of emergency. It can sense a partial heart blockage, a forthcoming myocardial infarction (heart attack), low oxygen saturation and low heart rate.
# Working
Using Arduino Nano (ATmega328P) with AD8232 sensor to read the ECG and MAX30102 sensor to sense SpO2, Heart rate, it monitors a person’s condition against a predefined limit for healthy parameters. Using  HC-05 Bluetooth Serial Port Protocol, an alert is sent to the wearer’s android device. A companion app, developed with MIT AI2 sends a SMS alert to emergency contact after an emergency signal is set off.
# Features
A SMS alert is sent to family/ emergency contact in case of deteriorating health (emergency). It comes with a companion android app and allows for easy integration with smart wearables like smart watches. It works on a 3.7v Li-ion rechargeable battery with charge time less than an hour from 5v 4A USB charger​ and high battery backup. The minimal companion app, being lightweight, can run in background without drawing much battery or without slowing down the device.
# Target Users
It is specially helpful in times of Covid, where it is difficult to stay close to a patient due to quarantine restrictions since it allows for a remote monitoring and alert system. However, it is useful in general for anyone to keep a regular track of their health parameters and inform their family, specially if they are living alone/ away from their family. It is also helpful for elderly people and people involved in much physical work. It can also be helpful for people with chronic heart condition.

[Video](https://youtu.be/bOj90gW0_6A)
