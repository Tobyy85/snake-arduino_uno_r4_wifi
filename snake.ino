#include <Arduino_LED_Matrix.h>

#include "JoyStick.h"
#include "number_array.h"
#include "clear_screen.h"

ArduinoLEDMatrix matrix;

// 507 and 509 are values for the joystick in normal position
// 5 and 5 are the deadzone precent (to ignore small movements of the joystick)
// you can change the SW pin to digital pin
JoyStick joy_stick(A0, A1, A2, 509, 507, 5, 5);


const int screen_width = 12;
const int screen_height = 8;

bool playing = false;
bool show_end_screen = false;
bool show_menu = true;

float difficulty = 5;//1-9
float multiplier;//0.02*difficulty + 1
float speed = 3;
float delay_time = 1000/speed;//ms


struct coordinates{
  int x;
  int y;
};
int snake_length = 1;
coordinates snake[96]; //max length of snake is 96 (8*12)
coordinates food;

String joy_stick_pos = ""; // "UP", "RIGHT", "DOWN", "LEFT", "CENTER"
String direction = ""; // "UP", "RIGHT", "DOWN", "LEFT"
String last_direction = ""; // "UP", "RIGHT", "DOWN", "LEFT"


void setup() {
  Serial.begin(9600);
  matrix.begin();

  randomSeed(analogRead(A4) + analogRead(A5));//seed for random food position (I am using 2 analog pins to get a more random seed)

  generate_food(); //generate the first food
  snake[0].x = screen_width/2; // start position of the snake is in the middle of the screen
  snake[0].y = screen_height/2; // start position of the snake is in the middle of the screen

}

void loop() {
  // print();
  
  if (playing){
    delay_time = 1000/speed;
    
    for (int _ = 0; _ < 10; _++){
      get_direction();
      delay(delay_time/10);
    }
    last_direction = direction;

    move();
    check_for_collisions();
    if (!show_end_screen){
      render_on_screen();
    }
  
  }else if (show_menu){

    
    if (difficulty > 1 && joy_stick.get_x_direction() == "LEFT"){
      difficulty--;
      delay(150);
    }else if (difficulty < 9 && joy_stick.get_x_direction() == "RIGHT"){
      difficulty++;
      delay(150);
    }

    if (joy_stick.get_SW()){
      multiplier = float(0.02*difficulty) + 1;
      speed *= multiplier;
      playing = true;
      show_menu = false;
    }

    number_on_matrix(difficulty);
  


  }else if (show_end_screen){
    delay(250);
    end_screen();
  }
    
  
  
  
}

void get_direction(){
  joy_stick_pos = joy_stick.get_direction();
  if (joy_stick_pos != "CENTER"){
    if (joy_stick_pos == "UP" && direction != "DOWN" && last_direction != "DOWN"){
      direction = "UP";
    }else if (joy_stick_pos == "RIGHT" && direction != "LEFT" && last_direction != "LEFT"){
      direction = "RIGHT";
    }else if (joy_stick_pos == "DOWN" && direction != "UP" && last_direction != "UP"){
      direction = "DOWN";
    }else if (joy_stick_pos == "LEFT" && direction != "RIGHT" && last_direction != "RIGHT"){
      direction = "LEFT";
    }
  }
  
}

void move(){
  
  //move the body of the snake
  for (int i = snake_length; i > 0; i--){
    snake[i].x = snake[i-1].x;
    snake[i].y = snake[i-1].y;
  }

  //move the head of the snake
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

void generate_food(){
  
  food.x = random(0, screen_width);
  food.y = random(0, screen_height);

  //check if food is not on snake
  for (int i = 0; i < snake_length; i++){
    if (food.x == snake[i].x && food.y == snake[i].y){
      generate_food();
      return;
    }
  }

  

}

void render_on_screen(){
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

  for (int i = 0; i < snake_length; i++){
    on_screen[snake[i].y][snake[i].x] = 1;
  }
  on_screen[food.y][food.x] = 1;

  
  matrix.renderBitmap(on_screen, 8, 12);

}

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


void check_for_collisions(){
  //food collision
  if (snake[0].x == food.x && snake[0].y == food.y){
    snake_length++;
    speed *= float(multiplier);
    generate_food();
  }

  //collision with self
  for (int i = 1; i < snake_length; i++){
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y){
      playing = false;
      show_end_screen = true;
    }
  }


  //if snake is out of screen
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

void number_on_matrix(int number){
  // Get the bitmap for the number
  byte (*p)[8][12] = get_number(number);

  // Create a copy of the bitmap
  byte copy[8][12];
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 12; ++j) {
      copy[i][j] = (*p)[i][j];
    }
  }

  matrix.renderBitmap(copy, 8, 12);
}


void end_screen(){
  int score = snake_length - 1;
  number_on_matrix(score);
  delay(250);
  matrix.renderBitmap(clear_screen, 8, 12);
  if (joy_stick.get_SW()){
    reset_variables();
  }
}


void reset_variables(){
  randomSeed(analogRead(A4) + analogRead(A5));
  Serial.print("seed: ");
  Serial.println(analogRead(A4) + analogRead(A5));
  playing = false;
  show_end_screen = false;
  show_menu = true;
  speed = 3;
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
  delay(500);  
}


