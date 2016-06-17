#include <Wire.h>

uint32_t step_ttg = 0; \\time remaining in current step in milliseconds
uint8_t new_step = 1;
uint8_t current_step = 0;
uint8_t array_length = 14;
uint32_t[array_length] default_program = {}; \\boiling steps
uint32_t *current_program = default_program;
uint8_t step_index = 0;
uint32_t fill_time = 120 * 1000;
uint32_t heat_time = 20 * 60 * 1000;
uint32_t drain_time = 120 * 1000;
float max_temp = 140.0;

uint8_t i2c_id = 40;

int heater_r = 2;
int inlet_r = 3;
int outlet_r = 4;
int magnet_r = 5;
int float_s = 6;
int temp_s = 7;
int green_led = 8;

void setup() {
  pinMode(heater_r, OUTPUT);
  pinMode(inlet_r, OUTPUT);
  pinMode(outlet_r, OUTPUT);
  pinMode(magnet_r, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(float_s, INPUT);
  pinMode(temp_s, INPUT);

  for (int i = 0; i < i2c_id; i++) {
    digitalWrite(green_led, HIGH);
    delay(500);
    digitalWrite(green_led, LOW);
  }

  Wire.begin(i2c_id);

  Wire.onReceive(receiveEvent); // register event
  transmit("Hello from ID: " + String(i2c_id))
}

void loop() {
  switch(current_step) {
    case test_1:
      break;
    case test_2:
      break;
    case fill:
      if new_step step_ttg = millis() + fill_time;
      else if step_ttg < millis() {
        current_step = current_program[++step_index];
        transmit("Warning: Fill Timer Hit");
        new_step = 1;
      } else if float_s {
        current_step = current_program[++step_index];
        new_step = 1;
      }
      break;

  }
}

void transmit(char s[]){
  Wire.beginTransmission(1); // transmit to device #8
  Wire.write(s);
  Wire.endTransmission();    // stop transmitting
}
