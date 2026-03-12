#include "driver/pulse_cnt.h"
#include "driver/adc.h"
#include "HX711.h"



#define pAnemometro 13
#define pAnemoscopio ADC1_CHANNEL_6  // pino 34 ?

uint8_t dataPin = 26;
uint8_t clockPin = 25;

#define nSamples 1024


pcnt_unit_handle_t handle_unit = NULL;
pcnt_channel_handle_t handle_channel = NULL;


void config_anemometro();
void task_anemometro(void *pv);
void task_anemoscopio(void *pv);
void task_piranometro(void *pv);

SemaphoreHandle_t pira_ok = NULL;
SemaphoreHandle_t cop_ok = NULL;
SemaphoreHandle_t anem_ok = NULL;

typedef enum {
  PIRANOMETRO = 0,
  ANEMOMETRO = 1,
  ANEMOSCOPIO = 2
} estado;

estado state = PIRANOMETRO;

void setup() {

  Serial.begin(115200);
  Serial.printf("Ola, da serial!");
  config_anemometro();




  pira_ok = xSemaphoreCreateBinary();
  cop_ok = xSemaphoreCreateBinary();
  anem_ok = xSemaphoreCreateBinary();


  xSemaphoreGive(pira_ok);  // se nao givar um o sistema trava
  xTaskCreate(task_anemometro, "task_anemometro", 4096, NULL, 1, NULL);
  xTaskCreate(task_anemoscopio, "task_anemoscopio", 4096, NULL, 1, NULL);
  xTaskCreate(task_piranometro, "task_piranometro", 4096, NULL, 1, NULL);
}


void loop() {

  vTaskDelay(pdMS_TO_TICKS(1000));
}

void task_anemometro(void *pv) {

  pcnt_unit_config_t config_unit = {
    .low_limit = -100,
    .high_limit = 4096,
  };




  pcnt_new_unit(&config_unit, &handle_unit);

  pcnt_chan_config_t config_channel = {
    .edge_gpio_num = pAnemometro,
    .level_gpio_num = -1,
    .flags = {
      .virt_level_io_level = 1,
    },
  };



  pcnt_new_channel(handle_unit, &config_channel, &handle_channel);

  pcnt_channel_set_edge_action(handle_channel, PCNT_CHANNEL_EDGE_ACTION_HOLD, PCNT_CHANNEL_EDGE_ACTION_INCREASE);
  pcnt_channel_set_level_action(handle_channel, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_KEEP);

  pcnt_glitch_filter_config_t config_glitch = {
    .max_glitch_ns = 1000,
  };
  pcnt_unit_set_glitch_filter(handle_unit, &config_glitch);

  pcnt_unit_enable(handle_unit);
  pcnt_unit_clear_count(handle_unit);
  pcnt_unit_start(handle_unit);


  Serial.printf("Anemometro configurado!!");

  int pulse_count = 0;
  int pulse_total = 0;
  float v = 0;
  float hz = 0;
  while (1) {

    if (1) {
      ESP_LOGD("RTOS", "Esperando pira\n");
      xSemaphoreTake(pira_ok, pdMS_TO_TICKS(5000));

      ESP_ERROR_CHECK(pcnt_unit_get_count(handle_unit, &pulse_count));
      hz = pulse_count;
      v = hz * hz * (-0.00065) / 4 + 0.6750 * hz + 0.2;
      pulse_total += pulse_count;
      Serial.printf("[anemometro] pulsos: %d \t freq: %f Hz \t v: %f\n", pulse_total, hz, v);
      pcnt_unit_clear_count(handle_unit);

      xSemaphoreGive(anem_ok);
    }

    state = ANEMOSCOPIO;
    //vTaskDelay(pdMS_TO_TICKS(1000));
  }
}



void task_piranometro(void *pv) {

  // configurando hx711
  HX711 scale;

  //  adjust pins if needed


  uint32_t start, stop;
  volatile float f;

  scale.begin(dataPin, clockPin);
  scale.set_gain(HX711_CHANNEL_A_GAIN_64);

  while (true) {


    if (1) {
      ESP_LOGD("RTOS", "Esperando Anemoscopio\n");
      xSemaphoreTake(cop_ok, pdMS_TO_TICKS(5000));
      Serial.printf("Oie");
      f = scale.read() * 3.3 / (1 << 23) / 64 * 1000000 * 1.22;
      float wm = f / 17.47;


      Serial.printf("[piranometro] Tensão (uV): %f \t Irradiância global (W/m²): %f \n", f, wm);

    }
    state = ANEMOMETRO;

    xSemaphoreGive(pira_ok);
    //vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void task_anemoscopio(void *pv) {

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(pAnemoscopio, ADC_ATTEN_DB_11);

  uint raw = 0;
  uint max = 0;

  calib_anemoscop(&max);
  Serial.printf("anemoscopio: valor maximo: %d", max);

  float angulo = 0;
  while (1) {


    ESP_LOGD("RTOS", "Esperando Anemometro\n");

    xSemaphoreTake(anem_ok, pdMS_TO_TICKS(5000));

    raw = adc1_get_raw(pAnemoscopio);
    angulo = raw / (float)max * 360.0;

    Serial.printf("[anemoscopio] raw: %d \t angulo: %f \n\n", raw, angulo);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    ESP_LOGD("ac", "liberando semaforo!");
    xSemaphoreGive(cop_ok);
  }
}

inline void calib_anemoscop(uint *max) {
  ESP_LOGI("calib", "calibrando anemoscopio, gire ele para calibrar");

  uint lmax = 0;

  for (int i = 0; i < 10 * 100; i++) {
    uint adc = adc1_get_raw(pAnemoscopio);

    if (adc > lmax) {
      lmax = adc;
    }
    Serial.print(".");
    vTaskDelay(pdMS_TO_TICKS(10));
  }

  *max = lmax;
}

void config_anemometro() {
}