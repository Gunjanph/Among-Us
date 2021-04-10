#include "main.h"

#ifndef BUTTON_H
#define BUTTON_H


class Button {
public:
    Button() {}
    Button(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    GLfloat vertex_buffer_data[370*9];
    GLfloat vertex_color_data[370*9];
    color_t temp;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void _delete();
    void lighting(int light, float posx, float posy);
    double speed;
private:
    VAO *object;
};

#endif // Button_H