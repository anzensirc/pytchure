import cv2
import os
import time
import shutil

def play_video_in_terminal(video_path, threshold=127, fps=15):
    cap = cv2.VideoCapture(video_path)

    if not cap.isOpened():
        print("Gagal membuka video.")
        return

    # Ambil ukuran terminal
    terminal_size = shutil.get_terminal_size((80, 24))
    term_width = terminal_size.columns
    term_height = terminal_size.lines - 2  # sisakan 2 baris

    # Loop untuk membaca dan menampilkan frame
    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                break  # Video selesai

            # Ubah ke grayscale
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            # Hitung rasio scaling berdasarkan ukuran terminal
            frame_height, frame_width = gray.shape
            aspect_ratio = frame_height / frame_width

            target_width = term_width
            target_height = int(aspect_ratio * target_width * 0.55)

            if target_height > term_height:
                target_height = term_height
                target_width = int(target_height / (aspect_ratio * 0.55))

            # Resize frame
            resized = cv2.resize(gray, (target_width, target_height))

            # Konversi ke hitam-putih
            _, binary = cv2.threshold(resized, threshold, 255, cv2.THRESH_BINARY)

            # Bersihkan terminal
            os.system('cls' if os.name == 'nt' else 'clear')

            # Tampilkan karakter ke terminal
            for row in binary:
                line = ''.join(['█' if pixel == 0 else ' ' for pixel in row])
                print(line)

            # Delay antar frame
            time.sleep(1 / fps)

    except KeyboardInterrupt:
        print("\nDihentikan oleh pengguna.")
    finally:
        cap.release()

# Ganti dengan nama file video kamu
video_path = "video.mp4"
play_video_in_terminal(video_path)
