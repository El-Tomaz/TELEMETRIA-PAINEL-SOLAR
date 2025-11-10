#define WIFI_SSID "espwifi"
#define WIFI_PASSWORD "tomaz123"

#define ntpServer "pool.ntp.org"
#define spreadsheetId "1NmjGE4v5qciEfXUvMa5JWOf9_-ou0Kc85wLm7AdnTHg"
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
