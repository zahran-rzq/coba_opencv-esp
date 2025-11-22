#include <WiFi.h>
#include <WiFiUdp.h>

// Konfigurasi WiFi
const char* ssid = "Redmi Note 9";        // Ganti dengan nama WiFi Anda
const char* password = "88888888"; // Ganti dengan password WiFi Anda

// Konfigurasi UDP
WiFiUDP udp;
const unsigned int localUdpPort = 4210;  // Port yang sama dengan simple_cv_udp.py
char incomingPacket[255];  // Buffer untuk data yang diterima


void setup() {
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

void loop() {
  // put your main code here, to run repeatedly:
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // Terima packet
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0';  // Null terminate string
    }

    Serial.println(incomingPacket);
    
    char received = incomingPacket[0];  // Ambil karakter pertama

  }
  delay(5); // Small delay untuk stabilitas
  
}
