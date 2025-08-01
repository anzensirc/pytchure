import cv2
import os
import time
import shutil
import argparse
import numpy as np
import sys
import threading
import sys
import threading

if os.name == "nt":
    import msvcrt
else:
    import termios
    import tty


ASCII_CHARS = "@%#*+=-:. "

def map_pixel_to_ascii(pixel_value):
    index = int((pixel_value / 255) * (len(ASCII_CHARS) - 1))
    return ASCII_CHARS[index]

def color_ansi(gray_val):
    """Buat kode warna ANSI berdasarkan nilai grayscale"""
    return f"\033[38;5;{232 + gray_val * 23 // 255}m"

def get_keypress():
    """Non-blocking keypress cross-platform"""
    if os.name == "nt":
        if msvcrt.kbhit():
            return msvcrt.getch().decode('utf-8').lower()
        return None
    else:
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setcbreak(fd)
            ch = sys.stdin.read(1)
            return ch.lower()
        except Exception:
            return None
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

def play_video_ascii(video_path, fps=15, scale=1.0, loop=False, use_color=False):
    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print("Gagal membuka video.")
        return

    term_size = shutil.get_terminal_size((80, 24))
    term_width = term_size.columns
    term_height = term_size.lines - 2

    paused = False

    def input_thread():
        nonlocal paused
        while True:
            key = get_keypress()
            if key == ' ':
                paused = not paused
            elif key == 'q':
                os._exit(0)

    threading.Thread(target=input_thread, daemon=True).start()

    try:
        while True:
            start_time = time.time()
            if not paused:
                ret, frame = cap.read()
                if not ret:
                    if loop:
                        cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
                        continue
                    else:
                        break

                gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
                h, w = gray.shape
                aspect_ratio = h / w

                target_width = int(term_width * scale)
                target_height = int(aspect_ratio * target_width * 0.55)
                if target_height > term_height:
                    target_height = term_height
                    target_width = int(target_height / (aspect_ratio * 0.55))

                resized = cv2.resize(gray, (target_width, target_height))

                os.system("cls" if os.name == "nt" else "clear")
                print(f"FPS: {fps} | Scale: {scale:.2f} | Press SPACE to pause/resume, Q to quit")

                for row in resized:
                    line = ''
                    for pixel in row:
                        char = map_pixel_to_ascii(pixel)
                        if use_color:
                            line += color_ansi(pixel) + char
                        else:
                            line += char
                    if use_color:
                        line += '\033[0m'  # Reset color
                    print(line)

            elapsed = time.time() - start_time
            time.sleep(max(0, (1 / fps) - elapsed))

    except KeyboardInterrupt:
        print("\nDihentikan oleh pengguna.")
    finally:
        cap.release()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="ASCII Video Player in Terminal")
    parser.add_argument("video_path", help="Path to video file")
    parser.add_argument("--fps", type=int, default=15, help="Frames per second")
    parser.add_argument("--scale", type=float, default=1.0, help="Scaling factor (0.1 - 1.0)")
    parser.add_argument("--loop", action="store_true", help="Loop the video")
    parser.add_argument("--color", action="store_true", help="Enable pseudo-color (ANSI)")
    args = parser.parse_args()

    if not os.path.exists(args.video_path):
        print(f"File video '{args.video_path}' tidak ditemukan.")
        sys.exit(1)

    play_video_ascii(
        args.video_path,
        fps=args.fps,
        scale=args.scale,
        loop=args.loop,
        use_color=args.color
    )
