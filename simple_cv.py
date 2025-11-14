import cv2
import mediapipe as mp
import math
import serial
import time

# Konfigurasi Bluetooth Serial
BT_PORT = "COM33"  # Ganti dengan port Bluetooth ESP32 Anda
BAUD_RATE = 115200


# Inisialisasi Bluetooth Serial
try:
    bt = serial.Serial(BT_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Tunggu koneksi stabil
    print(f"blutut konek {BT_PORT}")
except Exception as e:
    print(f"blutut eror {e}")
    bt = None


last_command = ""
last_send_time = 0
SEND_DELAY = 0.01  # Kirim setiap 10ms



hand_landmarks = None
# Inisialisasi MediaPipe Hands
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(static_image_mode=False,
                     max_num_hands=1,
                     min_detection_confidence=0.6,
                     min_tracking_confidence=0.6)
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles

cap = cv2.VideoCapture(0)


def send_serial(command):
    """Kirim perintah ke ESP32 via Bluetooth"""
    global last_command, last_send_time
    current_time = time.time()
    
    # Kirim hanya jika sudah lewat delay time
    if bt and (current_time - last_send_time >= SEND_DELAY):
        try:
            bt.write(command.encode())
            bt.flush()
            last_command = command
            last_send_time = current_time
            return True
        except Exception as e:
            print(f"Send error: {e}")
            return False
    return False


def calculate_angle(point1, point2, image, w, h):
    # Konversi koordinat normalized ke pixel
    x1, y1 = int(point1.x * w), int(point1.y * h)
    x2, y2 = int(point2.x * w), int(point2.y * h)

    # Hitung vektor dari point1 ke point2
    dx = point2.x - point1.x
    dy = point2.y - point1.y

    # Hitung sudut dalam derajat (0° = atas/sejajar Y, -180° = kiri, +180° = kanan)
    angle = math.degrees(math.atan2(dx, -dy))

    # Gambar garis dari point1 ke point2
    cv2.line(image, (x1, y1), (x2, y2), (255, 0, 0), 3)

    # Tampilkan sudut pada gambar
    return angle


def write_value_on_frame(label, value, x, y,image):    
    cv2.putText(image, f"{label} {value:.1f} ", (x, y),
    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)


def main():
    while cap.isOpened():
        success, image = cap.read()
        if not success:
            print("Ignoring empty camera frame.")
            continue

        image = cv2.flip(image, 1)  # Mirror image for a selfie-view display

        # Ubah warna gambar dari BGR ke RGB
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

        # Proses gambar untuk mendeteksi tangan
        results = hands.process(image)

        # Ubah kembali warna gambar ke BGR untuk OpenCV
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        # Hitung sudut antara wrist dan middle_mcp
        h, w, _ = image.shape

        # Gambar hasil deteksi tangan
        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    image,
                    hand_landmarks,
                    mp_hands.HAND_CONNECTIONS,
                    mp_drawing_styles.get_default_hand_landmarks_style(),
                    mp_drawing_styles.get_default_hand_connections_style())
                
                # Ambil koordinat landmark
                landmarks = hand_landmarks.landmark
                wrist = landmarks[0]      # WRIST
                middle_mcp = landmarks[9] # MIDDLE_MCP

                # test print data landmarks
                # print(landmarks)

                # fungsi untuk menghitung sudut dari dua koordinat thd sumbu y
                angle = calculate_angle(wrist, middle_mcp, image, w, h)
                angle = angle + 12 
                
                write_value_on_frame(f"angle:" ,angle,10,30,image)
                # print(f"Angle: {angle:.1f} deg")

                if angle < -20:
                    cv2.putText(image, f"Turn Left", (10, 70),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                    if send_serial('L'):
                        print("L")
                elif angle > 20:
                    cv2.putText(image, f"Turn Right", (10, 70),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                    if send_serial('R'):
                        print("R")
                else:
                    cv2.putText(image, f"Forward", (10, 70),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                    if send_serial('F'):
                        print("F")

                    
        # Tampilkan gambar
        cv2.imshow('MediaPipe Hands', image)
        if cv2.waitKey(5) & 0xFF == 27:
            break

    cap.release()
    cv2.destroyAllWindows()
    
    # Tutup Bluetooth
    if bt:
        try:
            bt.close()
            print("✓ Bluetooth closed")
        except:
            pass

if __name__ == "__main__":
    main()  