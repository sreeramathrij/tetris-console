#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

const int cellWidth = 5;
const int cellHeight = 3;
const int cols = 10;
const int rows = 20;
const int originX = 4;
const int originY = 2;

void setup()
{
  u8g2.begin();
}

void loop()
{
  u8g2.clearBuffer();

  for (int row = 0; row < rows; row++)
  {
    for (int col = 0; col < cols; col++)
    {
      int x = originX + col * cellWidth;
      int y = originY + row * cellHeight;
      u8g2.drawFrame(x, y, cellWidth, cellHeight);
    }
  }
  // u8g2.setFont(u8g2_font_5x7_mr);
  // u8g2.drawStr(10, 30, "Hello Tetris!");
  u8g2.sendBuffer();
  delay(1000);
}