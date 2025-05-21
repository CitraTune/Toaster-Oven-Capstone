#include <LovyanGFX.hpp>
#pragma once
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define SCREEN_MAIN 0
#define SCREEN_SETTINGS 1
#define SCREEN_FONTS 2
#define SCREEN_COOLDOWN 3  

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341 _panel;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;
  // Touch config block (you can remove if not needed)
  // Common FT5x06/FT6206 capacitive touch controller over I2C
  lgfx::Touch_FT5x06 _touch_instance;

public:
  LGFX(void) {
    { // SPI bus config
      auto cfg = _bus_instance.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 20000000;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = 1;

      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;

      cfg.pin_dc = 2;
      _bus_instance.config(cfg);
      _panel.setBus(&_bus_instance);
    }

    { // Panel config
      auto cfg = _panel.config();
      cfg.pin_cs   = 15;
      cfg.pin_rst  = -1;
      cfg.pin_busy = -1;

      cfg.memory_width  = 320;
      cfg.memory_height = 240;
      cfg.panel_width   = 320;
      cfg.panel_height  = 240;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 0;

      cfg.readable = false;
      cfg.invert = true;  // Because you used TFT_INVERSION_ON
      cfg.rgb_order = true;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;



      _panel.config(cfg);
    }



  { // Backlight control (PWM on GPIO 27)
      auto cfg = _light_instance.config();
      cfg.pin_bl = 27;
      cfg.invert = false;
      cfg.freq   = 44100;
      cfg.pwm_channel = 7;
      _light_instance.config(cfg);
      _panel.setLight(&_light_instance);
    }

    { // Touch controller configuration
      auto cfg = _touch_instance.config();
      cfg.pin_int  = 21;   // INT pin number
      cfg.pin_sda  = 33;   // I2C SDA pin number
      cfg.pin_scl  = 32;   // I2C SCL pin number
      cfg.pin_rst  = 25;   // RST pin number (if you have one)
      cfg.i2c_addr = 0x38; // I2C address
      cfg.i2c_port = 0;    // I2C port number
      cfg.freq     = 400000; // I2C freq
      cfg.x_min    = 0;
      cfg.x_max    = 320;
      cfg.y_min    = 0;
      cfg.y_max    = 240;
      _touch_instance.config(cfg);
      _panel.setTouch(&_touch_instance);
    }

    setPanel(&_panel);
  }
};
