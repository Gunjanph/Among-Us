#include "main.h"

#ifndef MAZE_H
#define MAZE_H


class Maze {
public:
    Maze() {}
    Maze(float x, float y, color_t color, float start, float end);
    glm::vec3 position;
    float rotation;
    GLfloat vertex_data[20000];
    GLfloat color_data[20000];
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    int collision(float x1, float y1, float x2, float y2);
    void lighting(int light, float posx, float posy);
    double speed;
    int coord;
private:
    VAO *object;
};

#endif // MAZE_H