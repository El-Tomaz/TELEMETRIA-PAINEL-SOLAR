
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt
 *
 * Copyright (c) 2023 mobizt
 *
 */

// This example shows how to create the spreadsheet, update and read the values.

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif

#include <ESP_Google_Sheet_Client.h>

// For SD/SD_MMC mounting helper
#include <GS_SDHelper.h>

#define WIFI_SSID "espwifi"
#define WIFI_PASSWORD "tomaz123"

// For how to create Service Account and how to use the library, go to https://github.com/mobizt/ESP-Google-Sheet-Client

#define PROJECT_ID "solar-esp-476120"

// Service Account's client email
#define CLIENT_EMAIL "solar-esp@solar-esp-476120.iam.gserviceaccount.com"

// Your email to share access to spreadsheet
#define USER_EMAIL "gabriel@cear.ufpb.br"

// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCrtZu/Bd8eJLej\ngZt6Wmi3lG7/R/LrvrjToJrBZB9ku2pJESeJjM4ECp8TTTydT3LG8onGJWEgdOTk\n6AImUc9wwddCkYuT8mqJOe/D46CDky6zDUa1Q0qPNJ1K9aCZitElnQSh1pSg13XR\neOuq/R0p7oYLmvEIhCJraWIP0jROHyB7WqeYJazEwwFd1h6ThzaEt4Bcn6q31Y/G\n7VuaE4kYEwh8z3n/WUaEqmxyZKFeNxP2o8oh1rdT3SNlngu/X1h0tQAopV2Nwjdu\n+QUwF5RQbrW/ovj6uLJ7zClBfY1qqVH8TpznxTVSvKJA+CFbe65JkO24xXO54tX4\nXx3867qFAgMBAAECggEABLFgnOD2pxHmusKFgtuazirYBl4x4rHYxqBQH0LfnWBs\n6EUE54+J9HzSbUcSLGETpTttB6J7/zsKo9uLOxjHZ12SehVU8zM8wg+rpwor1/GL\nS3JfsUw+3hKYgO6Di00kTn4DECXd9CM0Ky7Kz0z+WzTG0TIUuw00QDf5XOIITKE/\nYZK9NXCl3wVOVZYW/jMO3txTSLIUu13J3FRlrZNWryJZdqbleKOpbmqgEWbNAdsO\nGeXGzbtXY/su7NTLiACc4JgIXGiEgVURDrcl6JzoKjGc10XHrX1QKuUK2tYl3Yl8\niWCmPBkodBGFxBgOpJLqXiZI37yfVAP0y1cYiUXjyQKBgQDcDrr+umAWnA1q9HnJ\nCnGX5BM6GiV1dQsU1Iq2NH+Gq4NdfzcOSLcCfl4SDStRiBkHq6Xqb3MJLRbKq+RT\nRlJVK5cJOxQktoSDFSRi9oAZ3l0dkX8c3sGTNsscd5fJs+T8Gw5Brdmu0/I9AkSU\nhlHqRAvqxsomPH0N96G7zgJjPQKBgQDHwUvxpYtQgHuYU+7K0ju4dzVWPiqCWQxv\nroYBLhBRKlFeBnvh6hUEZ5ftSF1ZYC5cpvBAvd3xNWgqUNDQw2iLfCd0PvHUiTyT\nrHY8SD1ClyWMiLG/nUcouuTxnyQhKRBRcKg5rMox7hF5aprGpgSmODUicV0wmWuV\nJpSf7JOI6QKBgQC9dwlVTE5y6QU8cyVwDMfkyQAs5FogGRfjZWPQoL6zynQeSeuz\nJrQ7N2XzENedKRqz2fvOLwrSebnH4duCP2ef4Rn3XqBnQ6Yv3kIn2qhMnYF6dc0F\npFzyTRjvd6B+j5612O2bav2jYuSL1qoJXuSkWNYR2/oslaPuJKlhs2um2QKBgQDG\nVOfAf+t4/Hi63+siOebN2p58iLDQRvYQFSo8Fa5hyDCNuMZbon1HqEuybr0wtkyR\n9bZpsZqhmbNZAHC0NrCgfZ72zmBx8+jPftbON7OLYGMJy9cgN1ql2BrkxGus+QaY\ntdBxo1wGgyeac/xE7CWFnB01m3U5gbrNlkbPGqfJsQKBgFgjhlTMpEhQhAorfnMD\nqUMtt9dMrhZ9uNxFVoW38dtQlIpC9CURJegjTqEXaxXT5/q5o/8CCuX9JRIbk5uT\n/26JzOyegYzjirQd30Gg9fv8o4qUeqsaqA5l7RejRfOdA5P2dotzoZPEDJ1m2p6i\nj4+n5AvE8sgZP0woQ6Xac4BO\n-----END PRIVATE KEY-----\n";


const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                  "MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n"
                                  "CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n"
                                  "MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n"
                                  "MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n"
                                  "Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n"
                                  "A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n"
                                  "27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n"
                                  "Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n"
                                  "TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n"
                                  "qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n"
                                  "szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n"
                                  "Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n"
                                  "MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n"
                                  "wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n"
                                  "aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n"
                                  "VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n"
                                  "AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n"
                                  "FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n"
                                  "C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n"
                                  "QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n"
                                  "h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n"
                                  "7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n"
                                  "ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n"
                                  "MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n"
                                  "Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n"
                                  "6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n"
                                  "0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n"
                                  "2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n"
                                  "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n"
                                  "-----END CERTIFICATE-----\n";


bool taskComplete = false;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void tokenStatusCallback(TokenInfo info);

void setup()
{

    Serial.begin(115200);
    Serial.println();
    Serial.println();

    GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

#if defined(ESP32) || defined(ESP8266)
    WiFi.setAutoReconnect(true);
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        if (millis() - ms > 10000)
            break;
#endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // In case SD/SD_MMC storage file access, mount the SD/SD_MMC card.
    // SD_Card_Mounting(); // See src/GS_SDHelper.h

    GSheet.setCert(rootCACert); // or GSheet.setCertFile("path/to/certificate/file.pem", esp_google_sheet_file_storage_type_flash /* or esp_google_sheet_file_storage_type_sd */);

    // Set the callback for Google API access token generation status (for debug only)
    GSheet.setTokenCallback(tokenStatusCallback);

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    GSheet.clearAP();
    GSheet.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

    // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
    GSheet.setPrerefreshSeconds(10 * 60);

    // Begin the access token generation for Google API authentication
    GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

    // Or begin with the Service Account JSON file
    // GSheet.begin("path/to/serviceaccount/json/file", esp_google_sheet_file_storage_type_flash /* or esp_google_sheet_file_storage_type_sd */);
}

void loop()
{
    // Call ready() repeatedly in loop for authentication checking and processing
    bool ready = GSheet.ready();

    if (ready && !taskComplete)
    {
        // For basic FirebaseJson usage example, see examples/FirebaseJson/Create_Edit_Parse/Create_Edit_Parse.ino

        FirebaseJson response;
        // Instead of using FirebaseJson for response, you can use String for response to the functions
        // especially in low memory device that deserializing large JSON response may be failed as in ESP8266

        Serial.println("\nCreate spreadsheet...");
        Serial.println("------------------------");

        FirebaseJson spreadsheet;
        spreadsheet.set("properties/title", "Test - Create Update and Read");

        String spreadsheetId, spreadsheetURL;
        bool success = false;

        // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets/create

        /*
        success = GSheet.create(&response /* returned response , &spreadsheet /* spreadsheet object , USER_EMAIL /* your email that this spreadsheet shared to );
        response.toString(Serial, true);
        Serial.println();
        */
        if (true)
        {

            // Get the spreadsheet id from already created file.
            FirebaseJsonData result;
            spreadsheetId = "1NmjGE4v5qciEfXUvMa5JWOf9_-ou0Kc85wLm7AdnTHg";

            // Get the spreadsheet URL.
            result.clear();
            response.get(result, FPSTR("spreadsheetUrl")); // parse or deserialize the JSON response
            if (result.success)
            {
                spreadsheetURL = result.to<const char *>();
                Serial.println("\nThe spreadsheet URL");
                Serial.println(spreadsheetURL);
            }

            // If you assign the spreadsheet id from your own spreadsheet,
            // to update and read, you need to set share access to the Service Account's CLIENT_EMAIL

            Serial.println("\nUpdate spreadsheet values...");
            Serial.println("------------------------------");

            FirebaseJson valueRange;

            valueRange.add("range", "Sheet1!A1:C3");
            valueRange.add("majorDimension", "COLUMNS");
            valueRange.set("values/[0]/[0]", "A1"); // column 1/row 1
            valueRange.set("values/[0]/[1]", "A2"); // column 1/row 2
            valueRange.set("values/[0]/[2]", "A3"); // column 1/row 3
            valueRange.set("values/[1]/[0]", "B1"); // column 2/row 1
            valueRange.set("values/[1]/[1]", "B2"); // column 2/row 2
            valueRange.set("values/[1]/[2]", "B3"); // column 2/row 3
            valueRange.set("values/[2]/[0]", "C1"); // column 3/row 1
            valueRange.set("values/[2]/[1]", "C2"); // column 3/row 2
            valueRange.set("values/[2]/[2]", "C3"); // column 3/row 3

            // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/update

            success = GSheet.values.update(&response /* returned response */, spreadsheetId /* spreadsheet Id to update */, "Sheet1!A1:C3" /* range to update */, &valueRange /* data to update */);
            response.toString(Serial, true);
            Serial.println();


            response.toString(Serial, true);
            Serial.println();

            if (success)
            {

                Serial.println("\nGet spreadsheet values...");
                Serial.println("------------------------------");

                // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/get

                success = GSheet.values.get(&response /* returned response */, spreadsheetId /* spreadsheet Id to read */, "Sheet1!A1:C3" /* range to read */);
                response.toString(Serial, true);
                Serial.println();

                Serial.println("\nGet spreadsheet values...");
                Serial.println("------------------------------");

                success = GSheet.values.get(&response /* returned response */, spreadsheetId /* spreadsheet Id to read */, "Sheet1!G1:I3" /* range to read */);
                if (success)
                    response.toString(Serial, true);
                else
                    Serial.println(GSheet.errorReason());
                Serial.println();

#if defined(ESP32) || defined(ESP8266)
                Serial.println(ESP.getFreeHeap());
#elif defined(PICO_RP2040)
                Serial.println(rp2040.getFreeHeap());
#endif
            }
        }

        taskComplete = true;
    }
}

void tokenStatusCallback(TokenInfo info)
{
    if (info.status == token_status_error)
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
    }
    else
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}