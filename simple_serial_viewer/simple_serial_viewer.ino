#include "BluetoothSerial.h"

// Cek apakah Bluetooth tersedia
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi Bluetooth dengan nama device
  SerialBT.begin("ESP32_SimpleAdmin"); 
  Serial.println("Bluetooth Started! Device name: ESP32_Simple");
  Serial.println("Waiting for connection...");
}

void loop() {
  // Cek apakah ada data dari Bluetooth
  if (SerialBT.available()) {
    char received = SerialBT.read();
    
    // Cetak di Serial Monitor
    Serial.print("Received: ");
    Serial.println(received);
    
    // // Cetak kembali ke Bluetooth (echo)
    // SerialBT.print("Echo: ");
    // SerialBT.println(received);
    
    // Tambahan: Cetak pesan berdasarkan command
    // if (received == 'L') {
    //   Serial.println("Command: Turn Left");
    //   SerialBT.println("Turning Left...");
    // }
    // else if (received == 'R') {
    //   Serial.println("Command: Turn Right");
    //   SerialBT.println("Turning Right...");
    // }
    // else if (received == 'F') {
    //   Serial.println("Command: Forward");
    //   SerialBT.println("Moving Forward...");
    // }
    // else {
    //   Serial.println("Command: Unknown");
    //   SerialBT.println("Unknown command!");
    // }
  }
  
}