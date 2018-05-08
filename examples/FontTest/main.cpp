#include <graph2d.h>

using namespace graph;

int g_fontID[4];
int g_imgID;

void display()
{
    int w = getWinWidth(), h = getWinHeight();

    showImage(g_imgID);
    setColor(BROWN);
    useFont(g_fontID[0]);
    putText(w/32, 6*h/7, "The quick brown fox");
    setColor(BLACK);
    useFont(g_fontID[1]);
    putText(w/32, 5*h/7, "jumps over");
    setColor(ORANGE);
    useFont(g_fontID[2]);
    putText(w/32, 4*h/7, "the lazy dog.");
    setColor(BLACK);
    useFont(g_fontID[1]);
    putText(w/4, 3*h/7, "0123456789");
    useFont(g_fontID[3]);
    putText(2*w/3, 2*h/7, "ZZZZ");
}

int main()
{
    g_fontID[0] = createFont(36, "Arial", BOLD);
    g_fontID[1] = createFont(48, "Arial");
    g_fontID[2] = createFont(32, "Times New Roman", BOLD);
    g_fontID[3] = createFont(24, "Times New Roman", BOLD, true);
    g_imgID = loadImage(".\\images\\foxoverdog.bmp");
    setWinSize(600, 600);
    initGraph(display);
    return 0;
}
