#ifndef GRID_H
#define GRID_H

const int GRID_ROWS = 20;
const int GRID_COLS = 12;

extern int grid[GRID_ROWS][GRID_COLS];

void clearGrid();
void setCell(int row, int col, int value);
int getCell(int row, int col);
bool isRowFull(int row);
void clearRow(int row);
void shiftRowsDown(int fromRow);

#endif