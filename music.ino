#include <Wire.h>
#include "DFRobotDFPlayerMini.h"

// ---------- MPU6050 ----------
const int MPU_ADDR = 0x68;
int16_t accX, accY, accZ;
int16_t gyroX, gyroY, gyroZ;

long baseX = 0, baseY = 0, baseZ = 0;

// ---------- DFPlayer ----------
static const uint8_t PIN_MP3_RX = 16;
static const uint8_t PIN_MP3_TX = 17;

HardwareSerial mp3Serial(2);
DFRobotDFPlayerMini player;

// ---------- Thresholds (based on your measured data) ----------
const long LOW_THRESHOLD  = 9000;
const long MED_THRESHOLD  = 30000;

// ---------- Track cycling ----------
const int TRACKS_IN_FOLDER[4] = {0, 3, 3, 1};
int lastTrackPlayed[4] = {0, 0, 0, 0};

// ---------- State tracking ----------
int currentFolder = 0;
int candidateFolder = 0;
unsigned long candidateSince = 0;
const unsigned long STABLE_TIME_MS = 1500;

// ---------- Rolling average window ----------
const int WINDOW_SIZE = 15;
long magBuffer[WINDOW_SIZE];
int bufferIndex = 0;
bool bufferFilled = false;

void readMPU() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  if (Wire.available() == 14) {
    accX  = Wire.read() << 8 | Wire.read();
    accY  = Wire.read() << 8 | Wire.read();
    accZ  = Wire.read() << 8 | Wire.read();
    Wire.read(); Wire.read();
    gyroX = Wire.read() << 8 | Wire.read();
    gyroY = Wire.read() << 8 | Wire.read();
    gyroZ = Wire.read() << 8 | Wire.read();
  }
}

void calibrateMPU() {
  Serial.println("Calibrating... keep the sensor still and flat.");
  long sumX = 0, sumY = 0, sumZ = 0;
  const int samples = 200;

  for (int i = 0; i < samples; i++) {
    readMPU();
    sumX += accX;
    sumY += accY;
    sumZ += accZ;
    delay(5);
  }

  baseX = sumX / samples;
  baseY = sumY / samples;
  baseZ = sumZ / samples;

  Serial.print("Baseline -> X: "); Serial.print(baseX);
  Serial.print(" Y: "); Serial.print(baseY);
  Serial.print(" Z: "); Serial.println(baseZ);
}

long getAveragedMagnitude(long newMagnitude) {
  magBuffer[bufferIndex] = newMagnitude;
  bufferIndex = (bufferIndex + 1) % WINDOW_SIZE;
  if (bufferIndex == 0) bufferFilled = true;

  int count = bufferFilled ? WINDOW_SIZE : bufferIndex;
  long sum = 0;
  for (int i = 0; i < count; i++) {
    sum += magBuffer[i];
  }
  return sum / count;
}

void playNextTrackInFolder(int folder) {
  int totalTracks = TRACKS_IN_FOLDER[folder];

  lastTrackPlayed[folder]++;
  if (lastTrackPlayed[folder] > totalTracks) {
    lastTrackPlayed[folder] = 1;
  }

  Serial.print("Playing folder ");
  Serial.print(folder);
  Serial.print(" track ");
  Serial.println(lastTrackPlayed[folder]);

  player.playFolder(folder, lastTrackPlayed[folder]);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);
  Wire.setClock(100000);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  byte mpuError = Wire.endTransmission(true);

  if (mpuError == 0) {
    Serial.println("MPU6050 initialized.");
  } else {
    Serial.println("MPU6050 init failed!");
  }

  calibrateMPU();

  mp3Serial.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
  if (player.begin(mp3Serial)) {
    Serial.println("DFPlayer online.");
    player.volume(25);
  } else {
    Serial.println("DFPlayer failed to initialize!");
  }

  for (int i = 0; i < WINDOW_SIZE; i++) magBuffer[i] = 0;
}

void loop() {
  readMPU();

  long deltaX = abs(accX - baseX);
  long deltaY = abs(accY - baseY);
  long deltaZ = abs(accZ - baseZ);
  long rawMagnitude = deltaX + deltaY + deltaZ;

  long avgMagnitude = getAveragedMagnitude(rawMagnitude);

  Serial.print("Raw: "); Serial.print(rawMagnitude);
  Serial.print(" Avg: "); Serial.println(avgMagnitude);

  int targetFolder;
  if (avgMagnitude < LOW_THRESHOLD) {
    targetFolder = 1;
  } else if (avgMagnitude < MED_THRESHOLD) {
    targetFolder = 2;
  } else {
    targetFolder = 3;
  }

  unsigned long now = millis();

  if (targetFolder != candidateFolder) {
    candidateFolder = targetFolder;
    candidateSince = now;
  }

  if (candidateFolder != currentFolder && (now - candidateSince >= STABLE_TIME_MS)) {
    currentFolder = candidateFolder;
    playNextTrackInFolder(currentFolder);
  }

  delay(50);
}
