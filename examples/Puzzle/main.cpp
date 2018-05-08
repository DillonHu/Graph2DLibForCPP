#include <graph2d.h>

using namespace graph;

int g_imgID = -1;
int g_empty = 3;
int g_arr[4] = {2,3,0,1};
double g_pos[4][4] = {{0,0,0.5,0.5},{0.5,0,1,0.5},{0.5,0.5,1,1},{0,0.5,0.5,1}};
int g_w, g_h;

void display()
{
    g_w = getWinWidth();
    g_h = getWinHeight();

    for(int i=0; i<4; i++)
    {
        if (g_arr[i]==g_empty) setColor(GREY);
        else  setColor(WHITE);
        bindImage(g_imgID,true,g_pos[g_arr[i]][0], g_pos[g_arr[i]][1],
                          g_pos[g_arr[i]][2], g_pos[g_arr[i]][3]);
        fillRectangle(g_pos[i][0]*g_w, g_pos[i][1]*g_h, g_pos[i][2]*g_w, g_pos[i][3]*g_h);
    }
}

void mousePress(int message, int x, int y)
{
    int i, j, k=-1;
    if (message==LEFT_BUTTON_DOWN)
    {
        for(i=0; i<4; i++)
            if(g_pos[i][0]*g_w<=x&&x<=g_pos[i][2]*g_w&&g_pos[i][1]*g_h<=y&&y<=g_pos[i][3]*g_h)
                break;
        if (i!=4)
        {
            j = i-1;
            if (j<0) j=j+4;
            if (g_arr[j]==g_empty)  k = j;
            j = i+1;
            if (j>=4) j=j-4;
            if (g_arr[j]==g_empty)  k = j;
        }
    }
    if (k!=-1)
    {
        j = g_arr[k];
        g_arr[k] = g_arr[i];
        g_arr[i] = j;
        for(i=0; i<4; i++)
            if (g_arr[i]!=i) break;
        if (i==4) printf("Æ´Í¼Íê³É\n");
    }
}

int main(int argc, char *argv[])
{
    g_imgID = loadImage(".\\images\\london.bmp");
    initGraph(display,NULL,mousePress);
    return 0;
}
