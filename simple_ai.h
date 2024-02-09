#ifndef SIMPLE_AI_H
#define SIMPLE_AI_H

#include "Arduino.h"

#include <Arduino_LED_Matrix.h>

class SimpleAI{
    public:
        SimpleAI(int speed = 10, int difficulty = 9);
        void move();
        void check_for_collisions();
        void render_on_screen();

        void reset_variables();
        int get_score();


        float _speed;
        bool _playing;




    private:
        void generate_food();
        void get_direction();
        






        ArduinoLEDMatrix _matrix;
        struct coordinates{
            int x;
            int y;
            };
        const int _screen_width = 12;
        const int _screen_height = 8;
        int _snake_length;
        float _multiplier;
        int _default_speed;
        String _direction;
        coordinates _snake[96];
        coordinates _food;
        



};










#endif