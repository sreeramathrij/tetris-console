#include <Arduino.h>
#include <U8g2lib.h>

#include "piece.h"
#include "grid.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1);

const int cellWidth = 6;
const int cellHeight = 6;
const int originX = 4;
const int originY = 12;

const int btnLeftPin = 4;
const int btnRightPin = 5;
const int btnRotatePin = 14;

int score = 0;
int scoreFactor = 100;

bool isGameOver = false;

unsigned long lastButtonPress = 0;
const int buttonDelay = 100;

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

void animateRowClear(int row)
{
  for (int col = 0; col < GRID_COLS; col++)
  {
    setCell(row, col, 0);

    u8g2.clearBuffer();
    drawGrid();
    drawCurrentPiece(); // optional, or skip if animation only
    u8g2.sendBuffer();

    delay(20); // adjust speed of animation
  }
}

int clearFullRows()
{
  int rowsCleared = 0;

  for (int row = GRID_ROWS - 1; row >= 0; row--)
  {
    if (isRowFull(row))
    {
      animateRowClear(row); // handle the animation on this row
      rowsCleared++;

      for (int y = row; y > 0; y--)
      {
        for (int x = 0; x < GRID_COLS; x++)
        {
          setCell(y, x, getCell(y - 1, x));
        }
      }

      // Clear top row
      for (int x = 0; x < GRID_COLS; x++)
      {
        setCell(0, x, 0);
      }

      row++; // recheck this row again
    }
  }

  return rowsCleared;
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

  if (!isGameOver)
  {
    if (currentTime - lastFallTime > fallInterval)
    {
      if (canMove(currentPiece.x, currentPiece.y + 1, currentPiece.rotation))
      {
        currentPiece.y += 1;
      }
      else
      {
        lockPieceToGrid();
        int rowsCleared = clearFullRows();
        score += rowsCleared * scoreFactor;
        spawnNewPiece();
        if (!canMove(currentPiece.x, currentPiece.y, currentPiece.rotation))
        {
          isGameOver = true;
        }
      }
      lastFallTime = currentTime;
    }

    if (currentTime - lastButtonPress > buttonDelay)
    {
      if (digitalRead(btnLeftPin) == LOW)
      {
        if (canMove(currentPiece.x - 1, currentPiece.y, currentPiece.rotation))
        {
          currentPiece.x -= 1;
          lastButtonPress = currentTime;
        }
      }
      else if (digitalRead(btnRightPin) == LOW)
      {
        if (canMove(currentPiece.x + 1, currentPiece.y, currentPiece.rotation))
        {
          currentPiece.x += 1;
          lastButtonPress = currentTime;
        }
      }
      else if (digitalRead(btnRotatePin) == LOW)
      {
        int newRotation = (currentPiece.rotation + 1) % 4;
        if (canMove(currentPiece.x, currentPiece.y, newRotation))
        {
          currentPiece.rotation = newRotation;
          lastButtonPress = currentTime;
        }
      }
    }
  }

  u8g2.clearBuffer();

  char buffer[20];
  sprintf(buffer, "SCORE: %d", score);

  u8g2.setFont(u8g2_font_5x7_tr);
  u8g2.drawStr(2, 8, buffer);

  drawGrid();

  if (isGameOver)
  {
    char bufferOver[10];
    sprintf(bufferOver, "%d", score);

    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.clearBuffer();
    u8g2.drawStr(15, 30, "GAME");
    u8g2.drawStr(15, 40, "OVER");
    u8g2.drawStr(10, 55, "SCORE: ");
    u8g2.drawStr(20, 65, bufferOver);
    u8g2.drawStr(12, 80, "PRESS");
    u8g2.drawStr(10, 90, "ROTATE");
    u8g2.drawStr(25, 100, "TO");
    u8g2.drawStr(12, 110, "RESET");
    u8g2.sendBuffer();

    if (digitalRead(btnRotatePin) == LOW)
    {
      delay(200); // basic debounce
      clearGrid();
      isGameOver = false;
      score = 0;
      spawnNewPiece();
    }

    delay(100);
    return; // skip the rest of the loop
  }

  drawCurrentPiece();
  u8g2.sendBuffer();
}