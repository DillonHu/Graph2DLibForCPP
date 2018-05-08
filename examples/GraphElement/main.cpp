#include <graph2d.h>

using namespace graph;

void display()
{
    int w = getWinWidth(),   h = getWinHeight(), i;
    double dx = 1.0/3, dy = 1.0/3;
    const int n = 10;
    double poly[n][2] = {{0, 0.61},{0.25, 0.36},{0.14, 0},{0.46, 0.18},{0.86, 0},
                                      {0.75, 0.36}, {1, 0.61}, {0.64, 0.64}, {0.46, 1.0},{0.36, 0.64}};
    double tmpPolyX[n], tmpPolyY[n];

    setColor(0, 128, 255);
    for(i=0; i<n; i++)
        putPixel((0.2+0.3*poly[i][0])*dx*w, (1.3+0.4*poly[i][1])*dy*h);

    setColor(BLACK);
    line(0.2*dx*w, 0.8*dy*h, 0.8*dx*w, 0.8*dy*h);
    setLineWidth(4);
    line(0.2*dx*w, 0.6*dy*h, 0.8*dx*w, 0.6*dy*h);
    setDottedLines();
    line(0.2*dx*w, 0.4*dy*h, 0.8*dx*w, 0.4*dy*h);
    setLineWidth(1);
    setColor(RED);
    setSolidLines();
    arrowLine(0.2*dx*w, 0.2*dy*h, 0.8*dx*w, 0.2*dy*h);

    setColor(BLACK);
    rectangle(1.1*dx*w, 0.2*dy*h, 1.4*dx*w, 0.8*dy*h);
    setColor(BROWN);
    fillRectangle(1.6*dx*w, 0.2*dy*h, 1.9*dx*w, 0.8*dy*h);

    setColor(BLACK);
    circle(2.25*dx*w, 0.5*dy*h, 0.2*dx*w);
    setColor(PURPLE);
    fillCircle(2.75*dx*w, 0.5*dy*h, 0.2*dx*w);

    setColor(BLACK);
    ellipse(2.25*dx*w, 1.5*dy*h, 0.15*dx*w, 0.2*dx*w);
    setColor(BROWN);
    fillEllipse(2.75*dx*w, 1.5*dy*h, 0.2*dx*w, 0.15*dx*w);

    setColor(BLACK);
    donut(2.25*dx*w, 2.5*dy*h, 0.15*dx*w, 0.2*dx*w, 0.5);
    setColor(PURPLE);
    fillDonut(2.75*dx*w, 2.5*dy*h, 0.2*dx*w, 0.2*dx*w, 0.3);

    setColor(BLACK);
    triangle(1.1*dx*w, 2.1*dy*h, 1.4*dx*w, 2.1*dy*h, 1.25*dx*w, 2.9*dy*h);
    setColor(BROWN);
    fillTriangle(1.6*dx*w, 2.9*dy*h, 1.9*dx*w, 2.9*dy*h, 1.75*dx*w, 2.1*dy*h);

    setColor(BLACK);
    arc(0.25*dx*w, 2.5*dy*h, 0.2*dx*w, 30, 240);
    setColor(PURPLE);
    sector(0.75*dx*w, 2.5*dy*h, 0.2*dx*w, 30, 240);

    for(i=0; i<n; i++)
    {
        tmpPolyX[i] = (0.6+0.3*poly[i][0])*dx*w;
        tmpPolyY[i] = (1.3+0.4*poly[i][1])*dy*h;
    }
    setColor(BLACK);
    polygon(n, tmpPolyX, tmpPolyY);

    for(i=0; i<n/2; i++)
    {
        tmpPolyX[i] = (1.2+0.3*poly[2*i][0])*dx*w;
        tmpPolyY[i] = (1.3+0.4*poly[2*i][1])*dy*h;
    }
    setColor(PURPLE);
    fillPolygon(n/2, tmpPolyX, tmpPolyY);

    for(i=0; i<n; i++)
    {
        tmpPolyX[i] = (1.6+0.3*poly[i][0])*dx*w;
        tmpPolyY[i] = (1.3+0.4*poly[i][1])*dy*h;
    }
    setColor(BROWN);
    fillPolygon(n, tmpPolyX, tmpPolyY, false);
}

int main()
{
    initGraph(display);
    return 0;
}
