# ESP32 QRIS Display (Midtrans)

Project ini menampilkan QRIS dari Midtrans menggunakan ESP32 dan TFT Display.

ESP32 akan:
1. Connect WiFi
2. Request API Midtrans
3. Mengambil `qr_string`
4. Generate QR Code
5. Menampilkan QR di TFT display

---

# Hardware

- ESP32
- TFT Display 320x480 (ILI9488 / ILI9341)
- Kabel jumper

---

# Library yang Dibutuhkan

Semua library bisa diinstall melalui **Arduino Library Manager**.

Buka:

Arduino IDE → Sketch → Include Library → Manage Libraries

Lalu cari nama library berikut.

---

# 1️⃣ ArduinoJson

Digunakan untuk parsing response JSON dari Midtrans.

Library Manager Name:
ArduinoJson

Author:
Benoit Blanchon

GitHub:
https://github.com/bblanchon/ArduinoJson

Install dari Library Manager lalu pilih versi terbaru (>=6).

---

# 2️⃣ QRCode

Digunakan untuk generate QR code dari `qr_string`.

Library Manager Name:
QRCode

Author:
Richard Moore

GitHub:
https://github.com/ricmoo/QRCode

Library ini sangat ringan dan cocok untuk ESP32.

---

# 3️⃣ TFT_eSPI

Digunakan untuk menampilkan QR code ke TFT display.

Library Manager Name:
TFT_eSPI

Author:
Bodmer

GitHub:
https://github.com/Bodmer/TFT_eSPI

---

# Konfigurasi TFT_eSPI

Setelah install library ini, perlu konfigurasi driver display.

Edit file:

Arduino/libraries/TFT_eSPI/User_Setup_Select.h

atau

Arduino/libraries/TFT_eSPI/User_Setup.h

Contoh konfigurasi untuk display 320x480:

#define ILI9488_DRIVER
#define TFT_WIDTH 320
#define TFT_HEIGHT 480

Sesuaikan juga pin SPI sesuai wiring ESP32.

Dokumentasi lengkap:
https://github.com/Bodmer/TFT_eSPI#user-content-setup

---

# Cara Mendapatkan Server Key Midtrans

1. Login ke dashboard Midtrans  
https://dashboard.midtrans.com

2. Aktifkan **Sandbox Mode**

3. Masuk ke menu:

Settings → Access Keys

4. Ambil:

Server Key

Contoh:
SB-Mid-server-xxxxxxxx

---

# Convert Server Key ke Base64

Format Authorization Midtrans:

SERVER_KEY:

Contoh:

SB-Mid-server-xxxxx:

Encode ke Base64.

Contoh command:

echo -n "SB-Mid-server-xxxxx:" | base64

Hasilnya dimasukkan ke kode:

String authHeader = "Basic BASE64_RESULT";

---

# Cara Menjalankan

1. Install semua library
2. Edit konfigurasi WiFi

const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

3. Masukkan Base64 Server Key
4. Upload kode ke ESP32
5. ESP32 akan menampilkan QRIS di layar.

---

# Flow Sistem

ESP32 connect WiFi  
↓  
Create QRIS transaction  
↓  
Midtrans return qr_string  
↓  
ESP32 generate QR  
↓  
User scan QRIS

---

# Catatan Penting

Untuk production **jangan menyimpan Server Key di ESP32**.

Arsitektur yang direkomendasikan:

ESP32  
↓  
Backend API  
↓  
Midtrans  

ESP32 hanya menerima:

qr_string  
amount  
order_id

---

# Lisensi

MIT