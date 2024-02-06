#include "number_array.h"
// Define a 2D array of bytes that represents the numbers 0-9
byte numbers[8][50] = {
    {1, 1, 1, 1, 1,    0, 0, 0, 1, 0,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1,    1, 0, 0, 0, 1,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1,    0, 0, 1, 1, 0,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    0, 0, 0, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1,    0, 0, 0, 1, 0,    1, 1, 1, 1, 1,    0, 1, 1, 1, 1,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1,    0, 0, 0, 1, 0,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1,    0, 0, 0, 1, 0,    1, 0, 0, 0, 0,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 0, 1, 0, 0,    1, 0, 0, 0, 1,    0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1,    0, 0, 0, 1, 0,    1, 0, 0, 0, 0,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 0, 1, 0, 0,    1, 0, 0, 0, 1,    0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1,    0, 0, 0, 1, 0,    1, 0, 0, 0, 0,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 0, 0, 0,    1, 0, 0, 0, 1,    0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1,    0, 0, 0, 1, 0,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1,    0, 0, 0, 0, 1,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1,    0, 1, 0, 0, 0,    1, 1, 1, 1, 1,    0, 0, 0, 0, 1},
    
};
// Define a 2D array of bytes that represents the screen
byte screen[8][12] = {
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// Define a function that takes an integer as input and displays the number on the screen
void display_1_num(int num) {
    int width = 5;
    int d_width = 12;
    int start_x = (d_width - width) / 2;
    int screen_x, screen_y;

    screen_x = start_x;
    screen_y = 0;
    for (int y = 0; y < 8; y++) {
      screen_x = start_x;
      for (int x = num * 5; x < (num + 1) * 5; x++) {
        screen[screen_y][screen_x] = numbers[y][x];
        screen_x++;
      }
      screen_y++;
    }
  }

// Define a function that takes two integers as input and displays the numbers on the screen
void display_2_nums(int l_num, int r_num) {
    int screen_x, screen_y;

    // Left number
    screen_x = 0;
    screen_y = 0;
    for (int y = 0; y < 8; y++) {
          screen_x = 0;
          for (int x = l_num * 5; x < (l_num + 1) * 5; x++) {
              screen[screen_y][screen_x] = numbers[y][x];
              screen_x++;
          }
          screen_y++;
      }

      // Right number
      screen_x = 7;
      screen_y = 0;
      for (int y = 0; y < 8; y++) {
        screen_x = 7;
        for (int x = r_num * 5; x < (r_num + 1) * 5; x++) {
          screen[screen_y][screen_x] = numbers[y][x];
          screen_x++;
      }
      screen_y++;
    }
  }

// Define a function that takes an integer as input and returns a pointer to a 2D array of bytes
byte (*get_number(int number))[8][12] {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 12; x++) {
      screen[y][x] = 0;
    }
  }
  
  int length = String(number).length();
  int l_num, r_num;

  if (length == 1) {
      display_1_num(number);
  } else if (length == 2) {
      display_2_nums(int(number/10), int(number%10));
  }

  return &screen;

}

