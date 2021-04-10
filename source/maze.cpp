#include "main.h"
#include "maze.h"

Maze::Maze(float x, float y, color_t color, float start, float end)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    int width = 10;
    int height = 10;
    int ax=3.5;
    int ay=3.5;
    coord = 0;
    int grid[21][21];
    // GLfloat vertex_data[20000];
    std::cout<<"define"<<std::endl;
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            if(j==height-1 && i==end)
                grid[i][j]=-1;
            else if(j==0 && i==start)
                grid[i][j]=-1;
            else if(i==width-1)
                grid[i][j] = 1;
            else if(j==height-1)
                grid[i][j] = 0;
            else
                grid[i][j] = rand()%2;
            
            if(grid[i][j]==0)
            {
                vertex_data[coord+1]=-0.6*i+ax;
                vertex_data[coord]=-1*(-0.6*(j+1)+ay);
                vertex_data[coord+2]=0;
                vertex_data[coord+4]=-0.6*(i+1)+ax;
                vertex_data[coord+3]=-1*(-0.6*(j+1)+ay);
                vertex_data[coord+5]=0;
                coord+=6;
            }
            if(grid[i][j]==1)
            {
                vertex_data[coord+1]=-0.6*(i+1)+ax;
                vertex_data[coord]=-1*(-0.6*j+ay);
                vertex_data[coord+2]=0;
                vertex_data[coord+4]=-0.6*(i+1)+ax;
                vertex_data[coord+3]=-1*(-0.6*(j+1)+ay);
                vertex_data[coord+5]=0;
                coord+=6;
            }
            if(i == width-1 && j == height-1 && grid[width-1][height-1]!=-1)
            {
                vertex_data[coord+1]=-0.6*i+ax;
                vertex_data[coord]=-1*(-0.6*(j+1)+ay);
                vertex_data[coord+2]=0;
                vertex_data[coord+4]=-0.6*(i+1)+ax;
                vertex_data[coord+3]=-1*(-0.6*(j+1)+ay);
                vertex_data[coord+5]=0;
                coord+=6;
            }

            if(i == 0)
            {
                vertex_data[coord+1]=-0.6*i+ax;
                vertex_data[coord]=-1*(-0.6*(j)+ay);
                vertex_data[coord+2]=0;
                vertex_data[coord+4]=-0.6*(i)+ax;
                vertex_data[coord+3]=-1*(-0.6*(j+1)+ay);
                vertex_data[coord+5]=0;
                coord+=6;
            }
            if( j== 0 && grid[i][j]!=-1)
            {
                vertex_data[coord+1]=-0.6*i+ax;
                vertex_data[coord]=-1*(-0.6*(j)+ay);
                vertex_data[coord+2]=0;
                vertex_data[coord+4]=-0.6*(i+1)+ax;
                vertex_data[coord+3]=-1*(-0.6*(j)+ay);
                vertex_data[coord+5]=0;
                coord+=6;
            }
            std::cout<<grid[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<coord<<std::endl;
    this->object = create3DObject(GL_LINES, coord, vertex_data, COLOR_BLACK, GL_FILL);
    std::cout<<"out for"<<std::endl;
}
void Maze::draw(glm::mat4 VP) {
    // std::cout<<"draw"<<std::endl;
    Matrices.model = glm::mat4(1.0f);
    // this->position = glm::vec3(ax,3.5,0);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    // this->rotation = 90;
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    // std::cout<<Matrices.model<<std::endl;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    // std::cout<<"drawout"<<std::endl;
}

void Maze::set_position(float x, float y) {
    // std::cout<<"in set"<<std::endl;
    this->position = glm::vec3(x, y, 0);
    // std::cout<<"out set"<<std::endl;
}

int Maze::collision(float x1, float y1, float x2, float y2) {
    // std::cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<std::endl;
    for(int i=0;i<coord;i+=6)
    {
        if(x1!=x2)
        {
            // std::cout<<vertex_data[i]<<" "<<vertex_data[i+1]<<" "<<vertex_data[i+3]<<" "<<vertex_data[i+4]<<" "<<vertex_data[i]-x1<<" "<<vertex_data[i]-x2<<std::endl;
            if(vertex_data[i+1]!=vertex_data[i+4] && (vertex_data[i]-x1)*(vertex_data[i]-x2)<0 && (vertex_data[i+1]-y1)*(vertex_data[i+4]-y1)<0)
                return 1;
        }
        else
        {
            // std::cout<<vertex_data[i+1]<<" "<<vertex_data[i+1]-y1<<" "<<vertex_data[i+1]-y2<<std::endl;
            if(vertex_data[i]!=vertex_data[i+3] && (vertex_data[i+1]-y1)*(vertex_data[i+1]-y2)<0 && (vertex_data[i]-x1)*(vertex_data[i+3]-x1)<0)
                return 1;
        }
    }
    return 0;
}

void Maze::lighting(int light, float posx, float posy)
{
    if(light==0)
    {
        this->object = create3DObject(GL_LINES, coord, vertex_data, COLOR_BLACK, GL_FILL);
    }
    else
    {
        for(int i=0;i<coord;i+=6)
        {
            if((abs(vertex_data[i]-posx)<1 || abs(vertex_data[i+3]-posx)<1) &&( abs(vertex_data[i+1]-posy)<1 || abs(vertex_data[i+4]-posy)<1 ))
            {
                // std::cout<<"if\n";
                color_data[i]=0;
                color_data[i+1]=0;
                color_data[i+2]=0;
                color_data[i+3]=0;
                color_data[i+4]=0;
                color_data[i+5]=0;
            }
            else
            {
                color_data[i]=COLOR_BACKGROUND.r/255.0;
                color_data[i+1]=COLOR_BACKGROUND.g/255.0;
                color_data[i+2]=COLOR_BACKGROUND.b/255.0;
                color_data[i+3]=COLOR_BACKGROUND.r/255.0;
                color_data[i+4]=COLOR_BACKGROUND.g/255.0;
                color_data[i+5]=COLOR_BACKGROUND.b/255.0;
            }
        }
        this->object = create3DObject(GL_LINES, coord, vertex_data, color_data, GL_FILL);
    }
}

