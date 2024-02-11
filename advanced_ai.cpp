// #include "advanced_ai.h"

// // I need to every time create copy of the snake and move him and then check for collision. By that I can get the best direction to move the snake

// AdvancedAI::AdvancedAI(int speed, int difficulty) : _matrix(){
//     _matrix.begin();
//     randomSeed(analogRead(4) + analogRead(5));

//     _multiplier = float(1) + difficulty*0.02;
//     _default_speed = speed;
//     _speed = speed * float(_multiplier);
//     _snake_length = 1;
//     _playing = true;

//     coordinates _snake[96];
//     _snake[0].x = _screen_width/2;
//     _snake[0].y = _screen_height/2;
//     coordinates _food;
    
//     generate_food();
//     get_direction();

// }


// void AdvancedAI::render_on_screen(){
//     // Create clear screen
//     byte on_screen[8][12] = {
//     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//     };
//     // Render the snake
//     for (int i = 0; i < _snake_length; i++){
//         on_screen[_snake[i].y][_snake[i].x] = 1;
//     }

//     // Render the food
//     on_screen[_food.y][_food.x] = 1;
//     _matrix.renderBitmap(on_screen, 8, 12);

// }


// void AdvancedAI::reset_variables(){
//     _snake_length = 1;
//     _speed = _default_speed;
//     _playing = true;
//     _snake[0].x = _screen_width/2;
//     _snake[0].y = _screen_height/2;
//     generate_food();
//     get_direction();
// }

// int AdvancedAI::get_score(){
//     return _snake_length - 1;
// }


// void AdvancedAI::generate_food(){
//   _food.x = random(0, _screen_width); // Generate a random x position for the food
//   _food.y = random(0, _screen_height); // Generate a random y position for the food

//   // To avoid the food to be generated on the snake
//   for (int i = 0; i < _snake_length; i++){
//     // If the food is generated on the snake, generate a new food
//     if (_food.x == _snake[i].x && _food.y == _snake[i].y){
//       generate_food();
//     }
//   }
// }




// void AdvancedAI::get_direction(){
//     if (_direction == "up"){
//         if (_food.y >= _snake[0].y){
//             if (_food.x < _snake[0].x){
//                 _direction = "left";
//             }else if (_food.x > _snake[0].x){
//                 _direction = "right";
//             }else{
//                 // I need to get how close is from left and right wall
//                 int from_left = _snake[0].x;
//                 int from_right = _screen_width-1 - _snake[0].x;
//                 if (from_left > from_right){
//                     _direction = "left";
//                 }else if (from_left < from_right){
//                     _direction = "right";
//                 }else{
//                     _direction = random(0, 2) == 0 ? "left" : "right";
//                 }
//             }
//         }
//     }else if (_direction == "down"){
//         if (_food.y <= _snake[0].y){
//             if (_food.x < _snake[0].x){
//                 _direction = "left";
//             }else if (_food.x > _snake[0].x){
//                 _direction = "right";
//             }else{
//                 // I need to get how close is from left and right wall
//                 int from_left = _snake[0].x;
//                 int from_right = _screen_width-1 - _snake[0].x;
//                 if (from_left > from_right){
//                     _direction = "left";
//                 }else if (from_left < from_right){
//                     _direction = "right";
//                 }else{
//                     _direction = random(0, 2) == 0 ? "left" : "right";
//                 }
//             }
//         }
//     }else if (_direction == "left"){
//         if (_food.x >= _snake[0].x){
//             if (_food.y < _snake[0].y){
//                 _direction = "up";
//             }else if (_food.y > _snake[0].y){
//                 _direction = "down";
//             }else{
//                 // I need to get how close is from up and down wall
//                 int from_up = _snake[0].y;
//                 int from_down = _screen_height-1 - _snake[0].y;
//                 if (from_up > from_down){
//                     _direction = "up";
//                 }else if (from_up < from_down){
//                     _direction = "down";
//                 }else{
//                     _direction = random(0, 2) == 0 ? "up" : "down";
//                 }
//             }
//         }
//     }else if (_direction == "right"){
//         if (_food.x <= _snake[0].x){
//             if (_food.y < _snake[0].y){
//                 _direction = "up";
//             }else if (_food.y > _snake[0].y){
//                 _direction = "down";
//             }else{
//                 // I need to get how close is from up and down wall
//                 int from_up = _snake[0].y;
//                 int from_down = _screen_height-1 - _snake[0].y;
//                 if (from_up > from_down){
//                     _direction = "up";
//                 }else if (from_up < from_down){
//                     _direction = "down";
//                 }else{
//                     _direction = random(0, 2) == 0 ? "up" : "down";
//                 }
//             }
//         }
//     }else{
//         int x_difference = abs(_food.x - _snake[0].x);
//         int y_difference = abs(_food.y - _snake[0].y);
//         if (x_difference > y_difference && x_difference != 0){
//             if (_food.x < _snake[0].x){
//                 _direction = "left";
//             }else if (_food.x > _snake[0].x){
//                 _direction = "right";
//             }
//         }else if (y_difference > x_difference && y_difference != 0){
//             if (_food.y < _snake[0].y){
//                 _direction = "up";
//             }else if (_food.y > _snake[0].y){
//                 _direction = "down";
//             }
//         }
//     }
// }

// bool AdvancedAI::can_move(String direction){


// }


