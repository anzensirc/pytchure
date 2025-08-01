import cv2
import os
import time
import shutil

# Semakin kiri = gelap, kanan = terang
ASCII_CHARS = "@%#*+=-:. "

def map_pixel_to_ascii(pixel_value):
    """Map pixel value (0–255) ke karakter ASCII"""
    index = int((pixel_value / 255) * (len(ASCII_CHARS) - 1))
    return ASCII_CHARS[index]

def play_video_in_terminal_ascii(video_path, fps=15):
    cap = cv2.VideoCapture(video_path)

    if not cap.isOpened():
        print("Gagal membuka video.")
        return

    # Ukuran terminal
    term_size = shutil.get_terminal_size((80, 24))
    term_width = term_size.columns
    term_height = term_size.lines - 2  # sisakan baris bawah

    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                break

            # Konversi ke grayscale
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            # Hitung rasio dan scaling
            h, w = gray.shape
            aspect_ratio = h / w
            target_width = term_width
            target_height = int(aspect_ratio * target_width * 0.55)

            if target_height > term_height:
                target_height = term_height
                target_width = int(target_height / (aspect_ratio * 0.55))

            resized = cv2.resize(gray, (target_width, target_height))

            # Bersihkan terminal
            os.system("cls" if os.name == "nt" else "clear")

            # Tampilkan tiap baris sebagai ASCII
            for row in resized:
                line = ''.join(map_pixel_to_ascii(pixel) for pixel in row)
                print(line)

            time.sleep(1 / fps)
    except KeyboardInterrupt:
        print("\nDihentikan oleh pengguna.")
    finally:
        cap.release()

# Ganti path sesuai video kamu
video_path = "video.mp4"
play_video_in_terminal_ascii(video_path)
