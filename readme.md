Graph2D is a 2D graph library to assist programmers to develp simple graphic interface using C++.

The basic circle drawing and text displaying code is very simple using the Graph2D library:

#include <Graph2D.h>
using namespace graph;

void display() {
    circle(512, 384, 100);
    putText(480, 384, "Hello world!");
}

int main(int argc, char *argv[]) {
    initGraph(display);
    return 0;
}
