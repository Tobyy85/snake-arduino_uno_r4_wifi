/// this just go fisrt to the x or y coordinate and then to the other
#include "simple_ai.h"

SimpleAI::SimpleAI(int speed, int difficulty) : _matrix(){
    // Serial.begin(9600);
    _matrix.begin();
    randomSeed(analogRead(4) + analogRead(5));
    _snake_length = 1;
    _multiplier = float(1) + difficulty*0.02;
    _default_speed = speed;
    _speed = speed * float(_multiplier);
    _playing = true;
    coordinates _snake[96];
    _snake[0].x = _screen_width/2;
    _snake[0].y = _screen_height/2;
    coordinates _food;
    generate_food();
    get_direction();

}

void SimpleAI::move(){
    // Move the body of the snake
    get_direction();
    for (int i = _snake_length; i > 0; i--){
        _snake[i].x = _snake[i-1].x;
        _snake[i].y = _snake[i-1].y;
    }
  
    // Move the head of the snake
    if (_direction == "up"){
        _snake[0].y -= 1;
    }else if (_direction == "right"){
        _snake[0].x += 1;
    }else if (_direction == "down"){
        _snake[0].y += 1;
    }else if (_direction == "left"){
        _snake[0].x -= 1;
    }

}

void SimpleAI::check_for_collisions(){
    // If the snake collides with the food
    if (_snake[0].x == _food.x && _snake[0].y == _food.y){
        _snake_length++; // Increase the length of the snake
        _speed *= float(_multiplier); // Increase the speed of the snake
        generate_food(); // Generate a new food
    }

    // If the snake collides with itself
    for (int i = 1; i < _snake_length; i++){
        if (_snake[0].x == _snake[i].x && _snake[0].y == _snake[i].y){ // If the head of the snake collides with the body of the snake
            _playing = false;
        }
    }

    // If the snake is out of the screen
    if (_snake[0].x > _screen_width-1){
        _playing = false;
    }else if (_snake[0].x < 0){
        _playing = false;
    }else if (_snake[0].y < 0){
        _playing = false;
    }else if (_snake[0].y > _screen_height-1){
        _playing = false;
    }
   

}

void SimpleAI::render_on_screen(){
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
    // Render the snake
    for (int i = 0; i < _snake_length; i++){
        on_screen[_snake[i].y][_snake[i].x] = 1;
    }

    // Render the food
    on_screen[_food.y][_food.x] = 1;
    _matrix.renderBitmap(on_screen, 8, 12);

}


void SimpleAI::reset_variables(){
    _snake_length = 1;
    _speed = _default_speed;
    _playing = true;
    _snake[0].x = _screen_width/2;
    _snake[0].y = _screen_height/2;
    generate_food();
    get_direction();
}

int SimpleAI::get_score(){
    return _snake_length - 1;
}





void SimpleAI::get_direction(){
    // The snake go to the food coordinates
    // It will calculate which axis is closer to the food and go to that axis first
    int distance_x = abs(_snake[0].x - _food.x);
    int distance_y = abs(_snake[0].y - _food.y);
    if (distance_x < distance_y && _snake[0].x != _food.x){
        if (_snake[0].x < _food.x && _direction != "left"){
            _direction = "right";
        }else if (_snake[0].x > _food.x && _direction != "right"){
            _direction = "left";
        }
    
    }else if (_snake[0].y != _food.y){
        if (_snake[0].y < _food.y && _direction != "up"){
            _direction = "down";
        }else if (_snake[0].y > _food.y && _direction != "down"){
            _direction = "up";
        }
    }else{
        if (_snake[0].x < _food.x && _direction != "left"){
            _direction = "right";
        }else if (_snake[0].x > _food.x && _direction != "right"){
            _direction = "left";
        }else if (_snake[0].y < _food.y && _direction != "up"){
            _direction = "down";
        }else if (_snake[0].y > _food.y && _direction != "down"){
            _direction = "up";
        }
    
    }
}


void SimpleAI::generate_food(){
  _food.x = random(0, _screen_width); // Generate a random x position for the food
  _food.y = random(0, _screen_height); // Generate a random y position for the food

  // To avoid the food to be generated on the snake
  for (int i = 0; i < _snake_length; i++){
    // If the food is generated on the snake, generate a new food
    if (_food.x == _snake[i].x && _food.y == _snake[i].y){
      generate_food();
    }
  }
}