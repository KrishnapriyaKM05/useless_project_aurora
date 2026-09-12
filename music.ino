#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Audio Objects
HardwareSerial mySerial(2); // Pins 16 RX, 17 TX
DFRobotDFPlayerMini myDFPlayer;

// --- ADXL335 HARDWARE CONFIGURATION ---
const int Z_PIN = 32; // Analog input pin connected to ADXL335 Z-axis

// --- STEP TRACKING CONFIGURATION ---
const int STEP_THRESHOLD = 1200;         // Trigger value based on your 1000-3000 spikes
const unsigned long DEBOUNCE_TIME = 260; // Minimum ms between steps to prevent double-counting

unsigned long lastStepTime = 0;
int stepsInLastMinute = 0;
unsigned long cadenceTimer = 0;

// --- USER PROFILE CONFIGURATION ---
const float STRIDE_LENGTH = 0.8; // Your average stride length in meters

// --- MUSIC CONFIGURATION ---
int currentFolder = 0; // Tracks active folder to avoid endlessly restarting songs

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17 to connect to DFPlayer

  // Configure the analog pin for the ADXL335
  pinMode(Z_PIN, INPUT);

  Serial.println("\n=== ADXL335 PACED MUSIC TRACKER ARMED ===");

  Serial.println("Initializing DFPlayer Mini Audio...");
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("❌ ERROR: DFPlayer Mini not responding! Check SD card format and TX/RX wires.");
    while (1) { delay(10); }
  }
  
  myDFPlayer.volume(20); // Initial volume level (0 to 30)
  cadenceTimer = millis();
  
  // Seed the random number generator using an unused analog pin noise
  randomSeed(analogRead(34)); 
  
  Serial.println("✅ System Online! Start moving to trigger your music.");
}

void loop() {
  // Read the raw analog voltage level from the Z-axis (Ankle impact axis)
  int zRaw = analogRead(Z_PIN);
  unsigned long currentTime = millis();

  // Step Detection Logic using your calibrated threshold
  if (zRaw > STEP_THRESHOLD && (currentTime - lastStepTime) > DEBOUNCE_TIME) {
    stepsInLastMinute++;
    lastStepTime = currentTime;
    Serial.print("👟 Foot Strike! Value: ");
    Serial.print(zRaw);
    Serial.print(" | Step Count: ");
    Serial.println(stepsInLastMinute);
  }

  // Evaluate pace every 10 seconds to keep track switches fast and responsive
  if (currentTime - cadenceTimer >= 10000) {
    // Extrapolate the 10-second data window to get Steps Per Minute (SPM)
    float spm = stepsInLastMinute * 6.0; 
    
    // Reset window counters immediately
    stepsInLastMinute = 0;
    cadenceTimer = currentTime;

    if (spm > 40) { // User is actively moving
      // Pace Calculation: Minutes per Kilometer
      float pace = 1000.0 / (spm * STRIDE_LENGTH);
      
      Serial.print("Current Cadence: "); Serial.print(spm); Serial.print(" SPM | ");
      Serial.print("Calculated Pace: "); Serial.print(pace); Serial.println(" min/km");

      int targetFolder = 1; // Default: Folder 01 (Slow / Walking)
      
      if (spm >= 100 && spm < 135) {
        targetFolder = 2; // Folder 02 (Medium / Jogging)
      } else if (spm >= 135) {
        targetFolder = 3; // Folder 03 (Fast / Running)
      }

      // Change songs only when you shift pace categories
      if (targetFolder != currentFolder) {
        currentFolder = targetFolder;
        
        // Pick a random track number between 1 and 5
        int randomTrack = random(1, 6); 
        
        Serial.print("🎵 Pace Shifted! Playing Folder: ");
        Serial.print(currentFolder);
        Serial.print(" -> Random Track: ");
        Serial.println(randomTrack);
        
        // Command syntax: playFolder(folderNumber, trackNumber)
        myDFPlayer.playFolder(currentFolder, randomTrack); 
      }
      
    } else {
      // Pause music when you stand completely still
      Serial.println("🔇 User stationary. Pausing playback.");
      myDFPlayer.pause();
      currentFolder = 0; 
    }
  }
  
  delay(15); // Small delay to let the analog pin stabilize between reads
}
