#include "Tetromino.h"

//'I','J','L','O','S','T','Z'
CTetromino::CTetromino(char inType, int inImgID, float inSize, float initX, float initY)
{
    size = inSize;
    vxInit = initX;
    vyInit = initY;
    imgID = inImgID;
    type = inType;
    switch (type)
    {
        case 'I':
        case 'i':
            vx[0] = -2;     vy[0] = 0;
            vx[1] = -1;     vy[1] = 0;
            vx[2] = 0;     vy[2] = 0;
            vx[3] = 1;     vy[3] = 0;
        break;
        case 'J':
        case 'j':
            vx[0] = -1;      vy[0] = 0;
            vx[1] = 0;      vy[1] = 0;
            vx[2] = 1;      vy[2] = 0;
            vx[3] = -1;      vy[3] = 1;
        break;
        case 'L':
        case 'l':
            vx[0] = -1;       vy[0] = 0;
            vx[1] = 0;       vy[1] = 0;
            vx[2] = 1;       vy[2] = 0;
            vx[3] = 1;       vy[3] = 1;
            break;
        case 'O':
        case 'o':
            vx[0] = -1;       vy[0] = 0;
            vx[1] = 0;       vy[1] = 0;
            vx[2] = -1;       vy[2] = 1;
            vx[3] = 0;       vy[3] = 1;
        break;
        case 'S':
        case 's':
            vx[0] = -1;       vy[0] = 0;
            vx[1] = 0;       vy[1] = 0;
            vx[2] = 0;       vy[2] = 1;
            vx[3] = 1;       vy[3] = 1;
        break;
        case 'T':
        case 't':
            vx[0] = -1;       vy[0] = 0;
            vx[1] = 0;       vy[1] = 0;
            vx[2] = 1;       vy[2] = 0;
            vx[3] = 0;       vy[3] = 1;
        break;
        default:
            vx[0] = 0;       vy[0] = 0;
            vx[1] = 1;       vy[1] = 0;
            vx[2] = -1;       vy[2] = 1;
            vx[3] = 0;       vy[3] = 1;
        break;
    }
}

void CTetromino::Translate(float xOffset, float yOffset)
{
    vxInit = vxInit + xOffset*size;
    vyInit = vyInit + yOffset*size;
}

int CTetromino::IsInside(int x, int y)
{
    int id = -1;
    for(int i=0; i<4; i++)
    {
        if (x>=(vxInit+size*vx[i]) && x<=(vxInit+size*(vx[i]+1)) && y>=(vyInit+size*vy[i]) && y<=(vyInit+size*(vy[i]+1)))
            id = i;
    }
    return id;
}

void CTetromino::Draw()
{
    for(int i=0; i<4; i++)
    {
        bindImage(imgID);
        fillRectangle(vxInit+size*vx[i], vyInit+size*vy[i], vxInit+size*(vx[i]+1), vyInit+size*(vy[i]+1));
    }
}

void CTetromino::DrawFrame()
{
    setColor(RED);
    for(int i=0; i<4; i++)
        rectangle(vxInit+size*vx[i], vyInit+size*vy[i], vxInit+size*(vx[i]+1), vyInit+size*(vy[i]+1));
}
