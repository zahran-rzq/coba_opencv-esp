import serial
import time

# Konfigurasi Bluetooth Serial
BT_PORT = "COM33"  # Ganti dengan port Bluetooth ESP32 Anda
BAUD_RATE = 115200

print("=" * 50)
print("Test Bluetooth Serial - ESP32")
print("=" * 50)

# Koneksi ke Bluetooth
try:
    bt = serial.Serial(BT_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Tunggu koneksi stabil
    print(f"✓ Connected to {BT_PORT} at {BAUD_RATE} baud")
    print(f"✓ Ready to send data!")
    print("=" * 50)
except Exception as e:
    print(f"✗ Error: {e}")
    print(f"✗ Pastikan ESP32 sudah paired dan port benar")
    exit()

print("\nKetik pesan dan tekan Enter untuk mengirim")
print("Ketik 'exit' untuk keluar\n")

try:
    while True:
        # Input dari user
        user_input = input(">> ")
        
        # Keluar jika ketik 'exit'
        if user_input.lower() == 'exit':
            print("Closing connection...")
            break
        
        # Kirim ke ESP32 via Bluetooth
        try:
            bt.write(user_input.encode())
            bt.flush()
            print(f"✓ Sent: {user_input}")
            
            # Tunggu response dari ESP32 (opsional)
            time.sleep(0.1)
            if bt.in_waiting > 0:
                response = bt.read(bt.in_waiting).decode('utf-8', errors='ignore')
                print(f"← ESP32: {response}")
                
        except Exception as e:
            print(f"✗ Send error: {e}")

except KeyboardInterrupt:
    print("\n\nInterrupted by user")

finally:
    # Tutup koneksi
    bt.close()
    print("✓ Bluetooth connection closed")
