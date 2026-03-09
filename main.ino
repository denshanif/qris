#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <QRCode.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

// ============================
// WIFI CONFIG
// ============================

const char *ssid = "WIFI_NAME";         // Replace wifi SSID
const char *password = "WIFI_PASSWORD"; // Replace wifi password

// ============================
// MIDTRANS API
// ============================

const char *midtransUrl = "https://api.sandbox.midtrans.com/v2/charge";

// Base64 dari: SERVER_KEY:
String authHeader = "Basic REPLACE_WITH_BASE64_SERVER_KEY";

// ============================
// DRAW QR CODE
// ============================

void drawQR(String data)
{

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(4)];

  qrcode_initText(&qrcode, qrcodeData, 4, ECC_LOW, data.c_str());

  int size = qrcode.size;
  int scale = 6;

  int offsetX = (SCREEN_WIDTH - size * scale) / 2;
  int offsetY = (SCREEN_HEIGHT - size * scale) / 2;

  tft.fillScreen(TFT_WHITE);

  for (uint8_t y = 0; y < size; y++)
  {

    for (uint8_t x = 0; x < size; x++)
    {

      if (qrcode_getModule(&qrcode, x, y))
      {

        tft.fillRect(
            offsetX + x * scale,
            offsetY + y * scale,
            scale,
            scale,
            TFT_BLACK);
      }
    }
  }
}

// ============================
// CREATE QRIS TRANSACTION
// ============================

void createQRIS()
{

  HTTPClient http;

  http.begin(midtransUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", authHeader);

  String payload = R"rawliteral(
{
  "payment_type": "qris",
  "transaction_details": {
    "order_id": "order-test-123",
    "gross_amount": 100000
  },
  "qris": {
    "acquirer": "gopay"
  }
}
)rawliteral";

  int httpCode = http.POST(payload);

  if (httpCode > 0)
  {

    String response = http.getString();
    Serial.println(response);

    DynamicJsonDocument doc(8192);

    DeserializationError error = deserializeJson(doc, response);

    if (error)
    {
      Serial.println("JSON parse failed");
      return;
    }

    String qrString = doc["qr_string"].as<String>();

    if (qrString.length() == 0)
    {
      Serial.println("QR string kosong");
      return;
    }

    Serial.println("QR STRING:");
    Serial.println(qrString);

    drawQR(qrString);
  }
  else
  {

    Serial.print("HTTP Error: ");
    Serial.println(httpCode);
  }

  http.end();
}

// ============================
// SETUP
// ============================

void setup()
{

  Serial.begin(115200);

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_WHITE);

  Serial.println("Connecting WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {

    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  createQRIS();
}

// ============================

void loop()
{
}