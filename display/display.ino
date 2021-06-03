// SPDX-FileCopyrightText: 2020-2021, Alexander Thulcke
// SPDX-License-Identifier: MIT

// Hardware:
// Adafruit Feather M0 + Neopixel FeatherWing

// Board Manager:
// Addional Board Manager URL: https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
// Arduino SAMD Boards
// Adafruit SAMD Boards

// Libraries:
// Adafruit DMA neopixel library
// Adafruit Neopixel
// SerialCommands

#include <SerialCommands.h>

#include <bittable.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>

char cmd_buffer[32];
SerialCommands serial_cmd(&Serial, cmd_buffer, sizeof(cmd_buffer), "\n", " ");

#define PIN        6
#define NUM_PIXELS 32

Adafruit_NeoPixel_ZeroDMA strip(NUM_PIXELS, PIN, NEO_GRB);

bool getArg(SerialCommands* sender, int& value, int valuemin, int valuemax, const char* description = nullptr)
{
  const char* str = sender->Next();
  if(str == nullptr || *str == '\0') {
    sender->GetSerial()->print("ERROR: missing argument");
    if(description != nullptr) {
      sender->GetSerial()->print(" ");
      sender->GetSerial()->print(description);
    }
    sender->GetSerial()->println();
    return false;
  }

  char* endptr = nullptr;
  value = strtol(str, &endptr, 10);
  if(*endptr != '\0') {
    sender->GetSerial()->print("ERROR: invalid argument");
    if(description != nullptr) {
      sender->GetSerial()->print(" ");
      sender->GetSerial()->print(description);
    }
    sender->GetSerial()->println();
    return false;
  }

  if(value < valuemin || value > valuemax) {
    sender->GetSerial()->print("ERROR: argument ");
    if(description != nullptr) {
      sender->GetSerial()->print(description);
      sender->GetSerial()->print(" ");
    }
    sender->GetSerial()->print("out of range [");
    sender->GetSerial()->print(valuemin);
    sender->GetSerial()->print(",");
    sender->GetSerial()->print(valuemax);
    sender->GetSerial()->println("]");
    return false;
  }

  return true;
}

bool update = true;

void cmd_set_handler(SerialCommands* sender)
{
  int n, r, g, b;
  if(!getArg(sender, n, 0, strip.numPixels() - 1, "N")) {
    return;
  }

  if(!getArg(sender, r, 0, 255, "R")) {
    return;
  }
  if(!getArg(sender, g, 0, 255, "G")) {
    return;
  }
  if(!getArg(sender, b, 0, 255, "B")) {
    return;
  }

  strip.setPixelColor(n, r, g, b);
  if(update) {
    strip.show();
  }

  sender->GetSerial()->println("OK");
}

SerialCommand cmd_set("set", cmd_set_handler);

void cmd_show_handler(SerialCommands* sender)
{
  strip.show();
  sender->GetSerial()->println("OK");
}

SerialCommand cmd_show("show", cmd_show_handler);

void cmd_update_handler(SerialCommands* sender)
{
  int value;
  if(getArg(sender, value, 0, 1)) {
    update = value != 0;
    if(update) {
      strip.show();
    }
    sender->GetSerial()->println("OK");
  }
}

SerialCommand cmd_update("update", cmd_update_handler);

void cmd_help_handler(SerialCommands* sender)
{
  sender->GetSerial()->println("set N R G B   sets pixel at position N to RGB");
  sender->GetSerial()->println("show          forces a refresh");
  sender->GetSerial()->println("update 0/1    disables/enables automatic update after set");
}

SerialCommand cmd_help("help", cmd_help_handler);

void cmd_unknown(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("ERROR: Unknown command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}

void setup() {
  serial_cmd.AddCommand(&cmd_set);
  serial_cmd.AddCommand(&cmd_show);
  serial_cmd.AddCommand(&cmd_update);
  serial_cmd.AddCommand(&cmd_help);
  serial_cmd.SetDefaultHandler(&cmd_unknown);

  strip.begin();
  //strip.setBrightness(32);
  strip.clear();
  strip.show();

  for(uint32_t c = 0x100000; c; c >>= 8) {
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(10);
    }
  }

  strip.clear();
  strip.show();

  Serial.println("READY");
}

void loop() {
  serial_cmd.ReadSerial();
}
