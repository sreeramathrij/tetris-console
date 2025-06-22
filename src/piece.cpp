#include "piece.h"
#include "shapes.h"
#include "grid.h"
#include <Arduino.h>

ActivePiece currentPiece;

void spawnNewPiece()
{
  currentPiece.type = random(7);
  currentPiece.rotation = 0;
  currentPiece.x = 3;
  currentPiece.y = 0;
}

const int (*getCurrentShape())[4]
{
  return tetrominoShapes[currentPiece.type][currentPiece.rotation];
}

bool canMove(int newX, int newY, int rotation)
{
  const int (*shape)[4] = tetrominoShapes[currentPiece.type][rotation];

  for (int row = 0; row < 4; row++)
  {
    for (int col = 0; col < 4; col++)
    {
      if (shape[row][col])
      {
        int x = newX + col;
        int y = newY + row;

        if (col == 2)
          Serial.println(x, y);

        // Out of bounds?
        if (x < 0 || x >= GRID_COLS || y < 0 || y >= GRID_ROWS)
        {
          return false;
        }

        // Collides with something in the grid?
        if (getCell(y, x))
        {
          return false;
        }
      }
    }
  }

  return true;
}

void lockPieceToGrid()
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

        if (gridY >= 0 && gridY < GRID_ROWS && gridX >= 0 && gridX < GRID_ROWS)
        {
          setCell(gridY, gridX, 1);
        }
      }
    }
  }
}