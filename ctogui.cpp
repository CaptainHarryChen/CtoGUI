#include"ctogui.h"
#include<cstdio>
#include<map>

namespace OpenGL
{
void Display();
void Idle();
void Reshape(int new_w, int new_h);
void Timer(int id);
void OnKeyBoard(unsigned char key, int x, int y);
void OnMouse(int button, int state, int x, int y);
void OnSpecialKey(int key, int x,int y);
GLint LoadTexture(const char *file_name);

void (*UpdateScreen)(void) = NULL;
void (*KeyProcess)(unsigned char, int, int) = NULL;
void (*SpecialProcess)(int, int, int) = NULL;
void (*MouseProcess)(int, int, int, int);

bool timer_setted = false;
std::map<int,GLint> textures;
CtoGui::scene nowsce;
}

void OpenGL::Display()
{
    using namespace CtoGui;

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, nowsce.width, nowsce.height, 0);

    nowsce.Draw();

    glutSwapBuffers();
}

void OpenGL::Idle()
{
    using namespace CtoGui;

    if(timer_setted == false)
    {
        if(UpdateScreen != NULL)
            (*UpdateScreen)();
        Display();
    }
}

void OpenGL::Reshape(int new_w, int new_h)
{
    using namespace CtoGui;
    win_width = new_w;
    win_height = new_h;

    nowsce.Update();
}

void OpenGL::Timer(int id)
{
    using namespace CtoGui;

    switch(id)
    {
    case TIMER_UPDATE:
        if(UpdateScreen != NULL)
            (*UpdateScreen)();
        Display();
        glutTimerFunc(time_to_update, &Timer, TIMER_UPDATE);
        break;
    default:
        break;
    }
}

void OpenGL::OnKeyBoard(unsigned char key, int x, int y)
{
    int tx, ty;
    CtoGui::ChangeXY(x, y, tx, ty);
    if(KeyProcess != NULL)
        KeyProcess(key, ty, tx);
}

void OpenGL::OnSpecialKey(int key, int x,int y)
{
    int tx, ty;
    CtoGui::ChangeXY(x, y, tx, ty);
    if(SpecialProcess != NULL)
        SpecialProcess(key, ty, tx);
}

void OpenGL::OnMouse(int button, int state, int x, int y)
{
    int tx, ty;
    CtoGui::ChangeXY(x, y, tx, ty);
    if(MouseProcess != NULL)
        MouseProcess(button, state, ty, tx);
}

void CtoGui::Init(int *argc, char **argv, int height, int width, const char *title)
{
    using namespace OpenGL;

    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    glutInitWindowPosition((cx - width) / 2, (cy - height) / 2);

    win_height = height;
    win_width = width;
    glutInitWindowSize(width, height);

    glutCreateWindow(title);

    glutDisplayFunc(&Display);
    glutIdleFunc(&Idle);
    glutKeyboardFunc(&OnKeyBoard);
    glutSpecialFunc(&OnSpecialKey);
    glutMouseFunc(&OnMouse);
}

void CtoGui::SetImg(int id, const char *bpath)
{
    OpenGL::textures[id] = OpenGL::LoadTexture(bpath);
}

void CtoGui::SetUpdateTimer(int tim)
{
    OpenGL::timer_setted = true;
    time_to_update = tim;
    glutTimerFunc(time_to_update, &OpenGL::Timer, TIMER_UPDATE);
}

void CtoGui::SetUpdateScreenFunc(void (*p)())
{
    OpenGL::UpdateScreen = p;
}

void CtoGui::SetKeyFunc(void(*p)(unsigned char, int, int))
{
    OpenGL::KeyProcess = p;
}

void CtoGui::SetSpecialFunc(void(*p)(int, int, int))
{
    OpenGL::SpecialProcess = p;
}

void CtoGui::SetMouseFunc(void(*p)(int, int, int, int))
{
    OpenGL::MouseProcess = p;
}

void CtoGui::BeginPlay()
{
    glutMainLoop();
}

void CtoGui::DrawScreen(const scene &draw_scene)
{
    OpenGL::nowsce = draw_scene;
}

GLint OpenGL::LoadTexture(const char *file_name)
{
    using namespace CtoGui;

    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLint last_texture_ID;
    GLuint texture_ID = 0;

    FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 )
        return 0;

    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, 54, SEEK_SET);

    GLint line_bytes = width * 3;
    while( line_bytes % 4 != 0 )
        ++line_bytes;
    total_bytes = line_bytes * height;

    pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 )
    {
        fclose(pFile);
        return 0;
    }

    if( fread(pixels, total_bytes, 1, pFile) <= 0 )
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    GLint mx;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mx);
    if(!((width & (width - 1)) != 0)
            || !((width & (height - 1)) != 0)
            || width > mx
            || height > mx )
    {
        const GLint new_width = 256;
        const GLint new_height = 256;
        GLint new_line_bytes, new_total_bytes;
        GLubyte* new_pixels = 0;

        new_line_bytes = new_width * 3;
        while( new_line_bytes % 4 != 0 )
            ++new_line_bytes;
        new_total_bytes = new_line_bytes * new_height;

        new_pixels = (GLubyte*)malloc(new_total_bytes);
        if( new_pixels == 0 )
        {
            free(pixels);
            fclose(pFile);
            return 0;
        }

        gluScaleImage(GL_RGB,
                      width, height, GL_UNSIGNED_BYTE, pixels,
                      new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

        free(pixels);
        pixels = new_pixels;
        width = new_width;
        height = new_height;
    }

    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 )
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, last_texture_ID);

    free(pixels);
    return texture_ID;
}

void CtoGui::ChangeXY(int x, int y, int &resx, int &resy)
{
    resx = x / OpenGL::nowsce.img_width;
    resy = y / OpenGL::nowsce.img_height;
}

CtoGui::scene::scene()
{
    buf = NULL;

    height = -1;
    width = -1;

    img_height = -1;
    img_width = -1;
}

CtoGui::scene::scene(int *tbuf, const int theight, const int twidth)
{
    buf = tbuf;

    height = theight;
    width = twidth;

    img_height = win_height / height;
    img_width = win_width / width;
}

CtoGui::scene::~scene()
{
}

void CtoGui::scene::Update()
{
    img_height = win_height / height;
    img_width = win_width / width;
}

void CtoGui::scene::Draw()
{
    using namespace OpenGL;

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            int pos = i * width + j;
            if(textures[buf[pos]] != 0)
            {
                glBindTexture(GL_TEXTURE_2D, textures[buf[pos]]);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2d(j + 1, i);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2d(j + 1, i + 1);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2d(j, i + 1);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2d(j + 1, i + 1);
                glEnd();
            }
        }
}
