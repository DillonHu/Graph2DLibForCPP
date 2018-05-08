#ifndef TETROMINO_H_INCLUDED
#define TETROMINO_H_INCLUDED

#include <graph2d.h>
using namespace graph;

class CTetromino
{
    int imgID;
    float vxInit, vyInit;
    char type;
    float vx[4], vy[4];
    float size;
public:
    CTetromino(char inType='I', int inImgID=0, float inSize=40, float initX=300, float initY=400);
    void Translate(float xOffset, float yOffset);
    int IsInside(int x, int y);
    void Draw();
    void DrawFrame();
};

#endif // TETROMINO_H_INCLUDED
