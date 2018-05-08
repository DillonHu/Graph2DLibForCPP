#include <graph2d.h>

using namespace graph;

int g_imgID;

void display()
{
    setColor(ORANGE);
    bindImage(g_imgID, true);
    fillCircle(getWinWidth()/2, getWinHeight()/2, 300);
}

void keyboard(unsigned char key)
{
    if (key=='s' || key=='S')
        saveScreenImage(".\\images\\londonCopy.bmp");
}

int main()
{
    g_imgID = loadImage(".\\images\\london.bmp");
    initGraph(display, keyboard);
    return 0;
}
