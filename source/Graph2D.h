//Graph2D简易图形库V1.0
//作者：胡少军，耿楠，蔡骋，魏蕾，耿耀君
//2015年3月
/*Graph2D参考Borland C++中graphics库，通过名字空间(namespace)封装OpenGL中的图形初始化、
基本图元绘制、键盘和鼠标交互等函数，为用户提供一个简易的开发界面与接口，图元绘制函数的
命名多沿袭传统graphics库的命名。Graph2D包含图形初始化、窗口设置、基本图元绘制、字体创
建与文本显示、图像读入与显示、键盘鼠标交互等功能模块。
坐标系统采用二维笛卡尔右手坐标系，其中X轴指向右方，Y轴指向上方*/
#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>

#ifndef GRAPH_2D_H
#define GRAPH_2D_H

#if !defined(PI)
#define PI 3.1415926535897932384626433832795
#endif

#if !defined(DEG_TO_RAD)
#define DEG_TO_RAD       PI/180.0
#endif

#if !defined(GL_BGR_EXT)
#define GL_BGR_EXT       0x80E0
#endif

#if !defined(GL_BGRA_EXT)
#define GL_BGRA_EXT      0x80E1
#endif

#if !defined(GL_CLAMP_TO_EDGE)
#define GL_CLAMP_TO_EDGE 0x812F
#endif

namespace graph
{
    const unsigned long BLACK = 0x0;
    const unsigned long RED = 0xFF0000;
    const unsigned long GREEN = 0x00FF00;
    const unsigned long BLUE = 0x0000FF;
    const unsigned long GREY = 0x808080;
    const unsigned long PURPLE = 0x800080;
    const unsigned long WHITE = 0xFFFFFF;
    const unsigned long CYAN = 0x00FFFF;
    const unsigned long MAGENTA = 0xFF00FF;
    const unsigned long YELLOW = 0xFFFF00;
    const unsigned long ORANGE = 0xFFA500;
    const unsigned long BROWN = 0xA52A2A;

    const int LEFT_BUTTON_DOWN = 1001;
    const int LEFT_BUTTON_UP = 1002;
    const int RIGHT_BUTTON_DOWN = 1003;
    const int RIGHT_BUTTON_UP = 1004;
    const int MIDDLE_BUTTON_DOWN = 1005;
    const int MIDDLE_BUTTON_UP = 1006;
    const int KEY_LEFT = GLUT_KEY_LEFT;
    const int KEY_RIGHT = GLUT_KEY_RIGHT;
    const int KEY_UP = GLUT_KEY_UP;
    const int KEY_DOWN = GLUT_KEY_DOWN;

    const int BOLD = FW_BOLD;

    const int MAX_NUM_TEXS = 512;
    const int MAX_NUM_FONTS = 32;
    const double ANGLE_STEP = PI/180.0;

    extern int m_fps;
    extern int m_preTime;

    extern bool m_bInitGraphDone;
    extern bool m_bFullScreen;
    extern bool m_bBindImage;
    extern bool m_bHelpEnabled;
    extern bool m_bShowCoordinate;

    extern int m_numTexs;
    struct ImageData
    {
        void *pImage;
        int width;
        int height;
        GLint component;
        GLenum format;
        GLuint texID;
    };
    extern struct ImageData m_texture[MAX_NUM_TEXS];
    extern double m_texPosU[2];
    extern double m_texPosV[2];

    extern int m_winXPos;
    extern int m_winYPos;
    extern int m_winWidth;
    extern int m_winHeight;
    extern double m_bgColor[3];
    extern double m_color[3];

    extern char m_strWinTitle[];

    struct FontData
    {
        GLint fontList;
        int width;
        int height;
        HFONT hFont;
    };
    extern struct FontData m_font[MAX_NUM_FONTS];
    extern int m_numFonts;
    extern int m_nFontID;
    extern int m_nHelpFontID;

    extern void (*inDisplay)(void);
    extern void (*inKeyboard)(unsigned char key);
    extern void (*inMousePress)(int message, int x, int y);
    extern void (*inMouseMove)(int x, int y);
    extern void (*inSpecialKeyboard)(int key);
    void displayCK();
    void specialKeyboardCK(int key, int x, int y);
    void keyboardCK(unsigned char key, int x, int y);
    void mousePressCK(int button, int state, int x, int y);
    void mouseMoveCK(int x, int y);

    void idleCK(void);
    void reshapeCK(int w, int h);

    void scanLineFilling(int numPoints,double polyX[], double polyY[]);
    GLbyte *loadBMP(const char *fileName, int &width, int &height,
                           GLint &component, GLenum &format, bool bGrey);

    //一、图形初始化
    /*图形系统初始化函数，用于构建图形窗口，嵌入回调函数
    参数与返回值：函数包含5个函数指针，分别对应图形窗口绘制与更新函数display，键盘响应函数keyboard，
    鼠标按键响应函数mousePress，鼠标移动响应函数mouseMove和键盘特殊键响应函数specialKeyboard函数。
    说明：该图形函数中的第1个参数不能缺省，后面4个参数均可缺省参数，因此用户至少要定义一个用于图形显示与更新的display函数。*/
    void initGraph(void (*display)(),
                   void (*keyboard)(unsigned char key)=NULL,
                   void (*mousePress)(int message, int x, int y)=NULL,
                   void (*mouseMove)(int x, int y)=NULL,
                   void (*specialKeyboard)(int key)=NULL);

    //二、窗口设置
    /*设置窗口背景颜色
    参数与返回值：两个重载函数均可设置窗口背景颜色，其中第一个函数中对应的参数为红绿蓝三色分量，取值范围为[0, 255]；
    第二个函数对应参数可用一个16进制宏表示，例如红色RED对应0XFF0000，
    其余类似的宏还包括BLACK、GREEN、BLUE、GREY、PURPLE、WHITE、CYAN、MAGENTA、YELLOW、ORANGE和BROWN。
    说明：一般在initGraph前调用。*/
    void setBkColor(unsigned char red, unsigned char green, unsigned char blue);
    void setBkColor(unsigned long color);
    /*设置窗口标题
    参数与返回值：默认窗口标题为“Simple Graph Window”，通过strTitle字符数组指定新的窗口标题。
    说明：一般在initGraph前调用。*/
    void setWinTitle(char strTitle[]);
    /*设置窗口大小
    参数与返回值：默认窗口大小为1024*768，通过width和height分别设置新窗口的宽度与高度。
    说明：一般在initGraph前调用。*/
    void setWinSize(int width, int height);
    /*获取窗口宽度与高度
    参数与返回值：返回当前窗口宽度。
    说明：无调用限制。*/
    int getWinWidth();
    int getWinHeight();
    /*用背景颜色清屏
    参数与返回值：无。
    说明：在display函数中调用后，前面绘制的所有图形将被清除。*/
    void clearWindow();
    /*设置全屏模式
    参数与返回值：无。
    说明：进入全屏模式，一般在initGraph前调用。*/
    void setFullScreen();
    /*退出全屏，切换到窗口模式
    参数与返回值：无。
    说明：退出全屏模式，一般在keyboard回调函数中调用。*/
    void exitFullScreen();

    //三、基本图元绘制
    /*设置线宽
    参数与返回值：设置当前线型为虚线。
    说明：在display中调用*/
    void setLineWidth(int width);
    /*设置线型为虚线
    参数与返回值：无。
    说明：在display中调用。*/
    void setDottedLines();
    /*设置线型为实线
    参数与返回值：无。
    说明：设置当前线型为实线，默认线型为实线，该函数在display中调用。*/
    void setSolidLines();
    /*设置图元颜色
    参数与返回值：与setBkColor类似，两个重载函数均可设置图元绘制颜色，其中第1个函数中对应的3个参数为红绿蓝3色分量，取值范围为[0, 255]；第2个函数对应参数可用一个24位的16进制数表示，如0XFFFF00对应为橙色。
    说明：该函数可设置点、线、基本形状、文本以及与图像的混合色，在display中调用。*/
    void setColor(unsigned char red, unsigned char green, unsigned char blue);
    void setColor(unsigned long color);
    /*设置线宽
    参数与返回值：设置点的大小，默认情况下点的大小为1。
    说明：在display中调用。*/
    void setPointSize(double size);
    /*显示像素点
    参数与返回值：在坐标(x, y)处显示一个像素点。
    说明：在display中调用。*/
    void putPixel(double x, double y);
    /*获取像素点
    参数与返回值：获取坐标(x, y)处像素点的颜色。
    说明：在display中调用。*/
    void getPixel(int x, int y, unsigned char &red, unsigned char &green, unsigned char &blue);
    /*绘制线段
    参数与返回值：startX和startY对应线段起点，endX和endY对应线段终点。
    说明：在display中调用。*/
    void line(double startX, double startY, double endX, double endY);
    /*绘制带箭头线段
    参数与返回值：startX和startY对应线段起点，endX和endY对应线段终点，s1位箭头宽度，s2为箭头长度。
    说明：在display中调用。*/
    void arrowLine(double startX, double startY, double endX, double endY, double s1=5.0, double s2=10.0);
    /*绘制矩形线框
    参数与返回值：x0和y0对应矩形左下角坐标，x1和y1对应右上角坐标。
    说明：在display中调用。*/
    void rectangle(double x0, double y0, double x1, double y1);
    /*绘制填充矩形
    参数与返回值：x0和y0对应矩形左下角坐标，x1和y1对应右上角坐标。
    说明：在display中调用。*/
    void fillRectangle(double x0, double y0, double x1, double y1);
    /*绘制三角形
    参数与返回值：(x0, y0)、(x1, y1)和(x2, y2)分别对应三角形的三个顶点坐标。
    说明：在display中调用。*/
    void triangle(double x0, double y0, double x1, double y1, double x2, double y2);
    /*绘制填充三角形
    参数与返回值：(x0, y0)、(x1, y1)和(x2, y2)分别对应三角形的三个顶点坐标。
    说明：在display中调用。*/
    void fillTriangle(double x0, double y0, double x1, double y1, double x2, double y2);
    /*绘制圆
    参数与返回值：xcr和ycr为圆心坐标，radius为半径。
    说明：在display中调用。*/
    void circle(double xctr, double yctr, double radius);
    /*绘制填充圆
    参数与返回值：xctr和yctr为圆心坐标，radius为半径。
    说明：在display中调用。*/
    void fillCircle(double xctr, double yctr, double radius);
    /*绘制椭圆
    参数与返回值：xctr和yctr为椭圆圆心坐标，xRadius为x方向轴半径，yRadius为y方向轴半径。
    说明：在display中调用。*/
    void ellipse(double xctr, double yctr, double xRadius, double yRadius);
    /*绘制填充椭圆
    参数与返回值：xctr和yctr为椭圆圆心坐标，xRadius为x方向轴半径，yRadius为y方向轴半径。
    说明：在display中调用。*/
    void fillEllipse(double xctr, double yctr, double xRadius, double yRadius);
    /*绘制椭圆环
    参数与返回值：xctr和yctr为圆心坐标，xRadius为x方向轴半径，yRadius为y方向轴半径，ratio为内圆半径与外圆半径的比例。
    说明：在display中调用。*/
    void donut(double xctr, double yctr, double xRadius, double yRadius, double ratio);
    /*绘制填充椭圆环
    参数与返回值：xctr和yctr为圆心坐标，xRadius为x方向轴半径，yRadius为y方向轴半径，ratio为内圆半径与外圆半径的比例。
    说明：在display中调用。*/
    void fillDonut(double xctr, double yctr, double xRadius, double yRadius, double ratio);
    /*绘制圆弧
    参数与返回值：cx和cy为圆心坐标，radius为半径，startAngle和endAngle对应圆弧的起始角度。
    说明：在display中调用。*/
    void arc(double xctr, double yctr, double radius, double startAngle, double endAngle);
    /*绘制扇形
    参数与返回值：cx和cy为圆心坐标，radius为半径，startAngle和endAngle对应圆弧的起始角度。
    说明：在display中调用。*/
    void sector(double xctr, double yctr, double radius,double startAngle, double endAngle);
    /*绘制多边形
    参数与返回值：numPoints为多边形顶点数，x[]和y[]是用于存储多边形顶点x坐标和y坐标的数组。
    说明：在display中调用。*/
    void polygon(int numPoints, double x[], double y[]);
    /*绘制填充多边形
    参数与返回值：numPoints为多边形顶点数，x[]和y[]是用于存储多边形顶点x坐标和y坐标的数组，
    默认情况下bConvex为真，即绘制凸多边形，若为凹多边形，则将bConvex设为假，采用扫描线算法填充。
    说明：在display中调用，对于凹多边形的填充，暂不支持图像邦定功能。*/
    void fillPolygon(int numPoints, double x[], double y[], bool bConvex=true);

    //四、文本设置与显示
    /*设置字号与字体
    参数与返回值：pixHeight为字体高度（单位为像素），默认高度为24；
    style为字体，默认为“Arial”，也可设为“Times New Roman”、“宋体”、“黑体”等；
    weight为字体粗度，默认为正常粗度，也可设为加粗（对应宏为BOLD）；
    bItalic为倾斜模式，默认为非斜体。函数将返回创建字体标记号，若创建失败则返回-1。
    说明：在initGraph前调用，暂不支持汉字显示。*/
    int createFont(int pixHeight=24, const char style[]="Arial",
                   int weight=FW_NORMAL, bool bItalic=false);
    /*使用指定字体
    参数与返回值：fontID对应createFont中创建返回的字体标记号。
    说明：在display中调用。*/
    void useFont(int fontID);
    /*获取指定字体高度
    参数与返回值：返回fontID对应字体高度。
    说明：在display中调用。*/
    int getFontHeight(int fontID);
    /*获取指定字体宽度
    参数与返回值：返回fontID对应字体宽度。
    说明：在display中调用。*/
    int getFontWidth(int fontID);
    /*显示文本
    参数与返回值：在屏幕坐标(x, y)处显示字符串strText。
    说明：在display中调用，若不预先调用createFont和useFont函数，将使用默认“Arial”24号字体。*/
    void putText(int x, int y, const char strText[]);

    //五、图像载入与填充
    /*载入图像
    参数与返回值：读入fileName对应的BMP真彩色图像文件，bGrey若为真值，则将读入图像转化为灰度图像，否则为原图像。
    该函数返回读取图像的标记号，若图像读取失败，返回-1。
    说明：在initGraph前调用，暂不支持真彩色BMP格式以外图像文件的读写。*/
    int loadImage(const char *fileName, bool bGrey=false);
    /*显示指定图像
    参数与返回值：在屏幕窗口上显示标记号为imageID的图像，
    startU、startV、endU、endV分别指定从图像中截取矩形区域子图像的左下角和右上角坐标，
    坐标范围量化为[0, 1]，默认情况下显示整幅图像。
    说明：在display中调用。*/
    void showImage(int imageID, double startU=0.0, double startV=0.0, double endU=1.0, double endV=1.0);
    /*用指定图像填充基本图元
    参数与返回值：使用标记号为imageID的图像填充后续图像，如果bBlend为真，
    则图像将与setColor指定颜色混合后再填充后续图元对象（如矩形、圆、凸多边形等），
    默然情况下图像不与设定颜色混合；startU、startV、endU、endV分别指定从图像中截取矩形区域子图像的左下角和右上角坐标，
    坐标范围量化为[0, 1]，默认情况下获取的是整幅图像。
    说明：在display中调用。*/
    void bindImage(int imageID, bool bBlend=false,
                                double startU=0.0, double startV=0.0, double endU=1.0, double endV=1.0);
    // 六、其它
    /*设置帧速率
    参数与返回值：设置屏幕更新频率为framePerSec，默然帧速率为每秒40帧。
    说明：在initGraph前调用，如果图像绘制过于复杂，即使设置高帧速率也无法提高画面更新速度。*/
    void setFPS(int fps);
    /*获取帧速率
    参数与返回值：返回当前帧速率。
    说明：在display中调用。*/
    int getFPS();
    /*显示帮助信息
    参数与返回值：无。
    说明：在initGraph前调用，按F1键将隐藏该帮助信息。*/
    void showHelp();
    /*显示坐标系
    参数与返回值：无。
    说明：在display中调用，显示当前坐标系，默认情况下不显示，按F2键将调用该函数。*/
    void showCoordinate();
    /*保存屏幕图像至文件
    参数与返回值：实现抓屏功能，保存屏幕图像到BMP图像文件fileName，fileName后缀名为bmp。
    说明：在keyboard回调函数中使用。*/
    bool saveScreenImage(char *fileName);
}
#endif
