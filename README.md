<img width="1280" height="640" alt="git (1)" src="https://github.com/user-attachments/assets/8920b256-2ba8-4988-b824-5351134eb4bd" />



ODANDAMMAVA AALARIYAM


## Basic Details
### Team Name: AURORA


### Team Members
- Team Lead: Nifidha K- NSS COLLEGE OF ENGINEERING,PALAKKAD
- Member 2: Krishnapriya K M - NSS COLLEGE OF ENGINEERING,PALAKKAD


### Project Description
Our project is a music player which measures the pace of us walking and plays music accordingly.

### The Problem (that doesn't exist)
What if we had a real life bgm?

### The Solution (that nobody asked for)
We tried making a pant which has an gyroscope at the ankle which calculates the pace of our walking and is then used to calculate the steps per minute.
If the steps per minute(spm) is below certain threshold then slow songs would be played ,else if its between,then medium paced songs and if its greater than given threshold faster paced songs would play.

## Technical Details
### Technologies/Components Used


For Hardware:
ESP 32,
Gyroscope-MPU6050,
DF player mini,
Step down buck converter,
MicroSD Card: A standard 4Gb or higher to store your songs,
7.4V battery with charging module.

### Implementation


For Hardware:

# Schematic & Circuit
Circuit

[./circuit]

Battery produces 7.4v and is given to charger module which is connected to step down buck converter which steps down the voltage to 5V.
The buck converter is connected to DF mini player which contains the SD card with the songs as well as ESP32.
The gyroscope is then connected to ESP32.
The speaker is connected to DF mini player and ESP32.

Schematic

[./schematic]

The positve of 7.4v battery is connected to Bat of charging module and the negative of the battery is connected to the ground.
From bat the charger module is connected to the IN+ of the buck converter and from GND its connected to the IN- of the charger module.
From the OUT+ of the buck converter 2 connections are taken ,one is connected to V-in of the ESP32 and other towards the 1st pin of DF mini.
Gyroscope is connected from SCL,SDL,VCC,GND pin to GPIO 22 ,21,3.3V and GND.
From GPIO 16(RX) and 17(TX) is connected to pin 3(TX) and 2(RX).
Pins 6 and 8 of DF mini is connected to the speaker.
A common ground is formed between gyroscope,esp32,buck converter and df mini.


# Build Photos
![Components]

[./esp32]

ESP32

[./HW-907ABC]

Charging module

[./gyroscope-MPU6050]

[./7.4v_battery]

[./8ohm_minispeaker]

[./dfplayermini]

[./stepdown_buckconverter]




![Build]

[./build_process1]

Calibrating the gyroscope to predict the correct songs according to the pace.

[./build_process2]

7.4v from battery is stepped down to 5v using buck converter which was calibrated to 5v before.


![Final](Add photo of final product here)
[./completed_circuit]
[./working_circuit]
[./bag]

### Project Demo
# Video

https://drive.google.com/file/d/1KnG8c4DPN9-NSQsFqPzCVsRzqmkEVAgU/view?usp=sharing

https://drive.google.com/file/d/1EFFeL5qsfLNIIDsSMquzKpMts_BXWx4_/view?usp=drive_link

https://drive.google.com/file/d/16OQhUnsHDQhOYjg49GyouTNFMKOTI9ks/view?usp=drive_link

https://drive.google.com/file/d/1B0a1SP03Lwcu_lkJ9sFvAyMfTVeQX4bV/view?usp=sharing

https://drive.google.com/file/d/1CRd2sDmZFHVK0vI9gdsttBNMKs7Dhofv/view?usp=sharing






## Team Contributions
- Nifidha K:Circuit and connections.
- Krishnapriya K M:Dealt with ide


---
Made with ❤️ at TinkerHub Useless Projects 

![Static Badge](https://img.shields.io/badge/TinkerHub-24?color=%23000000&link=https%3A%2F%2Fwww.tinkerhub.org%2F)
![Static Badge](https://img.shields.io/badge/UselessProjects--26-26?link=https%3A%2F%2Ftinkerhub.org%2Fevents%2F1M8ORET9A1%2Fuseless-projects-3.0)



