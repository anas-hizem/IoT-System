# IoT System with ESP32, DHT11 Sensor, LED, and Node-RED

## Project Description

This project demonstrates the implementation of an IoT system using an ESP32 microcontroller, a DHT11 temperature and humidity sensor, an LED, and Node-RED as the visualization and control platform. The system leverages the MQTT protocol for seamless communication between devices, ensuring efficient data exchange and control.

The main goal of this project is to create an interactive IoT setup that collects real-time temperature and humidity data, visualizes it on Node-RED, and allows for remote control of an LED using MQTT messages.

---

## Key Features

1. **Temperature Monitoring**:  
   - The ESP32 collects temperature data using the DHT11 sensor.  
   - The readings are published to the `esp32/temperature` MQTT topic.  

2. **Humidity Monitoring**:  
   - The ESP32 collects humidity data using the DHT11 sensor.  
   - The readings are published to the `esp32/humidity` MQTT topic.  

3. **Data Visualization on Node-RED**:  
   - Node-RED subscribes to the `esp32/temperature` and `esp32/humidity` topics to receive sensor data.  
   - The data is displayed in real-time on a Node-RED dashboard, including charts for temperature and humidity trends.  

4. **Remote LED Control**:  
   - Node-RED sends control signals to the ESP32 via the `esp32/led` MQTT topic.  
   - The ESP32 turns the LED on or off based on the received control signals, enabling remote device management.

---

## System Components

### Hardware:
- **ESP32 Microcontroller**: Handles communication, sensor readings, and LED control.  
- **DHT11 Sensor**: Measures temperature and humidity.  
- **LED**: Acts as an output device controlled by the ESP32.  
- **MQTT Broker**: Facilitates communication between the ESP32 and Node-RED (e.g., Mosquitto).

### Software:
- **Node-RED**:  
  - A low-code programming platform for IoT visualizations and controls.  
  - Used to create a dashboard for displaying real-time data and controlling the LED.  
- **MQTT Protocol**:  
  - Enables efficient communication between the ESP32 and Node-RED.

---

## How It Works

1. The ESP32 reads temperature and humidity values from the DHT11 sensor.  
2. The ESP32 publishes the data to the respective MQTT topics (`esp32/temperature` and `esp32/humidity`).  
3. Node-RED, subscribed to these topics, retrieves the sensor data and updates the charts on the dashboard.  
4. Users can control the LED through Node-RED by sending commands via the `esp32/led` topic.  
5. The ESP32 processes the received commands and toggles the LED accordingly.

---

## Node-RED Dashboard

The Node-RED dashboard provides the following functionality:
- **Real-Time Data Charts**:  
  - Temperature and humidity data are displayed in line charts for easy monitoring.
- **LED Control Interface**:  
  - A simple control switch/button is available to toggle the LED on or off remotely.

---

## Use Cases

This project can be adapted for various IoT applications, including:
- Home automation for monitoring and controlling environmental parameters.  
- Industrial IoT systems for remote monitoring and control.  
- Educational projects for learning about IoT protocols and microcontroller programming.

---

## Prerequisites

Before starting, ensure you have the following:
- **Hardware**: ESP32, DHT11 sensor, LED, resistors, jumper wires, and a breadboard.  
- **Software**: Arduino IDE (with ESP32 board support), Node-RED installed, and an MQTT broker (e.g., Mosquitto).  
- **Dependencies**: MQTT libraries for the ESP32 (e.g., PubSubClient).

---

## Installation and Setup

1. **Prepare the Hardware**:  
   - Connect the DHT11 sensor and LED to the ESP32.  
   - Ensure proper wiring with a breadboard and jumper wires.

2. **Configure Node-RED**:  
   - Install Node-RED and set up the necessary MQTT nodes.  
   - Create a dashboard with charts for temperature and humidity and a control switch for the LED.

3. **Set Up the MQTT Broker**:  
   - Install and configure a broker (e.g., Mosquitto) to handle the communication.

4. **Program the ESP32**:  
   - Flash the ESP32 with the appropriate firmware to read sensor data, publish MQTT messages, and control the LED.

5. **Deploy and Test**:  
   - Start Node-RED and verify data visualization on the dashboard.  
   - Test LED control via Node-RED.

---

## Troubleshooting

- **No Data on Node-RED Dashboard**:  
  - Verify the MQTT broker is running and reachable.  
  - Check the MQTT topic names and ensure they match in both ESP32 and Node-RED configurations.  
- **LED Not Responding**:  
  - Ensure the ESP32 receives the control signals.  
  - Check the wiring and the LED’s connection.

---

## Conclusion

This project highlights the integration of IoT technologies using ESP32, DHT11, Node-RED, and MQTT. It provides a foundation for creating robust and scalable IoT systems with real-time data monitoring and remote control capabilities. Explore and expand on this system to suit your specific IoT needs!
