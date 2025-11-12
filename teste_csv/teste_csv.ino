#include <Arduino.h>
#include <WiFi.h>
#include "esp_sntp.h"
#include "esp_netif_sntp.h"
#include "time.h"

#include <freertos/task.h>
#include <freertos/queue.h>

#include <ESP_Google_Sheet_Client.h>

#include "max6675.h"
// parametros de wifi e api do google planilhas
#include "config.h"

// hardware mapping
#define tpDO 19
#define tpCLK 5

#define tpCS1 18

#define tpNUMBER 1  //number of thermocouples used

MAX6675 thermocouples[tpNUMBER] = { MAX6675(tpCLK, tpCS1, tpDO) };

typedef struct {
  tm time;
  float temp[tpNUMBER];
} data;

QueueHandle_t dados = NULL;

void tokenStatusCallback(TokenInfo info);

void read_temp(void *pv) {
  data item;
  time_t now = time(NULL);
  struct tm timeinfo;

  while (true) {
    time(&now);
    localtime_r(&now, &timeinfo);

    item.time = timeinfo;

    for (int i = 0; i < tpNUMBER; i++) {
      item.temp[i] = thermocouples[i].readCelsius();
    }

    if (xQueueSend(dados, (void *)&item, pdMS_TO_TICKS(100)) != pdPASS) {
      Serial.println("read_temp> erro: Falha ao colocar item na Queue");
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void send_data(void *pv) {
  time_t now;
  bool ready = false;

  FirebaseJson valueRange;
  FirebaseJson response;

  while (true) {
    ready = GSheet.ready();
    data item;
    char horario[9];
    if (ready) {

      if (xQueueReceive(dados, (void *)&item, pdMS_TO_TICKS(100)) != pdPASS) {
        Serial.println("Falha ao receber dados!");
      } else {
        tm timeinfo = item.time;
        Serial.println("Gsheet ready!");
        snprintf(horario, 9, "%d:%d:%d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        Serial.println(horario);
        valueRange.add("majorDimension", "COLUMNS");
        valueRange.set("values/[0]/[0]", horario);

        for (int i = 0; i < tpNUMBER; i++) {
          char key[16] = "";
          snprintf(key, 16, "values/[%d]/[0]", i + 1);
          valueRange.set(key, String(item.temp[i]));
        }

        bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to update */, "Sheet1!A1" /* range to update */, &valueRange /* data to update */);

        response.toString(Serial, true);
        Serial.println();

        if (success) {
          valueRange.clear();
          response.clear();
        }
      }
    }
  }
}

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

  WiFi.setAutoReconnect(true);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  GSheet.setCert(rootCACert);  // or GSheet.setCertFile("path/to/certificate/file.pem", esp_google_sheet_file_storage_type_flash /* or esp_google_sheet_file_storage_type_sd */);

  // Set the callback for Google API access token generation status (for debug only)
  GSheet.setTokenCallback(tokenStatusCallback);

  // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
  GSheet.setPrerefreshSeconds(10 * 60);

  // Begin the access token generation for Google API authentication
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);


  Serial.print("Spreedsheet id: ");
  Serial.println(spreadsheetId);

  // configurando timezone
  // a priori, as bibliotecas utilizadas aqui ja sincronizam o relogio com sntp

  setenv("TZ", "<-03>3", 1);  // horario de fortaleza
  Serial.println("Time zone: America/Fortaleza");

  Serial.println("Lauching tasks ...");

  dados = xQueueCreate(10, sizeof(data));

  xTaskCreate(send_data, "send_data", 40096, NULL, 1, NULL);
  xTaskCreate(read_temp, "read_temp", 4096, NULL, 2, NULL);
}

void loop() {

  // Call ready() repeatedly in loop for authentication checking and processing
  vTaskDelay(100);
}

void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}