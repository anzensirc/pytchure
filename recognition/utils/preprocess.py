# utils/preprocess.py
import numpy as np
from PIL import Image, ImageOps

def preprocess_image(img: Image.Image):
    """
    Mengubah gambar canvas Tkinter menjadi format input model MNIST.
    """
    # Ubah background menjadi putih dan garis menjadi hitam
    img = img.convert("L")  # grayscale
    img = ImageOps.invert(img)  # hitam jadi putih, putih jadi hitam
    img = img.resize((28, 28))  # ubah ukuran ke 28x28
    img = np.array(img) / 255.0  # normalisasi (0-1)
    img = img.reshape(1, 28, 28, 1)  # (batch, 28,28,1)
    return img
