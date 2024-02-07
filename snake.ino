#include <Arduino_LED_Matrix.h>

#include "JoyStick.h"
#include "number_array.h" // In this file is function to get the bitmap of a number
#include "clear_screen.h"

ArduinoLEDMatrix matrix;

// 507 and 509 are values for the joystick in normal position
// 5 and 5 are the deadzone precent (to ignore small movements of the joystick)
// You can change the SW pin to digital pin
JoyStick joy_stick(A0, A1, A2, 509, 507, 5, 5);


const int screen_width = 12;
const int screen_height = 8;

bool show_menu = true;
bool playing = false;
bool show_end_screen = false;

float difficulty = 5; // 1-9
float multiplier; // 0.02*difficulty + 1
float speed = 3;
float delay_time = 1000/speed; // ms

float current_millis = 0;
float last_millis_end_screen = 0;
float last_millis_food = 0;
float blink_delay = 400;


struct coordinates{
  int x;
  int y;
};

int snake_length = 1;
coordinates snake[96]; // Max length of snake is 96 (8*12)
coordinates food;

String joy_stick_pos = ""; // in which direction the joystick is pointing ("UP", "RIGHT", "DOWN", "LEFT", "CENTER")
String direction = ""; // in which direction the snake is moving ("UP", "RIGHT", "DOWN", "LEFT")
String last_direction = ""; // in which direction the snake was moving last ("UP", "RIGHT", "DOWN", "LEFT")


void setup() {
  Serial.begin(9600);
  matrix.begin();

  randomSeed(analogRead(A4) + analogRead(A5)); // Seed for random food position (I am using 2 analog pins to get a more random seed)

  generate_food(); // Generate the first food
  snake[0].x = screen_width/2; // Start position of the snake is in the middle of the screen
  snake[0].y = screen_height/2; // Start position of the snake is in the middle of the screen

}

void loop() {
  // Here you can call print function to print the values
  // print();
  
  // If the stage of the game is playing (main)
  if (playing){
    delay_time = 1000/speed;  // Update the delay time
    
    for (int _ = 0; _ < 10; _++){ // If this loop wasn't here, the joystick would be checked only once every delay time
      get_direction();
      delay(delay_time/10);
    }
    last_direction = direction;  // To prevent the snake from going back on itself

    move(); // Move the snake
    check_for_collisions(); // Check for collisions

    // If the game is still playing, render the snake and the food on the screen
    if (!show_end_screen){
      render_on_screen(); // Render the snake and the food on the screen
    }
  
   // If the stage of the game is menu
  }else if (show_menu){
    // If the joystick is in the left the difficulty will decrease
    if (difficulty > 1 && joy_stick.get_x_direction() == "LEFT"){
      difficulty--;
      delay(150); // To prevent the difficulty from changing too fast
    // If the joystick is in the right the difficulty will increase
    }else if (difficulty < 9 && joy_stick.get_x_direction() == "RIGHT"){
      difficulty++;
      delay(150); // To prevent the difficulty from changing too fast
    }

    // If the joystick is pressed, the game will start
    if (joy_stick.get_SW()){
      multiplier = float(0.02*difficulty) + 1; // The multiplier is used to increase the speed of the snake based on the difficulty
      speed *= multiplier; // Increase the speed of the snake
      playing = true; // The game is now playing
      show_menu = false; // The menu is now hidden
    }

    // Render the difficulty on the screen
    number_on_matrix(difficulty);
  
  // If the stage of the game is end screen
  }else if (show_end_screen){
    current_millis = millis();
    int score = snake_length - 1; // calculate the score

    if (current_millis - last_millis_end_screen >= blink_delay){
      matrix.renderBitmap(clear_screen, 8, 12); // Render the clear screen
      if (current_millis - last_millis_end_screen >= blink_delay*2){
        number_on_matrix(score); // Render the score on the screen
        last_millis_end_screen = current_millis;
      }
    }
    

    // If the joystick is pressed, the game will reset
    if (joy_stick.get_SW()){
      reset_variables(); // Reset the variables to start the game again
      delay(100); 
    }
  }
    
}

void get_direction(){
  joy_stick_pos = joy_stick.get_direction(); // Get the direction of the joystick
  if (joy_stick_pos != "CENTER"){ // If the joystick is not in the center
    if (joy_stick_pos == "UP" && last_direction != "DOWN"){ // If the joystick is pointing up and the snake wasn't moving down
      direction = "UP";
    }else if (joy_stick_pos == "RIGHT" && last_direction != "LEFT"){ // If the joystick is pointing right and the snake wasn't moving left
      direction = "RIGHT";
    }else if (joy_stick_pos == "DOWN" && last_direction != "UP"){ // If the joystick is pointing down and the snake wasn't moving up
      direction = "DOWN";
    }else if (joy_stick_pos == "LEFT" && last_direction != "RIGHT"){ // If the joystick is pointing left and the snake wasn't moving right
      direction = "LEFT";
    }
  } 
}

void move(){
  // Move the body of the snake
  for (int i = snake_length; i > 0; i--){
    snake[i].x = snake[i-1].x;
    snake[i].y = snake[i-1].y;
  }

  // Move the head of the snake
  if (direction == "UP"){
    snake[0].y -= 1;
  }else if (direction == "RIGHT"){
    snake[0].x += 1;
  }else if (direction == "DOWN"){
    snake[0].y += 1;
  }else if (direction == "LEFT"){
    snake[0].x -= 1;
  }
}



void check_for_collisions(){
  // If the snake collides with the food
  if (snake[0].x == food.x && snake[0].y == food.y){
    snake_length++; // Increase the length of the snake
    speed *= float(multiplier); // Increase the speed of the snake
    generate_food(); // Generate a new food
  }

  // If the snake collides with itself
  for (int i = 1; i < snake_length; i++){
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y){ // If the head of the snake collides with the body of the snake
      playing = false;
      show_end_screen = true;
    }
  }

  // If the snake is out of the screen
  if (snake[0].x > screen_width-1){
    playing = false;
    show_end_screen = true;
  }else if (snake[0].x < 0){
    playing = false;
    show_end_screen = true;
  }else if (snake[0].y < 0){
    playing = false;
    show_end_screen = true;
  }else if (snake[0].y > screen_height-1){
    playing = false;
    show_end_screen = true;
  }
}

// Generate a new food
void generate_food(){
  food.x = random(0, screen_width); // Generate a random x position for the food
  food.y = random(0, screen_height); // Generate a random y position for the food

  // To avoid the food to be generated on the snake
  for (int i = 0; i < snake_length; i++){
    // If the food is generated on the snake, generate a new food
    if (food.x == snake[i].x && food.y == snake[i].y){
      generate_food();
    }
  }
}

// Render the snake and the food on the screen
void render_on_screen(){
  // Create clear screen
  byte on_screen[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };
  // Render each part of the snake
  for (int i = 0; i < snake_length; i++){
    on_screen[snake[i].y][snake[i].x] = 1;
  }
  current_millis = millis();
  if (current_millis - last_millis_food >= 1000/speed){
    on_screen[food.y][food.x] = 1;
    if (current_millis - last_millis_food >= 2000/speed){
      on_screen[food.y][food.x] = 0;
      last_millis_food = current_millis;
    }
  }
  matrix.renderBitmap(on_screen, 8, 12); // Render the bitmap on the screen
}

// This function is here because the get_number function is returning a pointer to a 2D array
void number_on_matrix(int number){
  byte (*p)[8][12] = get_number(number); // Get the pointer to the bitmap of the number

  // Create a copy of the bitmap
  byte copy[8][12];
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 12; ++j) {
      copy[i][j] = (*p)[i][j];
    }
  }
  matrix.renderBitmap(copy, 8, 12); // Render the bitmap on the screen
}

// Print the values to the serial monitor
void print(){
  if (playing){
    Serial.println("");
    Serial.print("direction: ");
    Serial.println(direction);

    Serial.print("snake length: ");
    Serial.println(snake_length);

    Serial.print("speed: ");
    Serial.println(speed);

    Serial.print("delay: ");
    Serial.print(1000/speed);
    Serial.println("ms");
  }
}

// Reset the variables to start the game again
void reset_variables(){
  randomSeed(analogRead(A4) + analogRead(A5)); // Seed for random food position (I am using 2 analog pins to get a more random seed)
  playing = false;
  show_end_screen = false;
  show_menu = true;
  speed = 3;
  // Reset the snake
  for (int i = 0; i < snake_length; i++){
    snake[i].x = 0;
    snake[i].y = 0;
  }
  snake_length = 1;
  snake[0].x = screen_width/2;
  snake[0].y = screen_height/2;
  direction = "";
  joy_stick_pos = "";
  generate_food();
  matrix.renderBitmap(clear_screen, 8, 12);
  delay(250);  
}
