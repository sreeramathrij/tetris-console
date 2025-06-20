#include <Arduino.h>
#include <unity.h>
#include "grid.h"

void test_grid_set_get()
{
  clearGrid();
  setCell(1, 2, 1);
  TEST_ASSERT_EQUAL(1, getCell(1, 2));
}

void setup()
{
  UNITY_BEGIN();
  RUN_TEST(test_grid_set_get);
  UNITY_END();
}

void loop() {}