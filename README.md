# Hand Control Motor dengan OpenCV dan ESP32

Proyek ini adalah sistem kontrol motor menggunakan gerakan tangan yang dideteksi melalui kamera. Sistem ini menggunakan computer vision (OpenCV dan MediaPipe) untuk mendeteksi posisi tangan, lalu mengirim perintah ke ESP32 via Bluetooth untuk mengontrol motor.

---

## 📋 Daftar Isi

- [Fitur](#-fitur)
- [Cara Kerja](#-cara-kerja)
- [Hardware yang Dibutuhkan](#-hardware-yang-dibutuhkan)
- [Software yang Dibutuhkan](#-software-yang-dibutuhkan)
- [Instalasi](#-instalasi)
- [Cara Menggunakan](#-cara-menggunakan)
- [Penjelasan Kode untuk Pemula](#-penjelasan-kode-untuk-pemula)
- [Troubleshooting](#-troubleshooting)

---

## ✨ Fitur

- 🤚 **Deteksi Tangan Real-time** - Menggunakan MediaPipe untuk tracking tangan
- 📐 **Kontrol Berdasarkan Sudut** - Miringkan tangan untuk belok kiri/kanan
- 📡 **Komunikasi Bluetooth** - Koneksi wireless ke ESP32
- 🎮 **Kontrol Intuitif** - Gerakan tangan natural untuk kontrol motor
- 📊 **Visual Feedback** - Tampilan angle dan arah di layar

---

## 🔧 Cara Kerja

### Konsep Dasar

```
[KAMERA] → [DETEKSI TANGAN] → [HITUNG SUDUT] → [KIRIM PERINTAH] → [ESP32] → [MOTOR]
```

1. **Kamera** menangkap video dari webcam
2. **MediaPipe** mendeteksi 21 titik landmark di tangan Anda
3. **Python** menghitung sudut antara pergelangan tangan dan jari tengah
4. **Bluetooth** mengirim perintah 'L' (kiri), 'R' (kanan), atau 'F' (maju)
5. **ESP32** menerima perintah dan menampilkannya di Serial Monitor

### Visualisasi Sudut

```
      Tangan Miring Kiri          Tangan Lurus           Tangan Miring Kanan
           (< -20°)                 (-20° to 20°)              (> 20°)
              
              |                         |                         |
             /                          |                         \
            /                           |                          \
        Kirim 'L'                   Kirim 'F'                  Kirim 'R'
```

---

## 🛠️ Hardware yang Dibutuhkan

1. **ESP32 Development Board** - Mikrokontroler dengan Bluetooth
2. **Webcam** - Kamera laptop atau USB webcam
3. **Kabel USB** - Untuk upload program ke ESP32
4. **Komputer/Laptop** - Windows/Linux/Mac dengan Python

---

## 💻 Software yang Dibutuhkan

### 1. Python dan Library

```bash
Python 3.8 atau lebih baru
opencv-python
mediapipe
pyserial
```

### 2. Arduino IDE

- Download dari [arduino.cc](https://www.arduino.cc/en/software)
- Install ESP32 Board Support

---

## 📥 Instalasi

### Step 1: Clone Repository

```bash
git clone https://github.com/zahran-rzq/mencoba_opencv-esp.git
cd mencoba_opencv-esp
```

### Step 2: Buat Virtual Environment (Opsional tapi Direkomendasikan)

```bash
# Windows
python -m venv venv
venv\Scripts\activate

# Linux/Mac
python3 -m venv venv
source venv/bin/activate
```

### Step 3: Install Python Dependencies

```bash
pip install opencv-python mediapipe pyserial
```

### Step 4: Upload Program ke ESP32

1. Buka Arduino IDE
2. Buka file `simple_serial_viewer/simple_serial_viewer.ino`
3. Pilih Board: **ESP32 Dev Module**
4. Pilih Port COM yang sesuai
5. Click **Upload**

### Step 5: Pair ESP32 Bluetooth

1. Upload sketch ke ESP32
2. Buka **Bluetooth Settings** di komputer Anda
3. Cari device bernama **"ESP32_Simple"**
4. Pair (PIN biasanya: 1234 atau 0000)

### Step 6: Cari COM Port Bluetooth

**Windows:**
```powershell
Get-WmiObject Win32_SerialPort | Select-Object Name, DeviceID
```

**Linux:**
```bash
ls /dev/rfcomm*
```

**Mac:**
```bash
ls /dev/cu.*
```

### Step 7: Edit Port di Python Script

Buka file `simple_cv.py` dan edit baris ini:

```python
BT_PORT = "COM33"  # Ganti dengan port Bluetooth Anda
```

---

## 🚀 Cara Menggunakan

### 1. Jalankan Program ESP32

- Upload sketch `simple_serial_viewer.ino`
- Buka Serial Monitor (115200 baud)
- Pastikan muncul pesan: `"Bluetooth Started! Device name: ESP32_Simple"`

### 2. Test Koneksi Bluetooth (Opsional)

```bash
python test_btserial.py
```

Ketik pesan dan lihat apakah muncul di Serial Monitor ESP32.

### 3. Jalankan Hand Control

```bash
python simple_cv.py
```

### 4. Kontrol dengan Tangan

- **Posisikan tangan** di depan kamera
- **Tangan lurus** = Forward (F)
- **Miringkan kiri** (< -20°) = Turn Left (L)
- **Miringkan kanan** (> 20°) = Turn Right (R)
- **Tekan ESC** untuk keluar

---

## 📚 Penjelasan Kode untuk Pemula

### File Structure

```
modul_opencv_fix/
│
├── simple_cv.py                  # Program utama Python
├── test_btserial.py              # Test koneksi Bluetooth
├── simple_serial_viewer/
│   └── simple_serial_viewer.ino  # Program ESP32
└── README.md                     # File ini
```

---

## 📖 Penjelasan `simple_cv.py` (Line by Line)

### 1. Import Library

```python
import cv2          # OpenCV untuk kamera dan gambar
import mediapipe as mp  # MediaPipe untuk deteksi tangan
import math         # Untuk kalkulasi sudut
import serial       # Untuk komunikasi Bluetooth
import time         # Untuk delay dan timing
```

**Penjelasan:**
- `cv2` = Library untuk mengakses kamera dan menggambar di layar
- `mediapipe` = AI dari Google untuk deteksi tangan
- `math` = Untuk menghitung sudut menggunakan trigonometri
- `serial` = Untuk komunikasi serial/Bluetooth dengan ESP32
- `time` = Untuk mengatur waktu delay antar pengiriman data

---

### 2. Konfigurasi Bluetooth

```python
BT_PORT = "COM33"       # Port Bluetooth di komputer Anda
BAUD_RATE = 115200      # Kecepatan komunikasi (bit per detik)
```

**Penjelasan:**
- `BT_PORT` = Nama port yang digunakan ESP32 setelah paired
- `BAUD_RATE` = Kecepatan transfer data (harus sama dengan ESP32)

---

### 3. Koneksi Bluetooth

```python
try:
    bt = serial.Serial(BT_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Tunggu 2 detik agar koneksi stabil
    print(f"blutut konek {BT_PORT}")
except Exception as e:
    print(f"blutut eror {e}")
    bt = None
```

**Penjelasan:**
- `try-except` = Mencoba koneksi, jika gagal tampilkan error
- `serial.Serial()` = Membuka koneksi ke port Bluetooth
- `timeout=1` = Tunggu max 1 detik untuk setiap operasi
- `time.sleep(2)` = Delay 2 detik untuk stabilisasi
- `bt = None` = Jika gagal, set bt ke None (tidak terkoneksi)

---

### 4. Inisialisasi MediaPipe

```python
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(
    static_image_mode=False,        # False = video mode
    max_num_hands=1,                # Deteksi max 1 tangan
    min_detection_confidence=0.6,   # Confidence threshold 60%
    min_tracking_confidence=0.6     # Tracking threshold 60%
)
```

**Penjelasan:**
- `static_image_mode=False` = Mode video (bukan gambar diam)
- `max_num_hands=1` = Hanya deteksi 1 tangan (lebih cepat)
- `min_detection_confidence=0.6` = Yakin 60% ada tangan baru deteksi
- `min_tracking_confidence=0.6` = Yakin 60% tracking masih valid

---

### 5. Variabel Global untuk Throttling

```python
last_command = ""       # Command terakhir yang dikirim
last_send_time = 0      # Waktu terakhir kirim data
SEND_DELAY = 0.01       # Delay 10ms antar pengiriman
```

**Penjelasan:**
- Mencegah spam data ke ESP32
- Hanya kirim jika sudah lewat 10ms sejak kirim terakhir
- Menghindari buffer overflow yang menyebabkan crash

---

### 6. Fungsi `send_serial(command)`

```python
def send_serial(command):
    global last_command, last_send_time
    current_time = time.time()
    
    # Kirim hanya jika sudah lewat delay time
    if bt and (current_time - last_send_time >= SEND_DELAY):
        try:
            bt.write(command.encode())  # Konversi string ke bytes
            bt.flush()                  # Pastikan data terkirim
            last_command = command
            last_send_time = current_time
            return True
        except Exception as e:
            print(f"Send error: {e}")
            return False
    return False
```

**Penjelasan Step-by-Step:**

1. **`global`** - Akses variabel di luar fungsi
2. **`current_time = time.time()`** - Ambil waktu sekarang
3. **`if bt and ...`** - Cek apakah Bluetooth terkoneksi DAN sudah lewat delay
4. **`command.encode()`** - Konversi 'L' menjadi bytes untuk dikirim
5. **`bt.flush()`** - Paksa kirim data dari buffer
6. **`return True`** - Berhasil kirim
7. **`return False`** - Gagal/skip kirim

---

### 7. Fungsi `calculate_angle(point1, point2, image, w, h)`

```python
def calculate_angle(point1, point2, image, w, h):
    # Konversi koordinat normalized (0-1) ke pixel
    x1, y1 = int(point1.x * w), int(point1.y * h)
    x2, y2 = int(point2.x * w), int(point2.y * h)

    # Hitung vektor dari point1 ke point2
    dx = point2.x - point1.x  # Delta X
    dy = point2.y - point1.y  # Delta Y

    # Hitung sudut menggunakan arctan2
    angle = math.degrees(math.atan2(dx, -dy))

    # Gambar garis dari point1 ke point2
    cv2.line(image, (x1, y1), (x2, y2), (255, 0, 0), 3)

    return angle
```

**Penjelasan Matematika:**

```
MediaPipe memberikan koordinat normalized (0.0 - 1.0):
- x = 0.5 artinya 50% dari lebar layar
- y = 0.3 artinya 30% dari tinggi layar

Konversi ke pixel:
- x_pixel = 0.5 * 640 = 320 pixel
- y_pixel = 0.3 * 480 = 144 pixel

Hitung sudut:
        point2 (middle_mcp)
           /
          /  
         /   angle
        -------- (horizontal)
       /
      /
  point1 (wrist)

arctan2(dx, -dy) = sudut dalam radian
math.degrees() = konversi radian ke derajat
```

**Contoh:**
- Tangan lurus ke atas = 0°
- Tangan miring kiri = -45°
- Tangan miring kanan = +45°

---

### 8. Fungsi `write_value_on_frame()`

```python
def write_value_on_frame(label, value, x, y, image):    
    cv2.putText(image, f"{label} {value:.1f}", (x, y),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
```

**Penjelasan Parameter:**
- `label` = Teks label (contoh: "angle:")
- `value` = Nilai angka (contoh: 45.6)
- `x, y` = Posisi teks di layar
- `image` = Gambar tempat teks ditampilkan
- `f"{label} {value:.1f}"` = Format string (1 angka desimal)
- `(0, 255, 0)` = Warna hijau (BGR format)
- `2` = Ketebalan teks

---

### 9. Fungsi `main()` - Loop Utama

#### 9.1 Buka Kamera

```python
while cap.isOpened():
    success, image = cap.read()
    if not success:
        print("Ignoring empty camera frame.")
        continue
```

**Penjelasan:**
- `cap.read()` = Ambil 1 frame dari kamera
- `success` = True jika berhasil, False jika gagal
- Jika gagal, skip ke frame berikutnya

---

#### 9.2 Flip dan Konversi Warna

```python
image = cv2.flip(image, 1)  # Mirror horizontal
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)  # BGR → RGB
```

**Penjelasan:**
- `flip(image, 1)` = Balik horizontal (mirror mode)
- `cvtColor()` = Konversi BGR (OpenCV) ke RGB (MediaPipe)

---

#### 9.3 Deteksi Tangan

```python
results = hands.process(image)
```

**Penjelasan:**
- MediaPipe memproses gambar
- Mencari tangan di gambar
- Mengembalikan hasil deteksi (21 landmark points)

---

#### 9.4 Proses Jika Tangan Terdeteksi

```python
if results.multi_hand_landmarks:
    for hand_landmarks in results.multi_hand_landmarks:
        # Ambil landmark
        landmarks = hand_landmarks.landmark
        wrist = landmarks[0]       # Titik 0 = pergelangan tangan
        middle_mcp = landmarks[9]  # Titik 9 = pangkal jari tengah
```

**Penjelasan:**
- `multi_hand_landmarks` = List semua tangan yang terdeteksi
- `landmark[0]` = WRIST (pergelangan tangan)
- `landmark[9]` = MIDDLE_FINGER_MCP (pangkal jari tengah)

**21 Landmark Points MediaPipe:**
```
0: WRIST
1-4: THUMB (ibu jari)
5-8: INDEX_FINGER (telunjuk)
9-12: MIDDLE_FINGER (jari tengah)
13-16: RING_FINGER (jari manis)
17-20: PINKY (kelingking)
```

---

#### 9.5 Hitung Sudut dan Kirim Perintah

```python
angle = calculate_angle(wrist, middle_mcp, image, w, h)
angle = angle + 12  # Offset kalibrasi

if angle < -20:
    cv2.putText(image, "Turn Left", (10, 70), ...)
    if send_serial('L'):
        print("L")
elif angle > 20:
    cv2.putText(image, "Turn Right", (10, 70), ...)
    if send_serial('R'):
        print("R")
else:
    cv2.putText(image, "Forward", (10, 70), ...)
    if send_serial('F'):
        print("F")
```

**Penjelasan Logika:**

```
angle < -20°  →  Tangan miring kiri  →  Kirim 'L'
-20° ≤ angle ≤ 20°  →  Tangan lurus  →  Kirim 'F'
angle > 20°  →  Tangan miring kanan  →  Kirim 'R'
```

**Offset +12:**
- Setiap orang berbeda posisi tangannya
- Offset +12 untuk kalibrasi agar 0° = tangan lurus

---

#### 9.6 Tampilkan Window

```python
cv2.imshow('MediaPipe Hands', image)
if cv2.waitKey(5) & 0xFF == 27:  # 27 = ESC key
    break
```

**Penjelasan:**
- `imshow()` = Tampilkan gambar di window
- `waitKey(5)` = Tunggu 5ms untuk keyboard input
- `27` = Kode ASCII untuk tombol ESC
- Tekan ESC untuk keluar

---

#### 9.7 Cleanup

```python
cap.release()           # Tutup kamera
cv2.destroyAllWindows() # Tutup semua window

if bt:
    bt.close()          # Tutup koneksi Bluetooth
    print("✓ Bluetooth closed")
```

---

## 📖 Penjelasan `simple_serial_viewer.ino`

### 1. Include Library

```cpp
#include "BluetoothSerial.h"
```

**Penjelasan:**
- Library untuk Bluetooth Serial di ESP32

---

### 2. Inisialisasi Bluetooth

```cpp
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);        // Serial Monitor
  SerialBT.begin("ESP32_Simple");  // Bluetooth name
  Serial.println("Bluetooth Started!");
}
```

**Penjelasan:**
- `Serial.begin(115200)` = Buka Serial Monitor dengan baud rate 115200
- `SerialBT.begin("ESP32_Simple")` = Start Bluetooth dengan nama device
- Device akan muncul sebagai "ESP32_Simple" di Bluetooth list

---

### 3. Loop Utama

```cpp
void loop() {
  if (SerialBT.available()) {      // Cek ada data dari Bluetooth
    char received = SerialBT.read();  // Baca 1 karakter
    
    Serial.print("Received: ");
    Serial.println(received);      // Print ke Serial Monitor
  }
}
```

**Penjelasan:**
- `SerialBT.available()` = Cek apakah ada data di buffer
- `SerialBT.read()` = Baca 1 byte/karakter dari buffer
- `Serial.println()` = Tampilkan di Serial Monitor

**Contoh Output Serial Monitor:**
```
Bluetooth Started! Device name: ESP32_Simple
Waiting for connection...
Received: L
Received: F
Received: R
Received: F
Received: L
```

---

## 🎯 Troubleshooting

### 1. Kamera Tidak Terbuka

**Error:** `Ignoring empty camera frame`

**Solusi:**
```python
# Coba ganti index kamera
cap = cv2.VideoCapture(0)  # Coba 0, 1, atau 2
```

---

### 2. Bluetooth Tidak Connect

**Error:** `blutut eror [WinError 2]`

**Solusi:**
1. Pastikan ESP32 sudah paired di Bluetooth Settings
2. Cek COM port dengan PowerShell:
   ```powershell
   Get-WmiObject Win32_SerialPort | Select-Object Name, DeviceID
   ```
3. Edit `BT_PORT` di `simple_cv.py`

---

### 3. Program Crash Saat Deteksi Tangan

**Error:** Program freeze atau crash

**Solusi:**
```python
# Tingkatkan SEND_DELAY
SEND_DELAY = 0.2  # Dari 0.01 ke 0.2 (200ms)
```

---

### 4. Hand Tidak Terdeteksi

**Solusi:**
1. Tambah pencahayaan ruangan
2. Gunakan background polos
3. Turunkan confidence threshold:
   ```python
   hands = mp_hands.Hands(
       min_detection_confidence=0.3,  # Dari 0.6 ke 0.3
       min_tracking_confidence=0.3
   )
   ```

---

### 5. ESP32 Tidak Menerima Data

**Solusi:**
1. Cek Serial Monitor ESP32 (115200 baud)
2. Test dengan `test_btserial.py`
3. Cek koneksi Bluetooth di PC

---

## 📝 Tips untuk Pemula

### 1. Mulai dari Test Bluetooth

Sebelum jalankan program utama, test koneksi dulu:
```bash
python test_btserial.py
```

### 2. Pahami Flow Data

```
Tangan → Kamera → MediaPipe → Python → Bluetooth → ESP32
```

### 3. Eksperimen dengan Threshold

Edit nilai threshold untuk sensitivitas berbeda:
```python
if angle < -30:  # Lebih sensitif (dari -20 ke -30)
    send_serial('L')
```

### 4. Debug dengan Print

Tambahkan print untuk debugging:
```python
angle = calculate_angle(wrist, middle_mcp, image, w, h)
print(f"Angle: {angle:.1f}")  # Lihat nilai angle real-time
```

---

## 🎓 Konsep yang Dipelajari

### 1. Computer Vision
- Image processing dengan OpenCV
- Hand tracking dengan MediaPipe
- Coordinate system dan transformasi

### 2. Trigonometri
- Menghitung sudut dengan `atan2()`
- Konversi radian ke derajat
- Vector mathematics

### 3. Serial Communication
- Bluetooth Serial Protocol
- Buffer management
- Throttling dan anti-spam

### 4. Python Programming
- Exception handling (`try-except`)
- Global variables
- Function parameters
- String formatting

### 5. Arduino/ESP32
- Bluetooth Classic
- Serial communication
- Character reading

---

## 📚 Referensi

- [OpenCV Documentation](https://docs.opencv.org/)
- [MediaPipe Hands](https://google.github.io/mediapipe/solutions/hands.html)
- [PySerial Documentation](https://pyserial.readthedocs.io/)
- [ESP32 Bluetooth](https://randomnerdtutorials.com/esp32-bluetooth-classic-arduino-ide/)

---

## 📞 Support

Jika ada pertanyaan atau masalah:
1. Buka Issue di GitHub
2. Cek Troubleshooting section di atas
3. Test dengan `test_btserial.py` untuk isolasi masalah

---

## 📜 License

MIT License - Bebas digunakan untuk proyek pribadi maupun komersial.

---

**Selamat belajar dan bereksperimen! 🚀**
