#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <QRCode.h>

#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

String qrString =
"00020101021226620014COM.GO-JEK.WWW011993600914377670734580210G7767073450303UKE51440014ID.CO.QRIS.WWW0215AID7211849236160303UKE52043020530336054061000005802ID5918IT Brain Indonesia6006MANADO61059512562395028A120260309161312GQEsZM45QHID0703A0163042B76";

void drawQR(const String &data) {

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(12)];

  qrcode_initText(&qrcode, qrcodeData, 12, ECC_LOW, data.c_str());

  int size = qrcode.size;
  int scale = 3;

  int offsetX = (SCREEN_WIDTH - size * scale) / 2;
  int offsetY = (SCREEN_HEIGHT - size * scale) / 2;

  tft.fillScreen(ILI9341_WHITE);

  for (uint8_t y = 0; y < size; y++) {
    for (uint8_t x = 0; x < size; x++) {

      if (qrcode_getModule(&qrcode, x, y)) {

        tft.fillRect(
          offsetX + x * scale,
          offsetY + y * scale,
          scale,
          scale,
          ILI9341_BLACK
        );

      }

    }
  }
}

void setup() {

  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);

  drawQR(qrString);
}

void loop() {
}