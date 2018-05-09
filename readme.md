//Graph2D is a simple 2D graph library to assist programmers to develop simple graphic interface using C++.

//Here is an example code of drawing a circle with text using Graph2D.

#include <Graph2D.h>
using namespace graph;

void display() 
{
    circle(512, 384, 100);
    putText(480, 384, "Hello world!");
}

int main(int argc, char *argv[])
{
    initGraph(display);
    return 0;
}

[image](images/simpleGraphWin.jpg)
