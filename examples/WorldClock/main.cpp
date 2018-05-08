#include <graph2d.h>

using namespace graph;

const char *g_zoneName[4] = {"London UK", "Beijing China", "New York USA", "Tokyo Japan"};
const int g_GMTOffset[4] = {0, 8, -4, 9};
double g_zonePosX[4][2], g_zonePosY[4][2];
double g_dx=0, g_dy=0;
int g_activeZone = 1;
int g_highlightZone = -1;
int g_imageID[6] = {-1};
int g_fontID[2] = {-1};

void display();
void keyboard(unsigned char key);
void mousePress(int message, int x, int y);
void mouseMove(int x, int y);

int main()
{
    g_imageID[0] = loadImage(".\\images\\london.bmp");
    g_imageID[1] = loadImage(".\\images\\beijing.bmp");
    g_imageID[2] = loadImage(".\\images\\newyork.bmp");
    g_imageID[3] = loadImage(".\\images\\tokyo.bmp");
    g_imageID[4] = loadImage(".\\images\\clockface.bmp");
    g_fontID[0] = createFont(36,"Arial", BOLD);
    g_fontID[1] = createFont(24,"Arial", BOLD);

    setBkColor(BLACK);
    setFullScreen();
    showHelp();
    initGraph(display, keyboard, mousePress, mouseMove);

    return 0;
}

void display()
{
    int fH, fW, len;
    int winWidth, winHeight;
    char strTimeText[256], strZoneName[256], strGMTOffset[256];
    double hArm = 100, mArm = 160, sArm = 180;
    double hAngle, mAngle, sAngle;
    SYSTEMTIME sys;

    GetLocalTime(&sys);

    sprintf(strTimeText, "%02d:%02d:%02d", (sys.wHour+g_GMTOffset[g_activeZone]-g_GMTOffset[1]+24)%24, sys.wMinute, sys.wSecond);
    sprintf(strZoneName, "%s", g_zoneName[g_activeZone]);
    sprintf(strGMTOffset, "(GMT%+d)", g_GMTOffset[g_activeZone]);

    winWidth = getWinWidth();
    winHeight = getWinHeight();

    for(int i=0; i<4; i++)
    {
        g_zonePosX[i][0] = winWidth/21+i*5*winWidth/21;
        g_zonePosX[i][1] = 5*winWidth/21+i*5*winWidth/21;
        g_zonePosY[i][0] = winHeight/21;
        g_zonePosY[i][1] = 5*winHeight/21;
    }

    for(int i=0; i<4; i++)
    {
        if (i==g_activeZone)
        {
            setColor(WHITE);
            bindImage(g_imageID[i], true);
        }
        else
        {
            setColor(128, 128, 128);
            bindImage(g_imageID[i], true);
        }
        fillRectangle(g_zonePosX[i][0], g_zonePosY[i][0], g_zonePosX[i][1], g_zonePosY[i][1]);
        setColor(WHITE);
        if (i==g_highlightZone)
            rectangle(g_zonePosX[i][0], g_zonePosY[i][0], g_zonePosX[i][1], g_zonePosY[i][1]);
    }

    bindImage(g_imageID[4]);
    fillCircle(winWidth/2+g_dx, winHeight/2+g_dy, sArm+20);

    setColor(ORANGE);
    fH = getFontHeight(g_fontID[0]);
    fW = getFontWidth(g_fontID[0]);
    useFont(g_fontID[0]);
    len = strlen(strZoneName);
    putText(winWidth/2-len/2*fW, 7*winHeight/8, strZoneName);
    len = strlen(strGMTOffset);
    putText(winWidth/2-len/2*fW, 7*winHeight/8-fH-20, strGMTOffset);

    setColor(ORANGE);
    fH = getFontHeight(g_fontID[1]);
    fW = getFontWidth(g_fontID[1]);
    useFont(g_fontID[1]);
    len = strlen(strTimeText);
    putText(winWidth/2-len/2*fW+g_dx, winHeight/2-fH/2-hArm+g_dy, strTimeText);

    sAngle = -2 * PI * (sys.wSecond+sys.wMilliseconds/1000.0) / 60.0 + PI/2.0;
    mAngle = -2 * PI * (sys.wMinute+sys.wSecond/60.0) / 60.0 + PI/2.0;
    hAngle = -2 * PI * ((sys.wHour+g_GMTOffset[g_activeZone]-g_GMTOffset[1])%12+sys.wMinute/60.0) / 12.0 + PI/2.0;

    setColor(104, 164, 238);
    setLineWidth(1);
    arrowLine(winWidth/2+g_dx, winHeight/2+g_dy,
              winWidth/2+sArm*cos(sAngle)+g_dx, winHeight/2+sArm*sin(sAngle)+g_dy);
    setColor(WHITE);
    setLineWidth(2);
    arrowLine(winWidth/2+g_dx, winHeight/2+g_dy,
              winWidth/2+mArm*cos(mAngle)+g_dx, winHeight/2+mArm*sin(mAngle)+g_dy);
    setLineWidth(3);
    arrowLine(winWidth/2+g_dx, winHeight/2+g_dy,
              winWidth/2+hArm*cos(hAngle)+g_dx, winHeight/2+hArm*sin(hAngle)+g_dy);
}

void keyboard(unsigned char key)
{
    switch (key)
    {
        case 'r':
            g_dx = 0;   g_dy = 0;
        case 'w':
            g_dy += 10;
        break;
        case 's':
            g_dy -= 10;
        break;
        case 'a':
            g_dx -= 10;
        break;
        case 'd':
            g_dx += 10;
        break;
    }
}

void mousePress(int message, int x, int y)
{
    int i;
    switch (message)
    {
        case LEFT_BUTTON_DOWN:
            for(i=0; i<4; i++)
            {
                if (x>g_zonePosX[i][0] && x<g_zonePosX[i][1] && y>g_zonePosY[i][0] && y<g_zonePosY[i][1])
                    g_activeZone = i;
            }
        break;
    }
}

void mouseMove(int x, int y)
{
    int i;
    for(i=0; i<4; i++)
    {
        if (x>g_zonePosX[i][0] && x<g_zonePosX[i][1] && y>g_zonePosY[i][0] && y<g_zonePosY[i][1])
        {
            g_highlightZone = i;
            break;
        }
    }
    if (i==4)
        g_highlightZone = -1;
}
