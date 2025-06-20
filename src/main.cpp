#include <Arduino.h>
#include <U8g2lib.h>

#include "piece.h"
#include "grid.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1);

const int cellWidth = 6;
const int cellHeight = 6;
const int originX = 4;
const int originY = 2;

const int btnLeftPin = 4;
const int btnRightPin = 5;
const int btnRotatePin = 14;

unsigned long lastFallTime = 0;
const int fallInterval = 500;

void drawGrid()
{
  for (int row = 0; row < GRID_ROWS; row++)
  {
    for (int col = 0; col < GRID_COLS; col++)
    {
      if (getCell(row, col))
      {
        int x = originX + col * cellWidth;
        int y = originY + row * cellHeight;
        u8g2.drawBox(x, y, cellWidth, cellHeight);
      }
      else
      {
        int x = originX + col * cellWidth;
        int y = originY + row * cellHeight;
        u8g2.drawFrame(x, y, cellWidth, cellHeight);
      }
    }
  }
}

void drawCurrentPiece()
{
  const int (*shape)[4] = getCurrentShape();

  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      if (shape[row][col])
      {
        int gridX = currentPiece.x + col;
        int gridY = currentPiece.y + row;

        if (gridY >= 0 && gridY < GRID_ROWS && gridX >= 0 && gridX < GRID_COLS)
        {
          int x = originX + gridX * cellWidth - 2;
          int y = originY + gridY * cellHeight - 2;
          u8g2.drawBox(x + 2, y + 2, cellWidth - 2, cellHeight - 2);
        }
      }
    }
  }
}

void setup()
{
  u8g2.begin();
  delay(200);
  Serial.begin(115200);

  randomSeed(analogRead(A0));

  for (int i = 0; i < 5; i++)
  {
    Serial.print("Random: ");
    Serial.println(random(7)); // Expect varied output here
  }

  pinMode(btnLeftPin, INPUT); // external pull-up already present
  pinMode(btnRightPin, INPUT);
  pinMode(btnRotatePin, INPUT);

  clearGrid();
  spawnNewPiece();
}

void loop()
{
  unsigned long currentTime = millis();

  if (currentTime - lastFallTime > fallInterval)
  {
    if (canMove(currentPiece.x, currentPiece.y + 1, currentPiece.rotation))
    {
      currentPiece.y += 1;
    }
    else
    {
      lockPieceToGrid();
      spawnNewPiece();
    }
    lastFallTime = currentTime;
  }

  if (digitalRead(btnLeftPin) == LOW)
  {
    if (canMove(currentPiece.x - 1, currentPiece.y, currentPiece.rotation))
      currentPiece.x -= 1;
  }
  else if (digitalRead(btnRightPin) == LOW)
  {
    if (canMove(currentPiece.x + 1, currentPiece.y, currentPiece.rotation))
      currentPiece.x += 1;
  }
  else if (digitalRead(btnRotatePin) == LOW)
  {
    if (currentPiece.rotation == 3)
    {
      currentPiece.rotation = -1;
    }
    if (canMove(currentPiece.x, currentPiece.y, currentPiece.rotation + 1))
      if (currentPiece.rotation == -1)
        currentPiece.rotation = 3;
      else
        currentPiece.rotation += 1;
  }

  u8g2.clearBuffer();
  drawGrid();
  drawCurrentPiece();
  u8g2.sendBuffer();
}