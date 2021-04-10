#include "main.h"
#include "timer.h"
#include "player.h"
#include "maze.h"
#include "button.h"
#include "imposter.h"
#include "coin.h"
#include "bomb.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Player ball1;
Maze maze1;
Button button;
Button button1;
Imposter imposter;
Coin coin1;
Coin coin2;
cell *Cell;
Bomb bomb1;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 90;
float start, finish;
int delete_flag=0;
int coin1_flag=0;
int coin2_flag=0;
int bomb_flag=0;
int imp_flag=0;
int win_flag=0;
int game_flag=0;
int task = 0;
int lights = 0;
int reset_flag = 0;

// COLOR_BACKGROUND.r = 0;
// COLOR_BACKGROUND.g = 0;
// COLOR_BACKGROUND.b = 0;
Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    if(reset_flag==0)
    {
        ball1.draw(VP);
        if(delete_flag==0)
        {
            button.draw(VP);
        }
        if(delete_flag==1 && coin1_flag==0)
        {
            coin1.draw(VP);
        }
        if(delete_flag==1 && coin2_flag==0)
        {
            coin2.draw(VP);
        }
        if(delete_flag==1 && bomb_flag==0)
        {
            bomb1.draw(VP);
        }
        maze1.draw(VP);
        if(imp_flag==0)
        {
            button1.draw(VP);
            imposter.draw(VP);
        }
    }
}

void shortest_path()
{
    // sleep(1);
    float x1,y1; // player coordiates
    float x2,y2; // imposer coordiates

    // Make Graph
    float x0=-2.6;
    float y0=2.7;
    float d=0.6;
    float a[10][10][2];
    a[0][0][0] = x0;
    a[0][0][1] = y0;
    x1 = ball1.position.x;
    y1 = ball1.position.y;
    x2 = imposter.position.x;
    y2 = imposter.position.y;
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(i+j==0)
            {
                continue;
            }
            else if(j==0)
            {
                a[i][j][0]=a[i-1][j][0];
                a[i][j][1]=a[i-1][j][1]-d;
            }
            else
            {
                a[i][j][0]=a[i][j-1][0]+d;
                a[i][j][1]=a[i][j-1][1];
            }
        }
    }

    // for(int i=0;i<10;i++)
    // {
    //     for(int j=0;j<10;j++)
    //     {
    //         //("(%0.2f %0.2f)\t",a[i][j][0],a[i][j][1]);
    //     }
    //     //("\n");
    // }

    
    vector <pair<pair<float,float>,int >>vc;
    int c=0;
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            vc.push_back(make_pair(make_pair(a[i][j][0],a[i][j][1]),c));
            c++;
        }
    }
    //("YO\n");


    
    vector <int> al[100];

    c=0;
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            // vc.push_back(make_pair(c,make_pair(a[i][j][0],a[i][j][1])));
            if(i==0 and j==0)
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j+1][0],a[i][j+1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i+1][j][0],a[i+1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+10);
                }
            }
            if(i==0 and j==9)
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j-1][0],a[i][j-1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i+1][j][0],a[i+1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+10);
                }
            }
            if(i==9 and j==0)
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j+1][0],a[i][j+1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i-1][j][0],a[i-1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-10);
                }
            }
            if(i==9 and j==9)
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j-1][0],a[i][j-1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i-1][j][0],a[i-1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-10);
                }
            }
            else if( i == 0)
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j-1][0],a[i][j-1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j+1][0],a[i][j+1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i+1][j][0],a[i+1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+10);
                }
            }
            else if( i == 9)
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j-1][0],a[i][j-1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j+1][0],a[i][j+1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i-1][j][0],a[i-1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-10);
                }
            }
            else if( j == 0)
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i+1][j][0],a[i+1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+10);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i-1][j][0],a[i-1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-10);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j+1][0],a[i][j+1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+1);
                }
            }
            else if( j == 9)
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i+1][j][0],a[i+1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+10);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i-1][j][0],a[i-1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-10);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j-1][0],a[i][j-1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-1);
                }
            }
            else
            {
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i+1][j][0],a[i+1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+10);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i-1][j][0],a[i-1][j][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-10);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j-1][0],a[i][j-1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c-1);
                }
                if (maze1.collision(a[i][j][0],a[i][j][1],a[i][j+1][0],a[i][j+1][1])==0) // agar colision nahi ho raha to
                {
                    al[c].push_back(c+1);
                }
            }
            c++;
        }
    }
    // printf(Gra)
    //("YOO\n");

    int pl;
    int im;
    for(int i=0;i<vc.size();i++)
    {
        // std::cout<<vc[i].first.first<<" "<<x1<<" "<<vc[i].first.second<<" "<<y1<<std::endl;
        if(abs(vc[i].first.first-x1)<0.0001 and abs(vc[i].first.second-y1)<0.0001)
        {
            pl=vc[i].second;
        }
        if(abs(vc[i].first.first-x2)<0.0001 and abs(vc[i].first.second-y2)<0.0001)
        {
            im=vc[i].second;
        }
    }

    //("YOOO\n");
    int cp[100];
    for(int i=0;i<100;i++)
    {
        cp[i]=0;
    }
    //("Yess\n");
    int p[1000];
    //("%d\n",pl);
    p[pl]=-1;
    //("Yes1\n");
    vector <int> s;
    //("Yes1..\n");
    s.push_back(pl);
    //("Yes2\n");
    cp[pl]=1;
    int b=0;
    int t=1;
    //("Yes3\n");
    // for(int i=0;i<100;i++)
    // {
    //     printf("%d->",i);
    //     for(int j=0;j<al[i].size();j++)
    //     {
    //         printf("%d,",al[i][j]);
    //     }
    //     printf("\n");
    // }
    while(b<t)
    {
        //("Yessss\n");
        for(int i=0;i<al[s[b]].size();i++)
        {
            // cout<<s[b]<<endl;
            //("Yesssss\n");
            if(cp[al[s[b]][i]]==0)
            {
                // cout<<s[b]<<endl;
                cp[al[s[b]][i]]=1;
                s.push_back(al[s[b]][i]);
                p[al[s[b]][i]]=s[b];
                t++;
            }
        }
        b++;
    }
    // printf("YOOOO\n");
    int crt=p[im];

    // printf("pl=%d im=%d\n",pl,im);
    // for(int i=0;i<100;i++)
    // {
    //     cout<<p[i]<<endl;
    // }
    // cout<<im<<endl;
    // printf("Y:(\n");
    
    // while(p[p[crt]]!=-1)
    // {
    //     printf("%d->",crt);
    //     crt=p[crt];
    // }
    // printf("%d %d\n",crt,p[crt]);
    float xn,yn;

    for(int i=0;i<vc.size();i++)
    {
        if(vc[i].second==crt)
        {
            xn=vc[i].first.first;
            yn=vc[i].first.second;
        }
    }
    // printf("Y:(\n");
    imposter.set_position(xn,yn);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int light = glfwGetKey(window, GLFW_KEY_T);

    if (left) {
        // std::cout<<ball1.position.x<<std::endl;
        if(maze1.collision(ball1.position.x, ball1.position.y, ball1.position.x-0.6, ball1.position.y)==0)
        {
            ball1.move_left();
        }
        // std::cout<<ball1.position.x<<std::endl;
    }
    else if (right) {
        // std::cout<<"r"<<std::endl;
        if(maze1.collision(ball1.position.x, ball1.position.y, ball1.position.x+0.6, ball1.position.y)==0)
        {
            ball1.move_right();
        }
    }
    else if (up) {
        // std::cout<<"u"<<std::endl;
        if(maze1.collision(ball1.position.x, ball1.position.y, ball1.position.x, ball1.position.y+0.6)==0)
        {
            ball1.move_up();
        }
    }
    else if (down) {
        // std::cout<<"d"<<std::endl;
        if(maze1.collision(ball1.position.x, ball1.position.y, ball1.position.x, ball1.position.y-0.6)==0)
        {
            ball1.move_down();
        }
    }
    else if(light)
    {
        if(lights==1)
            lights=0;
        else
            lights=1;
    }
}

void tick_elements() {
    // if(maze1.collision(ball1.position.x, ball1.position.y, ball1.position.x, ball1.position.y-0.6)==0)
    // {
    //         ball1.move_down();
    // }
    // std::cout<<ball1.position.x<<" "<<button.position.x<<" "<<ball1.position.y<<" "<<button.position.y<<std::endl;
    if(delete_flag==0 && abs(ball1.position.x - button.position.x)<0.01 && abs(ball1.position.y - button.position.y)<0.01)
    {
        std::cout<<"iffff"<<std::endl;
        // button._delete();
        delete_flag=1;
    }
    if(imp_flag == 0 && abs(ball1.position.x - button1.position.x)<0.01 && abs(ball1.position.y - button1.position.y)<0.01)
    {
        imp_flag=1;
    }
    if(delete_flag==1 && coin1_flag==0 && abs(ball1.position.x - coin1.position.x)<0.01 && abs(ball1.position.y - coin1.position.y)<0.01)
    {
        coin1_flag=2;
        task++;
        ball1.health+=10;
    }
    if(delete_flag==1 && bomb_flag==0 && abs(ball1.position.x - bomb1.position.x)<0.01 && abs(ball1.position.y - bomb1.position.y)<0.01)
    {
        bomb_flag=-1;
        ball1.health-=10;
    }
    if(delete_flag==1 && coin2_flag==0 && abs(ball1.position.x - coin2.position.x)<0.01 && abs(ball1.position.y - coin2.position.y)<0.01)
    {
        coin2_flag=2;
        task++;
        ball1.health+=10;
    }
    if(task == 2 && abs(ball1.position.x - 2.8)<0.01 && abs(ball1.position.y - 2.7+(0.6*finish))<0.01)
    {
        win_flag=1;
    }
    if(imp_flag==0 && abs(ball1.position.x - imposter.position.x)<0.01 && abs(ball1.position.y - imposter.position.y)<0.01)
    {
        game_flag=1;
    }
}

void lighting()
{
    maze1.lighting(lights, ball1.position.x, ball1.position.y);
    imposter.lighting(lights, ball1.position.x, ball1.position.y);
    button1.lighting(lights, ball1.position.x, ball1.position.y);
    button.lighting(lights, ball1.position.x, ball1.position.y);
    coin1.lighting(lights, ball1.position.x, ball1.position.y);
    coin2.lighting(lights, ball1.position.x, ball1.position.y);
    bomb1.lighting(lights, ball1.position.x, ball1.position.y);
}

// void maze(int maze_height, int maze_width)
// {
//     for(int i=0;i<maze_width;i++)
//     {
//         for(int j=0;j<maze_height;j++)
//         {
//             if(Cell[i].edge[0]==1)
//             {
//                 glDrawArrays(GL_LINES, 1, GL_UNSIGNED_INT, x+1);
//             }
//         }
//     }
// }

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height, float start, float finish, float b_start, float b_end, float imp_start, float imp_end, float b1_start, float b1_end, float c1_start, float c1_end, float bomb1_start, float bomb1_end, float c2_start, float c2_end) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    // start = 0;
    maze1 = Maze(0, 0, COLOR_BLACK,start, finish);
    std::cout<<start<<" "<<finish<<std::endl;
    color_t color1;
    color_t color2;
    color1.r = 1;
    color1.g = 0;
    color1.b = 1;

    color2.r = 0;
    color2.g = 0;
    color2.b = 1; 
    // start = 6;
    // if (start < 5)
    // {
    start = 2.7-(0.6*start);

    b_start = -2.6 + (0.6*b_start);
    b_end = 2.7-(0.6*b_end);

    b1_start = -2.6 + (0.6*b1_start);
    b1_end = 2.7-(0.6*b1_end);

    imp_start = -2.6 + (0.6*imp_start);
    imp_end = 2.7-(0.6*imp_end);

    c1_start = -2.6 + (0.6*c1_start);
    c1_end = 2.7-(0.6*c1_end);

    c2_start = -2.6 + (0.6*c2_start);
    c2_end = 2.7-(0.6*c2_end);

    bomb1_start = -2.6 + (0.6*bomb1_start);
    bomb1_end = 2.7-(0.6*bomb1_end);
    // }
    // else
    // {
    //     start = -0.7*(start-5);
    // }
    button = Button(b_start,b_end,color1);
    ball1 = Player(-2.6, start, COLOR_RED);
    imposter = Imposter(imp_start, imp_end, COLOR_RED, imp_flag);
    button1 = Button(b1_start,b1_end,color2);
    coin1 = Coin(c1_start,c1_end,color2);
    coin2 = Coin(c2_start,c2_end,color2);
    bomb1 = Bomb(bomb1_start,bomb1_end,color2);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int time_cnt = 0;
    int width  = 1000;
    int height = 1000;

    // maze size
    int maze_height = 10;
    int maze_width = 10;

    // GLTtext *text1 = gltCreateText();

    start = rand()%maze_height;
    finish = rand()%maze_height;

    float b_start = rand()%maze_height;
    float b_end = rand()%maze_height;

    float imp_start = rand()%maze_height;
    float imp_end = rand()%maze_height;

    float b1_start = rand()%maze_height;
    float b1_end = rand()%maze_height;

    float c1_start = rand()%maze_height;
    float c1_end = rand()%maze_height;

    float c2_start = rand()%maze_height;
    float c2_end = rand()%maze_height;

    float bomb1_start = rand()%maze_height;
    float bomb1_end = rand()%maze_height;
    // std::cout<<start<<" "<<finish<<std::endl;

    window = initGLFW(width, height);

    initGL (window, width, height, start, finish, b_start, b_end, imp_start, imp_end, b1_start, b1_end, c1_start, c1_end, bomb1_start, bomb1_end, c2_start, c2_end);

    if (!gltInit())
	{
		fprintf(stderr, "Failed to initialize glText\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

    GLTtext *text1 = gltCreateText();
    // gltSetText(text1, "Hello World!");
    GLTtext *text2 = gltCreateText();

    int time_flag=0;
    double time1;
    char str[30];
    double last_time=glfwGetTime();
    double prev_time=last_time;
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        // if(time_cnt<2000 and time_flag==0)
        //     time_cnt++;
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            time1 = glfwGetTime();
            draw();
            if((time1-last_time)>=100)
            {
                // reset_screen()
                reset_flag=1;;
                gltSetText(text2, "Time Over");
                gltBeginDraw();

                gltColor(1.0f, 1.0f, 1.0f, 1.0f);
                // gltDrawText2D(text1, 0.f, 0.0f, 1.0f); // x=0.0, y=0.0, scale=1.0

                gltDrawText2DAligned(text2,width*0.5,250,5.0f,GLT_CENTER, GLT_CENTER);
                gltEndDraw();
                // break;
            }
            else if(win_flag==1)
            {
                reset_flag=1;;
                gltSetText(text2, "You Won");
                gltBeginDraw();

                gltColor(1.0f, 1.0f, 1.0f, 1.0f);
                // gltDrawText2D(text1, 0.f, 0.0f, 1.0f); // x=0.0, y=0.0, scale=1.0

                gltDrawText2DAligned(text2,width*0.5,250,5.0f,GLT_CENTER, GLT_CENTER);
                gltEndDraw();
            }
            else if(game_flag==1)
            {
                reset_flag=1;;
                gltSetText(text2, "You Lose");
                gltBeginDraw();

                gltColor(1.0f, 1.0f, 1.0f, 1.0f);
                // gltDrawText2D(text1, 0.f, 0.0f, 1.0f); // x=0.0, y=0.0, scale=1.0

                gltDrawText2DAligned(text2,width*0.5,250,5.0f,GLT_CENTER, GLT_CENTER);
                gltEndDraw();
            }
            else
            {
                // light = "Off";
                // Swap Frame Buffer in double buffering
                sprintf(str, "Light: %d Time: %d Health: %d Task: %d/2",lights, int(time1), ball1.health, task);
                gltSetText(text1, str);

                gltBeginDraw();

                gltColor(1.0f, 1.0f, 1.0f, 1.0f);
                // gltDrawText2D(text1, 0.f, 0.0f, 1.0f); // x=0.0, y=0.0, scale=1.0

                gltDrawText2DAligned(text1,width*0.5,70,2.0f,GLT_CENTER, GLT_TOP);
                gltEndDraw();
                // tick_input(window,key);
                // glfwSwapBuffers(window);
                
                // tick_elements();
            }
            if(lights == 1)
            {
                ball1.health+=1;
            }
            lighting();
            if(time1-prev_time>1)
            {
                shortest_path();
                prev_time=time1;
            }
            glfwSwapBuffers(window);
            tick_elements();

            // std::cout<<ball1.position.x<<" "<<button.position.x<<" "<<ball1.position.y<<" "<<button.position.y<<std::endl;
            
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    gltTerminate();
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
