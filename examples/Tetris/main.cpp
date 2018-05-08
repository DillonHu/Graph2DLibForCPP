#include "Tetromino.h"

CTetromino tetrObj[7] = { CTetromino('I', 0, 30, 100, 500), CTetromino('J', 1, 30, 220, 500),
                                        CTetromino('L', 2, 30, 340, 500), CTetromino('O', 3, 30, 460, 500),
                                        CTetromino('S', 4, 30, 580, 500), CTetromino('T', 5, 30, 700, 500),
                                        CTetromino('Z', 6, 30, 820, 500)};
CTetromino *choosedObj = &tetrObj[0];

void display(void)
{
    for (int i=0; i<7; i++)
        tetrObj[i].Draw();
    choosedObj->DrawFrame();
}

void keyboard(unsigned char key)
{
    switch (key)
    {
        case 'a':
            choosedObj->Translate(-1, 0);
            break;
        case 'd':
            choosedObj->Translate(1, 0);
            break;
        case 'w':
            choosedObj->Translate(0, 1);
            break;
        case 's':
            choosedObj->Translate(0, -1);
            break;
        default:
            break;
    }
}

void mousePress(int message, int x, int y)
{
    if (message==LEFT_BUTTON_DOWN)
    {
        for (int i=0; i<7; i++)
            if (-1!=tetrObj[i].IsInside(x, y))
                choosedObj = &tetrObj[i];
    }
}

int main(int argc, char *argv[])
{
    char imgName[][32] = {".\\images\\1.bmp", ".\\images\\2.bmp", ".\\images\\3.bmp",
                                          ".\\images\\4.bmp",".\\images\\5.bmp", ".\\images\\6.bmp",
                                          ".\\images\\7.bmp"};

    for (int i=0; i<7; i++)
        loadImage(imgName[i]);

    initGraph(display, keyboard, mousePress);
    return 0;
}
