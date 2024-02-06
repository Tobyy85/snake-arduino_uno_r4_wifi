#ifndef NUMBER_ARRAY_H
#define NUMBER_ARRAY_H

#include <Arduino.h>

extern byte numbers[8][50];

// Declare a function 'get_number' that takes an integer as input and returns a pointer to a 2D array of bytes
byte (*get_number(int number))[8][12];

#endif