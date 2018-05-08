#include <graph2d.h>

using namespace graph;

class CPendulum
{
    double L;
    double g;
    double theta0;
    double x, y;
    double r;
    double T;
    double theta;
public:
    CPendulum(double L=1.0, double g=9.8, double theta0 = PI/3.0, double x=512.0, double y=700.0, double r = 20.0)
    {
        this->L = L;
        this->g = g;
        this->theta0 = theta0;
        theta = theta0;
        this->x = x;
        this->y = y;
        this->r = r;
        T = 2*PI*sqrt(this->L/this->g);
    }
    void UpdateAngle(double t)
    {
        theta  = theta0*cos(2*PI*t/T);
    }
    double GetTheta()
    {
        return theta;
    }
    double GetT()
    {
        return T;
    }
    void Draw()
    {
        double scale = 500;
        setColor(BLACK);
        fillRectangle(x-50, y-5, x+50, y+5);
        setLineWidth(4);
        setColor(RED);
        line(x, y, x+L*scale*sin(theta), y-L*scale*cos(theta));
        setColor(BLUE);
        fillCircle(x+L*scale*sin(theta), y-L*scale*cos(theta), r);
    }
};

CPendulum myPendulum;

void display()
{
    static int startT = GetTickCount();
    int currT = GetTickCount()-startT;
    char strInfo[512];

    myPendulum.UpdateAngle(currT/1000.0);
    myPendulum.Draw();

    sprintf(strInfo, "T=%f\tt=%f\tTheta=%f", myPendulum.GetT(), currT/1000.0, myPendulum.GetTheta());
    putText(100, 100, strInfo);
}

int main()
{
    initGraph(display);
    return 0;
}
