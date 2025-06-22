#include "grid.h"

int grid[GRID_ROWS][GRID_COLS] = {0};

void clearGrid()
{
  for (int r = 0; r < GRID_ROWS; r++)
  {
    for (int c = 0; c < GRID_COLS; c++)
    {
      grid[r][c] = 0;
    }
  }
}

void setCell(int row, int col, int value)
{
  if (row >= 0 && row < GRID_ROWS && col >= 0 && col < GRID_COLS && (value == 1 || value == 0))
  {
    grid[row][col] = value;
  }
}

int getCell(int row, int col)
{
  if (row >= 0 && row < GRID_ROWS && col >= 0 && col <= GRID_COLS)
  {
    return grid[row][col];
  }

  return -1;
}

bool isRowFull(int row)
{
  for (int col = 0; col < GRID_COLS; col++)
  {
    if (grid[row][col] == 0)
      return false;
  }
  return true;
}

void clearFullRows()
{
  for (int row = GRID_ROWS - 1; row >= 0; row--)
  {
    bool isFull = true;
    for (int col = 0; col < GRID_COLS; col++)
    {
      if (!getCell(row, col))
      {
        isFull = false;
        break;
      }
    }

    if (isFull)
    {
      // Shift all rows above down
      for (int y = row; y > 0; y--)
      {
        for (int x = 0; x < GRID_COLS; x++)
        {
          setCell(y, x, getCell(y - 1, x));
        }
      }

      // Clear the top row
      for (int x = 0; x < GRID_COLS; x++)
      {
        setCell(0, x, 0);
      }

      // Re-check this row since it's now filled by above
      row++;
    }
  }
}

void shiftRowsDown(int fromRow)
{
  for (int row = fromRow; row > 0; row--)
  {
    for (int col = 0; col < GRID_COLS; col++)
    {
      grid[row][col] = grid[row - 1][col];
    }
  }

  for (int col = 0; col < GRID_COLS; col++)
  {
    grid[0][col] = 0;
  }
}