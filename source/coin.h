#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    GLfloat vertex_buffer_data[370*9];
    GLfloat vertex_color_data[370*9];
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void _delete();
    void lighting(int light, float posx, float posy);
    double speed;
private:
    VAO *object;
};

#endif // COIN_H