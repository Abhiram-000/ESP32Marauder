//                            USER DEFINED SETTINGS
//   ESP32-S3-DevKitC-1 + 1.8" ST7735S 160x128 SPI display
//
//   Wiring this file is built for:
//     VCC  -> 3.3V          SCK  -> GPIO 12
//     GND  -> GND           SDA  -> GPIO 11 (MOSI)
//     LED  -> GPIO 4 (PWM)  DC   -> GPIO 9
//     RES  -> GPIO 10       CS   -> GPIO 8
//     MISO -> not connected
//
//   The panel is driven in portrait (128 wide x 160 tall) by the library and
//   rotated to landscape with tft.setRotation(1) by the firmware
//   (SCREEN_ORIENTATION 1 in configs.h) giving 160x128.
//
//   See the User_Setup_Select.h file if you wish to be able to define multiple
//   setups and then easily select which setup file is used by the compiler.


// ##################################################################################
//
// Section 1. Call up the right driver file and any options for it
//
// ##################################################################################

// Only define one driver, the other ones must be commented out
//#define ILI9341_DRIVER // OG Marauder
#define ST7735_DRIVER    // ESP32-S3 DevKitC-1 1.8" ST7735S
//#define ILI9163_DRIVER
//#define ST7789_DRIVER
//#define ST7789_2_DRIVER
//#define ST7796_DRIVER

// Colour order: if red and blue appear swapped on the panel, switch to
// TFT_RGB_ORDER TFT_RGB.
#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

// Pixel width and height in PORTRAIT orientation.
// The ST7735S 1.8" panel has a 132x162 GRAM showing 128x160.
#define TFT_WIDTH  128
#define TFT_HEIGHT 160

// ST7735 tab type. ST7735_GREENTAB applies a column offset of 2 and a row
// offset of 1, which is the standard correction for 128x160 ST7735S panels
// with a 132x162 GRAM. If the image on your particular panel is shifted a few
// pixels or shows garbage on an edge, try in order:
//   ST7735_GREENTAB3  (col offset 2, row offset 3)
//   ST7735_REDTAB     (red-tab init, no extra offset)
//   ST7735_BLACKTAB   (no offset)
#define ST7735_GREENTAB
// #define ST7735_GREENTAB3
// #define ST7735_REDTAB
// #define ST7735_BLACKTAB

// If colours are inverted (white shows as black) uncomment one of the next
// 2 lines try both options, one of the options should correct the inversion.

// #define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF

// The backlight LED on GPIO 4 is ON with the pin HIGH. It is driven with a
// PWM signal by the firmware (HAS_PWM_BACKLIGHT), not by the library.
#define TFT_BACKLIGHT_ON HIGH  // HIGH or LOW are options

// ##################################################################################
//
// Section 2. Define the pins that are used to interface with the display here
//
// ##################################################################################

// ESP32-S3-DevKitC-1 hardware SPI mapping for this build
#define TFT_CS   8  // Chip select control pin
#define TFT_DC   9  // Data Command control pin
#define TFT_RST  10 // Reset pin
#define TFT_BL   4  // LED back-light, PWM dimmable
#define TOUCH_CS -1 // No touch panel
#define TFT_MISO -1 // Not connected on this display
#define TFT_MOSI 11
#define TFT_SCLK 12

// ##################################################################################
//
// Section 3. Define the fonts that are to be used here
//
// ##################################################################################

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// ##################################################################################
//
// Section 4. Other options
//
// ##################################################################################

// 40 MHz as specified for this build. The TFT_eSPI reference notes that some
// ST7735 panels misbehave above ~27 MHz; if you see spurious pixels or
// shifted rows, drop to 27000000.
#define SPI_FREQUENCY   40000000

// Optional reduced SPI frequency for reading TFT (MISO not wired here)
#define SPI_READ_FREQUENCY  20000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY  2500000

// The ESP32-S3 has SPI2/GPSPI2. TFT_eSPI's S3 default derives SPI_PORT from
// FSPI, which evaluates to SPI2_HOST = 1 on core 2.0.x, and REG_SPI_BASE(1)
// returns 0 - every register write then faults on address 0x10 at tft.init().
// USE_FSPI_PORT pins SPI_PORT to 2, which maps to the real GPSPI2 base.
#define USE_FSPI_PORT
