import cv2
import pytesseract

# Ganti dengan path ke tesseract.exe di sistemmu
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'

# Baca gambar tulisan tangan
image_path = 'tulisan_tangan.jpg'  # Ganti sesuai file kamu
image = cv2.imread(image_path)

# Preprocessing: grayscale + thresholding
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
_, thresh = cv2.threshold(gray, 150, 255, cv2.THRESH_BINARY_INV)

# (Opsional) Tampilkan gambar hasil threshold
# cv2.imshow("Threshold", thresh)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

# Jalankan OCR
custom_config = r'--oem 1 --psm 6'  # OEM 1 untuk LSTM OCR Engine
text = pytesseract.image_to_string(thresh, config=custom_config, lang='eng')

# Tampilkan hasil
print("Teks hasil OCR:")
print(text)
