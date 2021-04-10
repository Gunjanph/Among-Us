#include "bomb.h"
#include "main.h"

Bomb::Bomb(float x, float y, color_t color) {
    // std::cout<<"innn"<<std::endl;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.6;
    // GLfloat vertex_buffer_data[370*9];
    // GLfloat vertex_color_data[370*9];
    // std::cout<<"define"<<std::endl;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    // std::cout<<"after"<<std::endl;
    GLfloat x1,y1,z,x2,y2,x3,y3;
    for( int i =0 ; i<360 ; i++){
        int j = i*9;
        x1 = 0.0;
        vertex_buffer_data[j+0] = (x1+0)/7.0;
        y1 = 0.0;
        vertex_buffer_data[j+1] = (y1+1)/7.0;
        z = 0.005;
        vertex_buffer_data[j+2] = (z)/7.0;
        vertex_buffer_data[j+5] = (z)/7.0;
        vertex_buffer_data[j+8] = (z)/7.0;
        x2 = sin(i*(M_PI/180.0));
        vertex_buffer_data[j+3] = (x2+0)/7.0;
        y2 = cos(i*(M_PI/180.0));
        vertex_buffer_data[j+4] = (y2+1)/7.0;
        x3 = sin((i+1)*(M_PI/180.0));
        vertex_buffer_data[j+6] = (x3+0)/7.0;
        y3 = cos((i+1)*(M_PI/180.0));
        vertex_buffer_data[j+7] = (y3+1)/7.0;
        for(int t=0;t<3;t++){
            vertex_color_data[j+3*t+0] = 1;
            vertex_color_data[j+3*t+1] = 0;
            vertex_color_data[j+3*t+2] = 0;
        }
    }

    // std::cout<<"out"<<std::endsl;
    this->object = create3DObject(GL_TRIANGLES, 360*3, vertex_buffer_data, vertex_color_data, GL_FILL);
    // std::cout<<"outtt"<<std::endl;
}

void Bomb::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Bomb::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Bomb::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Bomb::_delete() {
    glDeleteVertexArrays(1, &this->object->VertexArrayID);
    // this->object = create3DObject(GL_TRIANGLES, 360*3, vertex_buffer_data, COLOR_BACKGROUND, GL_FILL);
    // glDeleteBuffers(1, &this->object->VertexBuffer);
    // glDeleteBuffers(1, &this->object->ColorBuffer);
}

void Bomb::lighting(int light, float posx, float posy)
{
    if(light==0)
    {
        for( int i =0 ; i<360 ; i++){
            int j = i*9;
            for(int t=0;t<3;t++){
                vertex_color_data[j+3*t+0] = 1;
                vertex_color_data[j+3*t+1] = 0;
                vertex_color_data[j+3*t+2] = 0;
            }
        }
        this->object = create3DObject(GL_TRIANGLES, 370*3, vertex_buffer_data, vertex_color_data, GL_FILL);
    }
    else
    {
        if((abs(this->position.x-posx)<1) &&( abs(this->position.y-posy)<1 ))
        {
            for( int i =0 ; i<360 ; i++){
                int j = i*9;
                for(int t=0;t<3;t++){
                    vertex_color_data[j+3*t+0] = 1;
                    vertex_color_data[j+3*t+1] = 0;
                    vertex_color_data[j+3*t+2] = 0;
                }
            }
        }
        else
        {
            for( int i =0 ; i<360 ; i++){
                int j = i*9;
                for(int t=0;t<3;t++){
                    vertex_color_data[j+3*t+0] = COLOR_BACKGROUND.r/255.0;
                    vertex_color_data[j+3*t+1] = COLOR_BACKGROUND.g/255.0;
                    vertex_color_data[j+3*t+2] = COLOR_BACKGROUND.b/255.0;
                }
            }
        }
        this->object = create3DObject(GL_TRIANGLES, 370*3, vertex_buffer_data, vertex_color_data, GL_FILL);
    }
}