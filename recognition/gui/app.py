# gui/app.py
import tkinter as tk
from PIL import Image, ImageDraw
import tensorflow as tf
import os
import sys

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
from utils.preprocess import preprocess_image

MODEL_PATH = "model/digit_model.h5"

# Load model
model = tf.keras.models.load_model(MODEL_PATH)

class DigitApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Digit Recognition AI")
        self.root.geometry("400x450")  # ukuran awal
        self.root.minsize(300, 350)    # ukuran minimum
        self.root.resizable(True, True)  # izinkan maximize & minimize

        # Gunakan grid untuk fleksibilitas ukuran
        self.root.grid_rowconfigure(0, weight=1)
        self.root.grid_rowconfigure(1, weight=0)
        self.root.grid_rowconfigure(2, weight=0)
        self.root.grid_columnconfigure(0, weight=1)
        self.root.grid_columnconfigure(1, weight=1)

        # Canvas untuk menggambar
        self.canvas_size = 200
        self.canvas = tk.Canvas(root, bg="white", cursor="cross")
        self.canvas.grid(row=0, column=0, columnspan=2, sticky="nsew")
        self.canvas.bind("<B1-Motion>", self.paint)

        # Tombol
        tk.Button(root, text="Predict", command=self.predict).grid(row=1, column=0, sticky="nsew", padx=5, pady=5)
        tk.Button(root, text="Clear", command=self.clear).grid(row=1, column=1, sticky="nsew", padx=5, pady=5)

        # Label hasil prediksi
        self.label_result = tk.Label(root, text="Draw a digit (0-9)", font=("Arial", 16))
        self.label_result.grid(row=2, column=0, columnspan=2, pady=10)

        # Gambar PIL untuk menyimpan coretan
        self.image = Image.new("RGB", (self.canvas_size, self.canvas_size), "white")
        self.draw = ImageDraw.Draw(self.image)

        # Perbarui ukuran canvas saat jendela diubah
        self.canvas.bind("<Configure>", self.resize_canvas)

    def paint(self, event):
        r = 8  # radius brush
        x1, y1 = (event.x - r), (event.y - r)
        x2, y2 = (event.x + r), (event.y + r)
        self.canvas.create_oval(x1, y1, x2, y2, fill="black", outline="black")
        self.draw.ellipse([x1, y1, x2, y2], fill="black", outline="black")

    def clear(self):
        self.canvas.delete("all")
        self.draw.rectangle([0, 0, self.canvas_size, self.canvas_size], fill="white")
        self.label_result.config(text="Draw a digit (0-9)")

    def predict(self):
        processed_img = preprocess_image(self.image)
        prediction = model.predict(processed_img)
        digit = prediction.argmax()
        confidence = prediction[0][digit] * 100
        self.label_result.config(text=f"Predicted: {digit} ({confidence:.2f}%)")

    def resize_canvas(self, event):
        # Saat ukuran jendela berubah, kita sesuaikan area gambar
        self.canvas_size = min(event.width, event.height)
        self.image = self.image.resize((self.canvas_size, self.canvas_size))
        self.draw = ImageDraw.Draw(self.image)

if __name__ == "__main__":
    root = tk.Tk()
    app = DigitApp(root)
    root.mainloop()
