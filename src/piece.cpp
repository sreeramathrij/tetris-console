#include "piece.h"
#include "shapes.h"

ActivePiece currentPiece;

void spawnNewPiece()
{
  currentPiece.type = random(0, 7);
  currentPiece.rotation = 0;
  currentPiece.x = 3;
  currentPiece.y = 0;
}

const int (*getCurrentShape())[4]
{
  return tetrominoShapes[currentPiece.type][currentPiece.rotation];
}