import cv2
import os
import time
video_path = "namafile.mp4"

def play_video_in_terminal(video_path, scale=0.1, threshold=127, fps=10):
    cap = cv2.VideoCapture(video_path)

    if not cap.isOpened():
        print("Gagal membuka video.")
        return

    # Loop untuk membaca dan menampilkan frame
    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                break  # Video selesai

            # Ubah ke grayscale
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            # Resize agar sesuai terminal
            height, width = gray.shape
            new_width = int(width * scale)
            new_height = int(height * scale * 0.5)  # rasio aspek terminal
            resized = cv2.resize(gray, (new_width, new_height))

            # Konversi ke hitam-putih
            _, binary = cv2.threshold(resized, threshold, 255, cv2.THRESH_BINARY)

            # Bersihkan terminal
            os.system('cls' if os.name == 'nt' else 'clear')

            # Tampilkan karakter
            for row in binary:
                line = ''.join(['#' if pixel == 0 else ' ' for pixel in row])
                print(line)

            # Delay antar frame
            time.sleep(1 / fps)
    except KeyboardInterrupt:
        print("\nDihentikan oleh pengguna.")
    finally:
        cap.release()

# Ganti path video di bawah ini dengan file yang kamu punya
video_path = "video.mp4"
play_video_in_terminal(video_path)
