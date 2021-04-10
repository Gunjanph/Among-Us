#include "imposter.h"
#include "main.h"

Imposter::Imposter(float x, float y, color_t color, int flag) {
    std::cout<<flag<<std::endl;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.6;
    health = 100;
    color_t color1, color2;
    color1.r = 255;
    color1.g = 0;
    color1.b = 0;
    color2.r = 0;
    color2.g = 255;
    color2.b = 255;
    // std::cout<<"define"<<std::endl;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    if (flag == 0)
    {    
        static const GLfloat temp_vertex_buffer_data[] = {
            1.25, 1.25, 0.01,
            0.0, 0.75, 0.01,
            1.25, 0.75, 0.01,
            
            0.0, 1.25, 0.01,
            1.25, 1.25, 0.01,
            0.0, 0.75, 0.01,

            -1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            -1.0, -1.0, 0.0,

            -1.0, 1.0, 0.0,
            -1.5, 1.0, 0.0,
            -1.5, -1.0, 0.0,
            
            1.0, 1.0, 0.0,
            -1.0, -1.0, 0.0,
            1.0, -1.0, 0.0,

            -1.0, 1.0, 0.0,
            -1.5, -1.0, 0.0,
            -1.0, -1.0, 0.0,

            -1.0, -1.0, 0.0,
            -1.0, -1.75, 0.0,
            -0.5, -1.0, 0.0,

            0.5, -1.75, 0.0,
            0.5, -1.0, 0.0,
            1.0, -1.0, 0.0,

            -1.0, -1.75, 0.0,
            -0.5, -1.0, 0.0,
            -0.5, -1.75, 0.0,

            0.5, -1.75, 0.0,
            1.0, -1.75, 0.0,
            1.0, -1.0, 0.0
        };
        static const GLfloat temp_color_buffer_data[] = {
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

            0, 0, 0.0,
            0, 0, 0.0,
            0, 0, 0.0,
            
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            
            0, 0, 0.0,
            0, 0, 0.0,
            0, 0, 0.0,

            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

        };
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
                vertex_color_data[j+3*t+0] = 0;
                vertex_color_data[j+3*t+1] = 0;
                vertex_color_data[j+3*t+2] = 0;
                temp[j+3*t+0] = 0;
                temp[j+3*t+1] = 0;
                temp[j+3*t+2] = 0;
            }
        }
        for(int i=0;i<90;i++){
            vertex_buffer_data[360*9+i] = temp_vertex_buffer_data[i]/7.0;
            vertex_color_data[360*9+i] = temp_color_buffer_data[i];
            temp[360*9+i]=vertex_color_data[360*9+i];
        }
        // std::cout<<"out"<<std::endsl;
        this->object = create3DObject(GL_TRIANGLES, 370*3, vertex_buffer_data, vertex_color_data, GL_FILL);
        // std::cout<<"outtt"<<std::endl;
    }
}

void Imposter::draw(glm::mat4 VP) {
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

void Imposter::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Imposter::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Imposter::move_left() {
    this->position.x -= speed;
}
void Imposter::move_right() {
    this->position.x += speed;
}
void Imposter::move_up() {
    this->position.y += speed;
}
void Imposter::move_down() {
    this->position.y -= speed;
}

void Imposter::lighting(int light, float posx, float posy)
{
    if(light==0)
    {
        for( int i =0 ; i<360 ; i++){
            int j = i*9;
            for(int t=0;t<3;t++){
                vertex_color_data[j+3*t+0] = 0;
                vertex_color_data[j+3*t+1] = 0;
                vertex_color_data[j+3*t+2] = 0;
            }
        }
        // for(int i=0;i<90;i+=9){
        //     for(int t=0;t<3;t++){
        //         vertex_color_data[i+3*t+0] = 0;
        //         vertex_color_data[i+3*t+1] = 1;
        //         vertex_color_data[i+3*t+2] = 0;
        //         if(i==27 || i == 45)
        //         {
        //             vertex_color_data[i+3*t+0] = 0;
        //             vertex_color_data[i+3*t+1] = 0.1;
        //             vertex_color_data[i+3*t+2] = 0;
        //         }
        //     }
        // }
        for(int i=0;i<90;i+=3){
                vertex_color_data[360*9+i+0] = temp[360*9+i+0];
                vertex_color_data[360*9+i+1] = temp[360*9+i+1];
                vertex_color_data[360*9+i+2] = temp[360*9+i+2];
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
                    vertex_color_data[j+3*t+0] = 0;
                    vertex_color_data[j+3*t+1] = 0;
                    vertex_color_data[j+3*t+2] = 0;
                }
            }
            for(int i=0;i<90;i+=3){
                vertex_color_data[360*9+i+0] = temp[360*9+i+0];
                vertex_color_data[360*9+i+1] = temp[360*9+i+1];
                vertex_color_data[360*9+i+2] = temp[360*9+i+2];
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
            for(int i=0;i<90;i+=3){
                vertex_color_data[360*9+i+0] = COLOR_BACKGROUND.r/255.0;
                vertex_color_data[360*9+i+1] = COLOR_BACKGROUND.g/255.0;
                vertex_color_data[360*9+i+2] = COLOR_BACKGROUND.b/255.0;
            }
        }
        this->object = create3DObject(GL_TRIANGLES, 370*3, vertex_buffer_data, vertex_color_data, GL_FILL);
    }
}