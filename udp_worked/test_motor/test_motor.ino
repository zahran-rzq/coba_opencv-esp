// Pin L298N untuk Motor A (kanan)
const int ENA = 14;  // D14 → ENA (motor A, kanan)
const int IN1 = 27;  // D27 → IN1
const int IN2 = 26;  // D26 → IN2

// Pin L298N untuk Motor B (kiri)
const int IN3 = 25;  // D25 → IN3
const int IN4 = 33;  // D33 → IN4
const int ENB = 32;  // D32 → ENB (motor B, kiri)


void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 100);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 255);

}
