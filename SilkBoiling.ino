#include <Wire.h>

uint32_t step_ttg = 0; \\time remaining in current step in milliseconds
uint32_t toggle_time = 0;
uint8_t new_step = 1;
uint8_t current_step = 0;
uint8_t array_length = 14;
uint8_t[array_length] default_program =
  {fill, heat, boil, drain, fill, rinse, drain, fill, rinse, drain, fill, rinse, drain, dry, done};
  \\boiling steps
uint8_t *current_program = default_program;
uint8_t step_index = 0;
uint32_t fill_time = 120 * 1000;
uint32_t heat_time = 20 * 60 * 1000;
uint32_t drain_time = 120 * 1000;
float max_temp = 140.0;
uint8_t[2] temp_program = {done, done};
uint8_t i2c_id = 40;

int heater_r = 2;
int inlet_r = 3;
int outlet_r = 4;
int magnet_r = 5;
int float_s = 6;
int temp_s = 7;
int green_led = 8;

enum states {test1, test2, fill, heat, boil, drain, rinse, dry, done}

void setup() {
  pinMode(heater_r, OUTPUT);
  pinMode(inlet_r, OUTPUT);
  pinMode(outlet_r, OUTPUT);
  pinMode(magnet_r, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(float_s, INPUT);
  pinMode(temp_s, INPUT);
  pinMode(button, INPUT);


  allPinsLow();

  attachInterrupt(button, buttonPressed, FALLING);

  for (int i = 0; i < i2c_id; i++) {
    digitalWrite(green_led, HIGH);
    delay(500);
    digitalWrite(green_led, LOW);
  }

  Wire.begin(i2c_id);
  Serial.begin(9600);

  Wire.onReceive(receiveEvent); // register event
  transmit("Hello from ID: " + String(i2c_id))
  current_program = default_program;
}

void loop() {
  switch(current_step) {
    case test_1:
      if new_step {
        step_ttg = millis + 30000;
        new_step = 0;
      }
      if (millis() > step_ttg) {
        current_step = off;
        digitalWrite(inlet_r, LOW);
        digitalWrite(outlet_r, LOW);
      }
      transmit("Starting Test 1");
      if (millis() > toggle_time + 5000){
        digitalWrite(inlet_r, !digitalRead(inlet_r));
        digitalWrite(outlet_r, !digitalRead(outlet_r));
        toggle_time = millis();
      }
      break;
    case test_2:
      if new_step {
        step_ttg = millis + 30000;
        digitalWrite(heater_r, HIGH);
        new_step = 0;
      }
      if (millis() > step_ttg) {
        current_step = off;
        digitalWrite(heater_r, LOW);
      }
      transmit("Starting Test 1");
      break;
    case fill:
      if new_step {
        step_ttg = millis() + fill_time;
        digitalWrite(inlet_r, HIGH);
        new_step = 0;
      }
      else if step_ttg < millis() {
        current_step = current_program[++step_index];
        digitalWrite(inlet_r, LOW);
        transmit("Warning: Fill Timer Hit");
        new_step = 1;
      } else if float_s {
        digitalWrite(inlet_r, LOW);
        current_step = current_program[++step_index];
        new_step = 1;
      }
      break;
    case heat:
      if new_step {
        step_ttg = millis() + heat_time;
        digitalWrite(heater_r, HIGH);
        new_step = 0;
      }
      else if step_ttg < millis() {
        current_step = stop;
        digitalWrite(heater_r, LOW);
        transmit("ERROR: Heat Timer Hit");
        new_step = 1;
      } else if temp_s > 96 {
        current_step = current_program[++step_index];
        new_step = 1;
      }
      break;
    case boil:
      if new_step {
        step_ttg = millis() + boil_time;
        digitalWrite(heater_r, HIGH);
        digitalWrite(magnet_r, HIGH);
        new_step = 0;
      }
      else if step_ttg < millis() {
        current_step = current_program[++step_index];
        digitalWrite(heater_r, LOW);
        transmit("Finished Boiling");
        new_step = 1;
      } else {
        if (millis() > toggle_time + 20000){
          digitalWrite(magnet_r, !digitalRead(magnet_r));
          toggle_time = millis();
        }
      }
      break;
    case drain:
      if new_step {
        step_ttg = millis() + drain_time;
        digitalWrite(outlet_r, HIGH);
        digitalWrite(magnet_r, HIGH);
        new_step = 0;
      }
      else if step_ttg < millis() {
        current_step = current_program[++step_index];
        digitalWrite(outlet_r, LOW);
        transmit("Finished Drain");
        new_step = 1;
        current_step = current_program[++step_index];
      } else {
        if (millis() > toggle_time + 10000){
          digitalWrite(magnet_r, !digitalRead(magnet_r));
          toggle_time = millis();
        }
      }
      break;
    case dry:
      if new_step {
        step_ttg = millis() + 60 * 60 * 1000;
        new_step = 0;
      }
      else if step_ttg < millis() {
        current_step = done;
        allPinsLow();
        transmit("Finished Degumming");
        new_step = 1;
      } else {
        if (temp > 50) digitalWrite(heater_r, LOW);
        else if (temp < 30) digitalWrite (heater_r, HIGH);
        if (millis() > toggle_time + 60* 1000){
          digitalWrite(magnet_r, !digitalRead(magnet_r));
          toggle_time = millis();
        }
      }
      break;
    case done:
      new_step = 0;
      break;
  }
}

void transmit(char s[]){
  Wire.beginTransmission(1); // transmit to device #8
  Wire.write(s);
  Wire.endTransmission();    // stop transmitting
  Serial.print(s);
}

void buttonPressed() {
  if (current_step != done) {
    current_step = off;
    allPinsLow();
  } else {
    current_program = default_program;
    step_index = 0;
    current_step = current_program[step_index]
  }
}

void allPinsLow() {
  digitalWrite(heater_r, LOW);
  digitalWrite(inlet_r, LOW);
  digitalWrite(outlet_r, LOW);
  digitalWrite(magnet_r, LOW);
  digitalWrite(green_led, LOW);
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read();
  }
  int x = Wire.read();    // receive byte as an integer
  x = x - '0'; //remove ascii offset
  current_program = temp_program;
  temp_program[0] = x;
  transmit("Starting temporary program");
 }
