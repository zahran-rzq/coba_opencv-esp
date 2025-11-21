#include "BluetoothSerial.h"

// Cek apakah Bluetooth tersedia
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

// Pin L298N untuk Motor A (kiri)
const int ENA = 14;  // D14 → ENA (motor A, kiri)
const int IN1 = 27;  // D27 → IN1
const int IN2 = 26;  // D26 → IN2

// Pin L298N untuk Motor B (kanan)
const int IN3 = 25;  // D25 → IN3
const int IN4 = 33;  // D33 → IN4
const int ENB = 32;  // D32 → ENB (motor B, kanan)

BluetoothSerial SerialBT;

int motorA_speed = 0; // Motor kiri
int motorB_speed = 0; // Motor kanan

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
  
  // Inisialisasi Bluetooth dengan nama device
  SerialBT.begin("ESP32_SimpleAdmin"); 
  Serial.println("Bluetooth Started! Device name: ESP32_SimpleAdmin");
  Serial.println("Waiting for connection...");
  Serial.println("Commands: L (Left), R (Right), F (Forward)");
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
  setMotorA(TURN_SPEED / 2);   // Motor kiri lebih lambat
  setMotorB(TURN_SPEED);       // Motor kanan lebih cepat
}

void turnRight() {
  Serial.println("Command: Turn Right");
  setMotorA(TURN_SPEED);       // Motor kiri lebih cepat
  setMotorB(TURN_SPEED / 2);   // Motor kanan lebih lambat
}

void loop() {
  // Cek apakah ada data dari Bluetooth
  if (SerialBT.available()) {
    char received = SerialBT.read();
    
    // Cetak di Serial Monitor
    Serial.print("Received: ");
    Serial.println(received);
    
    // Proses command berdasarkan karakter yang diterima
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
