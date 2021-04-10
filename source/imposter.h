#include "main.h"

#ifndef IMPOSTER_H
#define IMPOSTER_H


class Imposter {
public:
    Imposter() {}
    Imposter(float x, float y, color_t color, int flag);
    GLfloat vertex_buffer_data[370*9];
    GLfloat vertex_color_data[370*9];
    GLfloat temp[370*9];
    glm::vec3 position;
    float rotation;
    int health;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void move_left();
    void move_right();
    void move_up();
    void move_down();
    void lighting(int light, float posx, float posy);
    double speed;
private:
    VAO *object;
};

#endif // Player_H