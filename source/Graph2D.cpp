#include "Graph2D.h"

namespace graph
{
    int m_fps = 40;
    int m_preTime;

    bool m_bInitGraphDone = false;
    bool m_bFullScreen = false;
    bool m_bBindImage = false;
    bool m_bHelpEnabled = false;
    bool m_bShowCoordinate = false;

    int m_numTexs = 0;

    struct ImageData m_texture[MAX_NUM_TEXS];
    double m_texPosU[2] = {0.0, 1.0};
    double m_texPosV[2] = {0.0, 1.0};

    int m_winXPos = 100;
    int m_winYPos = 100;
    int m_winWidth = 1024;
    int m_winHeight = 768;
    double m_bgColor[3] = {1.0, 1.0, 1.0};
    double m_color[3] = {0.0, 0.0, 0.0};

    char m_strWinTitle[] = "Simple Graph Window";

    struct FontData m_font[MAX_NUM_FONTS];
    int m_numFonts = 0;
    int m_nFontID = -1;
    int m_nHelpFontID = -1;

    void (*inDisplay)(void);
    void (*inKeyboard)(unsigned char key);
    void (*inMousePress)(int message, int x, int y);
    void (*inMouseMove)(int x, int y);
    void (*inSpecialKeyboard)(int key);

    void initGraph(void (*display)(),
                   void (*keyboard)(unsigned char key),
                   void (*mousePress)(int message, int x, int y),
                   void (*mouseMove)(int x, int y),
                   void (*specialKeyboard)(int key))
    {
        int i, argc = 1;
        const char *argv[] = {"Null", 0};
        HDC hDC;
        TEXTMETRIC tm;

        inDisplay = display;
        inKeyboard = keyboard;
        inMousePress = mousePress;
        inMouseMove = mouseMove;
        inSpecialKeyboard = specialKeyboard;

        glutInit(&argc, (char **)argv);

        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

        glutInitWindowPosition(m_winXPos, m_winYPos);
        glutInitWindowSize(m_winWidth, m_winHeight);
        glutCreateWindow(m_strWinTitle);

        if (m_bFullScreen)
            glutFullScreen();

        for (i=0; i<m_numTexs; i++)
        {
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glGenTextures(1, &m_texture[i].texID);
            glBindTexture(GL_TEXTURE_2D, m_texture[i].texID);
            glTexImage2D(GL_TEXTURE_2D, 0, m_texture[i].component,
                         m_texture[i].width, m_texture[i].height, 0,
                         m_texture[i].format, GL_UNSIGNED_BYTE, m_texture[i].pImage);
            free(m_texture[i].pImage);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        m_nHelpFontID = createFont(12, "Arial", BOLD);

        for (i=0; i<m_numFonts; i++)
        {
            hDC = wglGetCurrentDC();

            SelectObject(hDC, m_font[i].hFont);
            GetTextMetrics(hDC, &tm);
            m_font[i].width = tm.tmAveCharWidth;
            m_font[i].fontList = glGenLists(128);
            wglUseFontBitmaps(hDC, 0, 128, m_font[i].fontList);
            DeleteObject(m_font[i].hFont);
        }

        glClearColor(m_bgColor[0],m_bgColor[1],m_bgColor[2],1.0);
        glColor3d(m_color[0], m_color[1], m_color[2]);
        glPointSize(1.0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, (GLdouble) m_winWidth, 0.0, (GLdouble) m_winHeight);

        glutDisplayFunc(displayCK);
        glutKeyboardFunc(keyboardCK);
        glutSpecialFunc(specialKeyboardCK);
        glutMouseFunc(mousePressCK);
        glutPassiveMotionFunc(mouseMoveCK);
        glutReshapeFunc(reshapeCK);
        glutIdleFunc(idleCK);

        m_preTime = glutGet(GLUT_ELAPSED_TIME);

        m_bInitGraphDone = true;

        glutMainLoop();
    }

    void idleCK(void)
    {
        glutPostRedisplay();
    }

    void reshapeCK(int w, int h)
    {
        m_winWidth = w;
        m_winHeight = h;
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, (GLdouble) m_winWidth, 0.0, (GLdouble) m_winHeight);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void keyboardCK(unsigned char key, int x, int y)
    {
        switch (key)
        {
        case 27:
            glutDestroyWindow(glutGetWindow());
            break;
        case 'q':
            if(m_bFullScreen)
                exitFullScreen();
            else
                setFullScreen();
            break;
        default:
            break;
        }
        if (inKeyboard!=NULL)
            inKeyboard(key);
        if (glutGetWindow()!=0)
            glutPostRedisplay();
    }

    void specialKeyboardCK(int key, int x, int y)
    {
        switch (key)
        {
        case GLUT_KEY_F1:
            m_bHelpEnabled = !m_bHelpEnabled;
            break;
        case GLUT_KEY_F2:
            m_bShowCoordinate = !m_bShowCoordinate;
            break;
        }
        if (inSpecialKeyboard!=NULL)
            inSpecialKeyboard(key);

        glutPostRedisplay ();
    }

    void displayCK()
    {
        int now = glutGet(GLUT_ELAPSED_TIME);
        if ((now - m_preTime) > (1000/m_fps))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            inDisplay();
            if(m_bHelpEnabled)
            {
                glColor3d(0.5, 0.5, 0.5);
                useFont(m_nHelpFontID);
                putText(m_winWidth/32, 30*m_winHeight/32, "Press 'Esc' to quit");
                putText(m_winWidth/32, 29*m_winHeight/32, "Press 'q' to switch window/fullscreen");
                putText(m_winWidth/32, 28*m_winHeight/32, "Press 'F1' to turn Help on/off");
                putText(m_winWidth/32, 27*m_winHeight/32, "Press 'F2' to turn Coordinate on/off");
                setColor(m_color[0], m_color[1], m_color[2]);
            }
            if (m_bShowCoordinate)
            {
                showCoordinate();
            }
            glFlush();
            glutSwapBuffers();
            m_preTime = now;
        }
    }

    void setColor(unsigned char red, unsigned char green, unsigned char blue)
    {
        if (m_bBindImage)
        {
            glDisable(GL_TEXTURE_2D);
            m_bBindImage = !m_bBindImage;
        }
        m_color[0] = red/255.0;
        m_color[1] = green/255.0;
        m_color[2] = blue/255.0;
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }

    void setColor(unsigned long color)
    {
        setColor((color >> 16),(color >> 8 & 0xFF),(color & 0xFF));
    }

    void setBkColor(unsigned long color)
    {
        setBkColor((color >> 16),(color >> 8 & 0xFF),(color & 0xFF));
    }

    void line(double startX, double startY, double endX, double endY)
    {
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINES);
        glVertex2d(startX,startY);
        glVertex2d(endX,endY);
        glEnd();
    }

    void rectangle(double x0, double y0, double x1, double y1)
    {
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x0,y0);
        glVertex2d(x1,y0);
        glVertex2d(x1,y1);
        glVertex2d(x0,y1);
        glVertex2d(x0,y0);
        glEnd();
    }

    void fillRectangle(double x0, double y0, double x1, double y1)
    {
        glBegin(GL_TRIANGLES);
        glTexCoord2d(m_texPosU[0],m_texPosV[0]);
        glVertex2d(x0,y0);
        glTexCoord2d(m_texPosU[1],m_texPosV[0]);
        glVertex2d(x1,y0);
        glTexCoord2d(m_texPosU[0],m_texPosV[1]);
        glVertex2d(x0,y1);
        glTexCoord2d(m_texPosU[1],m_texPosV[0]);
        glVertex2d(x1,y0);
        glTexCoord2d(m_texPosU[1],m_texPosV[1]);
        glVertex2d(x1,y1);
        glTexCoord2d(m_texPosU[0],m_texPosV[1]);
        glVertex2d(x0,y1);
        glEnd();
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }

    void triangle(double x0, double y0, double x1, double y1, double x2, double y2)
    {
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINE_LOOP);
        glVertex2d(x0,y0);
        glVertex2d(x1,y1);
        glVertex2d(x2,y2);
        glVertex2d(x0,y0);
        glEnd();
    }

    void fillTriangle(double x0, double y0, double x1, double y1, double x2, double y2)
    {
        double xMin=x0, xMax=x0, yMin=y0, yMax=y0;

        if(x1 < xMin)
            xMin = x1;
        if(x2 < xMin)
            xMin = x2;
        if(x1 > xMax)
            xMax = x1;
        if(x2 > xMax)
            xMax = x2;
        if(y1 < yMin)
            yMin = y1;
        if(y2 < yMin)
            yMin = y2;
        if(y1 > yMax)
            yMax = y1;
        if(y2 > yMax)
            yMax = y2;
        if(xMax==xMin || yMax==yMin)
            return;

        glBegin(GL_TRIANGLES);
        glTexCoord2d((x0-xMin)/(xMax-xMin), (y0-yMin)/(yMax-yMin));
        glVertex2d(x0,y0);
        glTexCoord2d((x1-xMin)/(xMax-xMin), (y1-yMin)/(yMax-yMin));
        glVertex2d(x1,y1);
        glTexCoord2d((x2-xMin)/(xMax-xMin), (y2-yMin)/(yMax-yMin));
        glVertex2d(x2,y2);
        glEnd();
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }


    void circle(double xctr, double yctr, double radius)
    {
        double vectorX,vectorY;
        double angle;

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINE_STRIP);
        for(angle=0; angle < 2.0*PI + ANGLE_STEP; angle+= ANGLE_STEP)
        {
            vectorX= xctr + radius * cos(angle);
            vectorY= yctr + radius * sin(angle);
            glVertex2d(vectorX,vectorY);
        }
        glEnd();
    }

    void fillCircle(double xctr, double yctr, double radius)
    {
        double vectorX1,vectorY1;
        double vectorX0,vectorY0;
        double angle;
        double delta = 0.1*radius;

        if(radius <= 0)
            return;
        vectorX1 = xctr + radius;
        vectorY1 = yctr;
        glBegin(GL_TRIANGLES);
        for(angle=ANGLE_STEP; angle < 2.0*PI + ANGLE_STEP; angle+= ANGLE_STEP)
        {
            vectorX0 = vectorX1;
            vectorY0 = vectorY1;
            vectorX1= xctr + radius*cos(angle);
            vectorY1= yctr + radius*sin(angle);
            glTexCoord2d(0.5, 0.5);
            glVertex2d(xctr,yctr);
            glTexCoord2d(0.5+(vectorX0-xctr)/(2*radius+delta), 0.5+(vectorY0-yctr)/(2*radius+delta));
            glVertex2d(vectorX0,vectorY0);
            glTexCoord2d(0.5+(vectorX1-xctr)/(2*radius+delta), 0.5+(vectorY1-yctr)/(2*radius+delta));
            glVertex2d(vectorX1,vectorY1);
        }
        glEnd();
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }

    void ellipse(double xctr, double yctr, double xRadius, double yRadius)
    {
        double vectorX, vectorY;
        double angle;

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINE_STRIP);
        for(angle=0; angle < 2.0*PI + ANGLE_STEP; angle+= ANGLE_STEP)
        {
            vectorX= xctr + xRadius * cos(angle);
            vectorY= yctr + yRadius * sin(angle);
            glVertex2d(vectorX, vectorY);
        }
        glEnd();
    }

    void donut(double xctr, double yctr, double xRadius, double yRadius, double ratio)
    {
        double vectorX, vectorY;
        double angle;

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINE_STRIP);
        for(angle=0; angle < 2.0*PI + ANGLE_STEP; angle+= ANGLE_STEP)
        {
            vectorX= xctr + xRadius * cos(angle);
            vectorY= yctr + yRadius * sin(angle);
            glVertex2d(vectorX, vectorY);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for(angle=0; angle < 2.0*PI + ANGLE_STEP; angle+= ANGLE_STEP)
        {
            vectorX= xctr + ratio*xRadius * cos(angle);
            vectorY= yctr + ratio*yRadius * sin(angle);
            glVertex2d(vectorX, vectorY);
        }
        glEnd();
    }

    void fillEllipse(double xctr, double yctr, double xRadius, double yRadius)
    {
        double vectorX1,vectorY1;
        double vectorX0,vectorY0;
        double angle;

        vectorX1 = xctr + xRadius;
        vectorY1 = yctr;
        glBegin(GL_TRIANGLES);
        for(angle=ANGLE_STEP; angle < 2.0*PI + ANGLE_STEP; angle+= ANGLE_STEP)
        {
            vectorX0 = vectorX1;
            vectorY0 = vectorY1;
            vectorX1= xctr + xRadius*cos(angle);
            vectorY1= yctr + yRadius*sin(angle);

            glTexCoord2d(0.5, 0.5);
            glVertex2d(xctr, yctr);
            glTexCoord2d(0.5+(vectorX0-xctr)/(2*xRadius), 0.5+(vectorY0-yctr)/(2*yRadius));
            glVertex2d(vectorX0,vectorY0);
            glTexCoord2d(0.5+(vectorX1-xctr)/(2*xRadius), 0.5+(vectorY1-yctr)/(2*yRadius));
            glVertex2d(vectorX1,vectorY1);
        }
        glEnd();
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }

    void fillDonut(double xctr, double yctr, double xRadius, double yRadius, double ratio)
    {
        double vectorX3,vectorY3;
        double vectorX2,vectorY2;
        double vectorX1,vectorY1;
        double vectorX0,vectorY0;
        double angle;

        vectorX1 = xctr + xRadius;
        vectorY1 = yctr;
        vectorX3 = xctr + ratio*xRadius;
        vectorY3 = yctr;
        glBegin(GL_TRIANGLES);
        for(angle=ANGLE_STEP; angle < 2.0*PI + ANGLE_STEP; angle+= ANGLE_STEP)
        {
            vectorX0 = vectorX1;
            vectorY0 = vectorY1;
            vectorX2 = vectorX3;
            vectorY2 = vectorY3;
            vectorX1= xctr + xRadius*cos(angle);
            vectorY1= yctr + yRadius*sin(angle);
            vectorX3= xctr + ratio*xRadius*cos(angle);
            vectorY3= yctr + ratio*yRadius*sin(angle);

            glTexCoord2d(0.5+(vectorX2-xctr)/(2*xRadius), 0.5+(vectorY2-yctr)/(2*yRadius));
            glVertex2d(vectorX2, vectorY2);
            glTexCoord2d(0.5+(vectorX0-xctr)/(2*xRadius), 0.5+(vectorY0-yctr)/(2*yRadius));
            glVertex2d(vectorX0,vectorY0);
            glTexCoord2d(0.5+(vectorX1-xctr)/(2*xRadius), 0.5+(vectorY1-yctr)/(2*yRadius));
            glVertex2d(vectorX1,vectorY1);

            glTexCoord2d(0.5+(vectorX1-xctr)/(2*xRadius), 0.5+(vectorY1-yctr)/(2*yRadius));
            glVertex2d(vectorX1, vectorY1);
            glTexCoord2d(0.5+(vectorX3-xctr)/(2*xRadius), 0.5+(vectorY3-yctr)/(2*yRadius));
            glVertex2d(vectorX3,vectorY3);
            glTexCoord2d(0.5+(vectorX2-xctr)/(2*xRadius), 0.5+(vectorY2-yctr)/(2*yRadius));
            glVertex2d(vectorX2,vectorY2);
        }
        glEnd();
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }

    void arc(double xctr, double yctr, double radius, double startAngle, double endAngle)
    {
        double vectorX,vectorY;
        double angle, ang0,ang1;

        glDisable(GL_TEXTURE_2D);
        ang0 = startAngle * (PI/180.0);
        ang1 = endAngle * (PI/180.0);
        glBegin(GL_LINE_STRIP);
        for(angle=ang0; angle <= ang1+ANGLE_STEP; angle+= ANGLE_STEP)
        {
            vectorX = xctr + radius*cos(angle);
            vectorY = yctr + radius*sin(angle);
            glVertex2d(vectorX,vectorY);
        }
        glEnd();
    }

    void sector(double xctr, double yctr, double radius, double startAngle, double endAngle)
    {
        double vectorX1,vectorY1;
        double vectorX0,vectorY0;
        double angle,ang0,ang1;

        ang0 = startAngle * (PI/180.0);
        ang1 = endAngle * (PI/180.0);
        glBegin(GL_TRIANGLES);

        vectorX1 = xctr + radius*cos(ang0);
        vectorY1 = yctr + radius*sin(ang0);
        for(angle=ang0 + ANGLE_STEP; angle < ang1 + ANGLE_STEP; angle += ANGLE_STEP)
        {
            vectorX0 = vectorX1;
            vectorY0 = vectorY1;
            vectorX1= xctr + radius*cos(angle);
            vectorY1= yctr + radius*sin(angle);
            glTexCoord2d(0.5, 0.5);
            glVertex2d(xctr,yctr);
            glTexCoord2d(0.5+(vectorX0-xctr)/(2*radius), 0.5+(vectorY0-yctr)/(2*radius));
            glVertex2d(vectorX0,vectorY0);
            glTexCoord2d(0.5+(vectorX1-xctr)/(2*radius), 0.5+(vectorY1-yctr)/(2*radius));
            glVertex2d(vectorX1,vectorY1);
        }
        glEnd();
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }

    void polygon(int numPoints, double x[], double y[])
    {
        int i;

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINE_STRIP);
        for(i=0; i<numPoints; i++)
            glVertex2d(x[i], y[i]);
        glVertex2d(x[0], y[0]);
        glEnd();
    }

    void fillPolygon(int numPoints, double x[], double y[], bool bConvex)
    {
        int i;
        double xMin, xMax, yMin, yMax;

        if(numPoints<=2)
            return;

        xMin = x[0];
        xMax = x[0];
        yMin = y[0];
        yMax = y[0];

        for (i=1; i<numPoints; i++)
        {
            if (xMin > x[i])
                xMin = x[i];
            if (xMax < x[i])
                xMax = x[i];
            if (yMin > y[i])
                yMin = y[i];
            if (yMax < y[i])
                yMax = y[i];
        }

        if(xMin==xMax || yMin==yMax)
            return;

        if(bConvex)
        {
            glBegin(GL_POLYGON);
            for(i=0; i<numPoints; i++)
            {
                glTexCoord2d((x[i]-xMin)/(xMax-xMin), (y[i]-yMin)/(yMax-yMin));
                glVertex2d(x[i], y[i]);
            }
            glTexCoord2d((x[0]-xMin)/(xMax-xMin), (y[0]-yMin)/(yMax-yMin));
            glVertex2d(x[0], y[0]);
            glEnd();
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
            scanLineFilling(numPoints, x, y);
        }
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }

    void putPixel(double x, double y)
    {
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_POINTS);
        glVertex2d(x, y);
        glEnd();
    }

    void getPixel(int x, int y, unsigned char &red, unsigned char &green, unsigned char &blue)
    {
        unsigned char pixel[4];

        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        red = pixel[0];
        green = pixel[1];
        blue = pixel[2];
    }

    void arrowLine(double startX, double startY, double endX, double endY, double s1, double s2)
    {
        double l, t;
        double k;
        double xe, ye, xk, yk, xp, yp, xq, yq;

        l = sqrt((endX-startX)*(endX-startX)+(endY-startY)*(endY-startY));
        if(l<1.0e-6)
            return;

        xe = endX;
        ye = endY;

        t = (l-s1-s2)/l;
        xk = startX + t*(endX-startX);
        yk = startY + t*(endY-startY);

        if(fabs(endY-startY)<1.0e-6)
        {
            xp = xk;
            yp = startY+s1;
            xq = xk;
            yq = startY-s1;
        }
        else
        {
            k = -(endX-startX)/(endY-startY);
            xp = xk + s1/sqrt(1+k*k);
            yp = yk + s1*k/sqrt(1+k*k);
            xq = xk - s1/sqrt(1+k*k);
            yq = yk - s1*k/sqrt(1+k*k);
        }

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINES);
        glVertex2d(startX,startY);
        glVertex2d(endX,endY);
        glEnd();
        fillTriangle(xe, ye, xp, yp, xq, yq);
    }

    void scanLineFilling(int numPoints,double polyX[], double polyY[])
    {
        int  nodes, *nodeX, pixelY, i, j, swap ;
        int IMAGE_TOP, IMAGE_BOT, IMAGE_LEFT, IMAGE_RIGHT;

        IMAGE_TOP = int(polyY[0]+0.5);
        IMAGE_BOT = int(polyY[0]+0.5);
        IMAGE_LEFT = int(polyX[0]+0.5);
        IMAGE_RIGHT = int(polyX[0]+0.5);

        for (i=1; i<numPoints; i++)
        {
            if (IMAGE_LEFT > polyX[i])
                IMAGE_LEFT = int(polyX[i]+0.5);
            if (IMAGE_RIGHT < polyX[i])
                IMAGE_RIGHT = int(polyX[i]+0.5);
            if (IMAGE_TOP > polyY[i])
                IMAGE_TOP = int(polyY[i]+0.5);
            if (IMAGE_BOT < polyY[i])
                IMAGE_BOT = int(polyY[i]+0.5);
        }
        nodeX = new int[numPoints];
        for (pixelY=IMAGE_TOP; pixelY<=IMAGE_BOT; pixelY++)
        {
            nodes=0;
            j=numPoints-1;
            for (i=0; i<numPoints; i++)
            {
                if ((polyY[i]<(double)pixelY && polyY[j]>=(double)pixelY)
                        ||  (polyY[j]<(double)pixelY && polyY[i]>=(double)pixelY))
                {
                    nodeX[nodes++]=(int) (polyX[i]+(pixelY-polyY[i])/(polyY[j]-polyY[i])
                                          *(polyX[j]-polyX[i])+0.5);
                }
                j=i;
            }

            i=0;
            while (i<nodes-1)
            {
                if (nodeX[i]>nodeX[i+1])
                {
                    swap=nodeX[i];
                    nodeX[i]=nodeX[i+1];
                    nodeX[i+1]=swap;
                    if (i)
                        i--;
                }
                else
                {
                    i++;
                }
            }

            glBegin(GL_POINTS);
            for (i=0; i<nodes; i+=2)
            {
                if   (nodeX[i ]> IMAGE_RIGHT)
                    break;
                if   (nodeX[i+1]> IMAGE_LEFT )
                {
                    if (nodeX[i]< IMAGE_LEFT )
                        nodeX[i]=IMAGE_LEFT ;
                    if (nodeX[i+1]> IMAGE_RIGHT)
                        nodeX[i+1]=IMAGE_RIGHT;
                    for (j=nodeX[i]; j<nodeX[i+1]; j++)
                        glVertex2d(j,pixelY);
                }
            }
            glEnd();
        }
        delete []nodeX;
    }

    void putText(int x, int y, const char strText[])
    {
        int len=strlen(strText);

        if (m_nFontID<0)
            return;
        glDisable(GL_TEXTURE_2D);
        glRasterPos2i(x, y);
        glListBase(m_font[m_nFontID].fontList);
        glCallLists(len, GL_UNSIGNED_BYTE, strText);
    }

    void useFont(int fontID)
    {
        m_nFontID = fontID;
    }

    int getFontHeight(int fontID)
    {
        if (fontID<0)
            return 0;
        return m_font[fontID].height;
    }

    int getFontWidth(int fontID)
    {
        if (fontID<0)
            return 0;
        return m_font[fontID].width;
    }

    int getWinHeight()
    {
        return m_winHeight;
    }

    int getWinWidth()
    {
        return m_winWidth;
    }

    void setWinSize(int width, int height)
    {
        m_winWidth = width;
        m_winHeight = height;
    }

    void setFullScreen()
    {
        if(!m_bFullScreen)
        {
            if (m_bInitGraphDone)
                glutFullScreen();
            m_bFullScreen = !m_bFullScreen;
        }
    }

    void exitFullScreen()
    {
        if(m_bFullScreen)
        {
            m_winWidth = 1024;
            m_winHeight = 768;
            glutPositionWindow(m_winXPos, m_winYPos);
            glutReshapeWindow(m_winWidth, m_winHeight);
            m_bFullScreen = !m_bFullScreen;
        }
    }

    void setWinTitle(char strTitle[])
    {
        strcpy(m_strWinTitle, strTitle);
    }

    void setDottedLines()
    {
        glLineStipple(4, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
    }

    void setSolidLines()
    {
        glDisable(GL_LINE_STIPPLE);
    }

    void setLineWidth(int wide)
    {
        glLineWidth(wide);
    }

    void setPointSize(double size)
    {
        glPointSize(size);
    }

    void clearWindow()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void setBkColor(unsigned char red, unsigned char green, unsigned char blue)
    {
        if(m_bInitGraphDone)
        {
            glClearColor(red/255.0, green/255.0, blue/255.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        else
        {
            m_bgColor[0] = red/255.0;
            m_bgColor[1] = green/255.0;
            m_bgColor[2] = blue/255.0;
        }
    }

    int createFont(int pixHeight, const char style[], int weight, bool bItalic)
    {
        HFONT hFont;
        LOGFONT logfont;
        int fontHeight = abs((pixHeight * 96) / 72);

        if(m_bInitGraphDone || m_numFonts>=MAX_NUM_FONTS)
            return -1;

        logfont.lfHeight = -fontHeight;
        logfont.lfWidth = 0;
        logfont.lfEscapement = 0;
        logfont.lfOrientation = 0;
        logfont.lfWeight = weight;
        logfont.lfItalic = bItalic;
        logfont.lfUnderline = FALSE;
        logfont.lfStrikeOut = FALSE;
        logfont.lfCharSet = ANSI_CHARSET;
        logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
        logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        logfont.lfQuality = DEFAULT_QUALITY;
        logfont.lfPitchAndFamily = DEFAULT_PITCH;
        strcpy(logfont.lfFaceName, style);

        hFont = CreateFontIndirect(&logfont);
        m_font[m_numFonts].hFont = hFont;
        m_font[m_numFonts].height = pixHeight;
        m_nFontID = m_numFonts;
        m_numFonts++;

        return (m_numFonts-1);
    }

    GLbyte *loadBMP(const char *fileName, int &width, int &height,
                    GLint &component, GLenum &format, bool bGrey)
    {
        GLbyte* pBits;
        int i,j,texpos = 0;
        short sDepth;
        FILE* pFile;
        BITMAPFILEHEADER bmfileheader;
        BITMAPINFOHEADER bminfoheader;
        RGBTRIPLE rgb;
        char alpha;

        pFile = fopen(fileName, "rb");

        if (!pFile)
            return NULL;

        fread(&bmfileheader, sizeof(BITMAPFILEHEADER), 1, pFile);
        fread(&bminfoheader, sizeof(BITMAPINFOHEADER), 1, pFile);

        sDepth = bminfoheader.biBitCount/8;
        if (bminfoheader.biBitCount!=24 && bminfoheader.biBitCount!=32)
            return NULL;

        width = bminfoheader.biWidth;
        height = bminfoheader.biHeight;

        pBits = (GLbyte*)malloc(width * height *sDepth);

        for (i = 0; i < (int) width; i++)
        {
            for(j = height - 1; j>=0 ; j--)
            {
                fread(&rgb, sizeof(RGBTRIPLE),1,pFile);
                pBits[texpos] = rgb.rgbtBlue ;
                if (!bGrey)
                {
                    pBits[texpos+1] = rgb.rgbtGreen;
                    pBits[texpos+2] = rgb.rgbtRed;
                }
                else
                {
                    pBits[texpos+1] = pBits[texpos];
                    pBits[texpos+2] = pBits[texpos];
                }
                if(sDepth==4)
                {
                    fread(&alpha, 1,1,pFile);
                    pBits[texpos+3] = alpha;
                }
                texpos += sDepth;
            }
        }

        switch(sDepth)
        {
        case 3:
            format = GL_BGR_EXT;
            component = GL_RGB8;
            break;
        case 4:
            format = GL_BGRA_EXT;
            component = GL_RGBA8;
            break;
        }
        fclose(pFile);

        return pBits;
    }

    int loadImage(const char *fileName, bool bGrey)
    {
        GLbyte *pBytes;
        GLint component = 0;
        GLenum format = 0;
        GLint width;
        GLint height;

        if (m_numTexs>=MAX_NUM_TEXS)
            return -1;

        pBytes = loadBMP(fileName, width, height, component, format, bGrey);
        if (!pBytes)
        {
            printf("Image file %s loading error!\n", fileName);
            exit(0);
        }

        m_texture[m_numTexs].pImage = (void *)pBytes;
        m_texture[m_numTexs].width = width;
        m_texture[m_numTexs].height = height;
        m_texture[m_numTexs].component = component;
        m_texture[m_numTexs].format = format;
        m_numTexs++;

        return (m_numTexs-1);
    }

    void showImage(int imageID,
                   double startU, double startV,
                   double endU, double endV)
    {
        if (imageID==-1)
            return;

        glColor3d(1.0, 1.0, 1.0);
        m_texPosU[0] = startU;
        m_texPosU[1] = endU;
        m_texPosV[0] = startV;
        m_texPosV[1] = endV;
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_texture[imageID].texID);
        fillRectangle(0, 0, m_winWidth, m_winHeight);
        glDisable(GL_TEXTURE_2D);
        glColor3d(m_color[0], m_color[1], m_color[2]);
    }

    void bindImage(int imageID, bool bBlend,
                   double startU, double startV, double endU, double endV)
    {
        if (imageID==-1)
            return;

        glEnable(GL_TEXTURE_2D);
        if (bBlend)
            glColor3d(m_color[0], m_color[1], m_color[2]);
        else
            glColor3d(1.0, 1.0, 1.0);
        m_texPosU[0] = startU;
        m_texPosV[0] = startV;
        m_texPosU[1] = endU;
        m_texPosV[1] = endV;
        glBindTexture(GL_TEXTURE_2D, m_texture[imageID].texID);
        m_bBindImage = true;
    }

    void mousePressCK(int button, int state, int x, int y)
    {
        int message;

        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            message = LEFT_BUTTON_DOWN;
        else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
            message = LEFT_BUTTON_UP;
        else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
            message = RIGHT_BUTTON_DOWN;
        else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
            message = RIGHT_BUTTON_UP;
        else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
            message = MIDDLE_BUTTON_DOWN;
        else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
            message = MIDDLE_BUTTON_UP;
        else
            message = -1;

        y = m_winHeight - y;
        if (inMousePress!=NULL)
            inMousePress(message, x, y);

        glutPostRedisplay();
    }

    void mouseMoveCK(int x, int y)
    {
        y = m_winHeight - y;
        if (inMouseMove!=NULL)
            inMouseMove(x, y);

        glutPostRedisplay();
    }

    void setFPS(int fps)
    {
        if (fps<0)
            return;

        m_fps = fps;
    }

    int getFPS()
    {
        return m_fps;
    }

    void showCoordinate()
    {
        int i;
        int step = m_winWidth/100;
        int offset = 5, len = 10;
        double ratio = 0.95;

        setLineWidth(2);

        glColor3d(0.5, 0.5, 0.5);
        arrowLine(offset, offset, ratio*m_winWidth+offset, offset);
        glColor3d(0.5, 0.5, 0.5);
        arrowLine(offset, offset, offset, ratio*m_winHeight+offset);

        glColor3d(0.5, 0.5, 0.5);
        for(i=offset; i<ratio*m_winWidth-step; i+=step)
            line(i, offset, i, len+offset);
        glColor3d(0.5, 0.5, 0.5);
        for(i=offset; i<ratio*m_winHeight-step; i+=step)
            line(offset, i,  len+offset, i);

        useFont(m_nHelpFontID);
        glColor3d(0.75, 0, 0);
        putText(1.5*offset, 1.5*offset, "O");
        putText(ratio*m_winWidth+2*offset, offset, "X");
        putText(2*offset, ratio*m_winHeight+offset, "Y");

        setColor(m_color[0], m_color[1], m_color[2]);
    }

    void showHelp()
    {
        m_bHelpEnabled = true;
    }

    bool saveScreenImage(char* filename)
    {
        unsigned char *bmpBuffer;
        unsigned char temp;
        long size, i;
        GLint view[4];
        int wWidth, wHeight;
        FILE *filePtr;
        BITMAPFILEHEADER  bitmapFileHeader;
        BITMAPINFOHEADER  bitmapInfoHeader;

        glGetIntegerv(GL_VIEWPORT,view);
        wWidth = view[2]-view[0];
        wHeight = view[3]-view[1];

        bmpBuffer = new unsigned char[wWidth*wHeight*3];
        if  (!bmpBuffer)
            return false;

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(view[0], view[1], wWidth, wHeight, GL_RGB, GL_UNSIGNED_BYTE, bmpBuffer);

        size = 3*wWidth*wHeight;
        i = 0;
        while (i<size)
        {
            temp = bmpBuffer[i];
            bmpBuffer[i] = bmpBuffer[i+2];
            bmpBuffer[i+2] = temp;
            i += 3;
        }

        filePtr = fopen(filename,  "wb");
        if (!filePtr)
            return false;

        bitmapFileHeader.bfType = 0x4D42;
        bitmapFileHeader.bfSize =  size;
        bitmapFileHeader.bfReserved1 =  0;
        bitmapFileHeader.bfReserved2 = 0;
        bitmapFileHeader.bfOffBits  = sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER);

        bitmapInfoHeader.biSize =  sizeof(BITMAPINFOHEADER);
        bitmapInfoHeader.biWidth =  wWidth;
        bitmapInfoHeader.biHeight =  wHeight;
        bitmapInfoHeader.biPlanes = 1;
        bitmapInfoHeader.biBitCount  = 24;
        bitmapInfoHeader.biCompression =  BI_RGB;
        bitmapInfoHeader.biSizeImage = 0;
        bitmapInfoHeader.biXPelsPerMeter  = 0;
        bitmapInfoHeader.biYPelsPerMeter = 0;
        bitmapInfoHeader.biClrUsed = 0;
        bitmapInfoHeader.biClrImportant =  0;

        fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1,  filePtr);
        fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1,  filePtr);
        fwrite(bmpBuffer, size, 1,  filePtr);
        fclose(filePtr);

        delete []bmpBuffer;

        return true;
    }
}
