#include <WiFi.h>
#include <WiFiUdp.h>

// Konfigurasi WiFi
const char* ssid = "ARANIBA";        // Ganti dengan nama WiFi Anda
const char* password = "lpkojihu"; // Ganti dengan password WiFi Anda

// Konfigurasi UDP
WiFiUDP udp;
const unsigned int localUdpPort = 4210;  // Port yang sama dengan simple_cv_udp.py
char incomingPacket[255];  // Buffer untuk data yang diterima

// Pin L298N untuk Motor A (kanan)
const int ENA = 14;  // D14 → ENA (motor A, kanan)
const int IN1 = 27;  // D27 → IN1
const int IN2 = 26;  // D26 → IN2

// Pin L298N untuk Motor B (kiri)
const int IN3 = 25;  // D25 → IN3
const int IN4 = 33;  // D33 → IN4
const int ENB = 32;  // D32 → ENB (motor B, kiri)

int motorA_speed = 0; // Motor kanan
int motorB_speed = 0; // Motor kiri

// Speed untuk setiap command
const int FORWARD_SPEED = 200;
const int TURN_SPEED = 150;

void setup() {
  // Setup pin motor sebagai output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Inisialisasi motor dalam keadaan stop
  stopMotors();
  
  Serial.begin(115200);
  Serial.println("\n=== ESP32 UDP Motor Control ===");
  
  // Koneksi ke WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n✓ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("UDP Port: ");
  Serial.println(localUdpPort);
  
  // Mulai UDP
  udp.begin(localUdpPort);
  Serial.println("✓ UDP Started!");
  Serial.println("Waiting for commands...");
  Serial.println("Commands: L (Left), R (Right), F (Forward), S (Stop)");
  Serial.println("================================");
}

void setMotorA(int speed) {
  motorA_speed = constrain(speed, -255, 255);
  
  if (motorA_speed > 0) {
    // Motor A maju
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, motorA_speed);
  } else if (motorA_speed < 0) {
    // Motor A mundur
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, abs(motorA_speed));
  } else {
    // Motor A stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
}

void setMotorB(int speed) {
  motorB_speed = constrain(speed, -255, 255);
  
  if (motorB_speed > 0) {
    // Motor B maju
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, motorB_speed);
  } else if (motorB_speed < 0) {
    // Motor B mundur
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, abs(motorB_speed));
  } else {
    // Motor B stop
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}

void stopMotors() {
  setMotorA(0);
  setMotorB(0);
}

void moveForward() {
  Serial.println("Command: Forward");
  setMotorA(FORWARD_SPEED);
  setMotorB(FORWARD_SPEED);
}

void turnLeft() {
  Serial.println("Command: Turn Left");
  setMotorA(TURN_SPEED);   // Motor kanan lebih cepat
  setMotorB(TURN_SPEED / 2);       // Motor kiri lebih lambat
}

void turnRight() {
  Serial.println("Command: Turn Right");
  setMotorA(TURN_SPEED / 2);       // Motor kanan lebih lambat
  setMotorB(TURN_SPEED);   // Motor kiri lebih cepat
}

void loop() {
  // Cek apakah ada packet UDP masuk
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // Terima packet
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0';  // Null terminate string
    }
    
    // Cetak info packet
    // Serial.print("Received UDP from ");
    // Serial.print(udp.remoteIP());
    // Serial.print(":");
    // Serial.print(udp.remotePort());
    // Serial.print(" - ");

    Serial.println(incomingPacket);
    
    // Proses command berdasarkan karakter yang diterima
    char received = incomingPacket[0];  // Ambil karakter pertama
    
    if (received == 'L' || received == 'l') {
      turnLeft();
    }
    else if (received == 'R' || received == 'r') {
      turnRight();
    }
    else if (received == 'F' || received == 'f') {
      moveForward();
    }
    else if (received == 'S' || received == 's') {
      Serial.println("Command: Stop");
      stopMotors();
    }
    else {
      Serial.println("Command: Unknown");
    }
  }
  
  delay(5); // Small delay untuk stabilitas
}
