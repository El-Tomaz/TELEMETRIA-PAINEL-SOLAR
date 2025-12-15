#include "driver/pulse_cnt.h"

#define pAnemometro 13
#define p2Anemometro 32
pcnt_unit_handle_t handle_unit = NULL;
pcnt_channel_handle_t handle_channel = NULL;

int pulse_count = 0;

void setup(){
  pinMode(32,INPUT);
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

  pcnt_channel_set_edge_action(handle_channel, PCNT_CHANNEL_EDGE_ACTION_HOLD,PCNT_CHANNEL_EDGE_ACTION_INCREASE);
  pcnt_channel_set_level_action(handle_channel, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_KEEP);

  pcnt_glitch_filter_config_t config_glitch = {
    .max_glitch_ns = 1000000,
  };
  pcnt_unit_set_glitch_filter(handle_unit, &config_glitch);

  pcnt_unit_enable(handle_unit);
  pcnt_unit_clear_count(handle_unit);
  pcnt_unit_start(handle_unit);

  Serial.begin(115200);
}
int c = 0;
void loop(){
  
  //*
    pcnt_unit_get_count(handle_unit, &pulse_count);
    Serial.println(pulse_count);
  //*/

//Serial.println(digitalRead(32));


}


