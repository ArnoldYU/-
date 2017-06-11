#include<iostream>
#include<cmath>
#include <GLUT/GLUT.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define ROUND(a) int (a+0.5)
#define LEFT 1      //0001
#define RIGHT 2     //0010
#define BOTTOM 4    //0100
#define TOP 8       //1000

using namespace std;
#define pi 3.1415926
typedef float Color[3];
typedef struct MYPoint{
    int x;
    int y;
}P;
P point[100];
P insidepoint[100];

P linepoint[100];
P lineinsidepoint[100];

double line_x,line_y; //交点
int flag0=0;
int num = 0;
int linenum = 0;
int lineinsidenum = 0;
int choice = 0;
unsigned int RC,RC0,RC1;
int m_i;//判断1或2点
int wxl,wxr,wyb,wyt;//上下左右
bool m_attatch;
bool m_draw;
float h,m,s;

void setpixel(GLint x, GLint y);
void myMouse(int button,int state,int x,int y);
void display();
void getpixel(GLint x, GLint y, Color color);
void putpixel(int x, int y);
void init(void);
void Bresenham_line(int myi, int x0, int y0, int x1, int y1);
void swap_value(int* a, int* b);
void ProcessMenu(int value);
int compareColor(Color color1, Color color2);
void boundaryFill4(int x, int y, Color fillColor, Color boarderColor);
int inOrNot(float x, float y);
void move ();
void rotate2D();
void scale2D();
void change2D();
void cut_line();//与下面这个函数配合，是纯求交点截取线段
bool between(double a, double X0, double X1);
bool detectIntersect(P p1, P p2, P p3, P p4,int i);



int EnCode(double linepx,double linepy)
{
    RC=0;
    if (linepx<wxl){
        RC=RC|LEFT;
    }
    if (linepx>wxr){
        RC=RC|RIGHT;
    }
    if(linepy<500-wyt){
        RC=RC|BOTTOM;
    }
    if (linepy>500-wyb){
        RC=RC|TOP;
    }
    return RC;
}

void Cohen()
{
    bool change;
    double x,y;
    for(int i=0;i<linenum;i+=2){
        RC0=EnCode(linepoint[i].x, linepoint[i].y);
        RC1=EnCode(linepoint[i+1].x, linepoint[i+1].y);
        while (1){
            change=false;
            if(0==(RC0|RC1)){//表示编码全为0
                lineinsidepoint[lineinsidenum].x=linepoint[i].x;
                lineinsidepoint[lineinsidenum].y=linepoint[i].y;
                lineinsidenum++;
                lineinsidepoint[lineinsidenum].x=linepoint[i+1].x;
                lineinsidepoint[lineinsidenum].y=linepoint[i+1].y;
                lineinsidenum++;
                break;
            } else if (0!=(RC0 & RC1)) {//不为0 全舍弃
                break;
            } else {
                if (0==RC0){//如果P0点在窗口内，交换P0和P1，保证P0在窗口外
                    double TPointx,TPointy;
                    TPointx=linepoint[i].x;TPointy=linepoint[i].y;
                    linepoint[i].x=linepoint[i+1].x;
                    linepoint[i].y=linepoint[i+1].y;
                    linepoint[i+1].x=TPointx;
                    linepoint[i+1].y=TPointy;
                    unsigned int TRC;
                    TRC=RC0;RC0=RC1;RC1=TRC;
                }
                //按左右下上的顺序裁剪
                if(RC0 & LEFT )//P0点位于窗口的左侧
                {
                    x=wxl;//求交点y
                    y=(double)linepoint[i].y+(linepoint[i+1].y-linepoint[i].y)*(x-linepoint[i].x)/(linepoint[i+1].x-linepoint[i].x);
                    cout << "y:" << y<<endl;
                    linepoint[i].x=x;
                    linepoint[i].y=y;
                    change=true;
                    RC0=EnCode(linepoint[i].x,linepoint[i].y);
                    RC1=EnCode(linepoint[i+1].x,linepoint[i+1].y);
                }
                if(RC0 & RIGHT )//P0点位于窗口的右侧
                {
                    x=wxr;//求交点y
                    y=(double)linepoint[i].y+(linepoint[i+1].y-linepoint[i].y)*(x-linepoint[i].x)/(linepoint[i+1].x-linepoint[i].x);
                    cout << "y:" << y<<endl;
                    linepoint[i].x=x;
                    linepoint[i].y=y;
                    change=true;
                    RC0=EnCode(linepoint[i].x,linepoint[i].y);
                    RC1=EnCode(linepoint[i+1].x,linepoint[i+1].y);
                }
                if(RC0 & BOTTOM )//P0点位于窗口的下侧
                {
                    y=500-wyt;//求交点x
                    x=(double)linepoint[i].x+(linepoint[i+1].x-linepoint[i].x)*(y-linepoint[i].y)/(linepoint[i+1].y-linepoint[i].y);
                    cout << "x:" << x<<endl;
                    linepoint[i].x=x;
                    linepoint[i].y=y;
                    change=true;
                    RC0=EnCode(linepoint[i].x,linepoint[i].y);
                    RC1=EnCode(linepoint[i+1].x,linepoint[i+1].y);
                }
                if(RC0 & TOP )//P0点位于窗口的上侧
                {
                    y=500-wyb;//求交点x
                    x=(double)linepoint[i].x+(linepoint[i+1].x-linepoint[i].x)*(y-linepoint[i].y)/(linepoint[i+1].y-linepoint[i].y);
                    cout << "x:" << x<<endl;
                    linepoint[i].x=x;
                    linepoint[i].y=y;
                    change=true;
                    RC0=EnCode(linepoint[i].x,linepoint[i].y);
                    RC1=EnCode(linepoint[i+1].x,linepoint[i+1].y);
                }
                if(false==change)
                {
                    break;
                }
            }
        }
    
    
    }
}

void Draw()
{
    int i;
    float R,TR,h_Angle,m_Angle,s_Angle,count,h_Length,m_Length,s_Length;
    R=0.5;
    TR=R-0.05;
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    for (i=0; i<100; i++)
    {
        glVertex2f(R*cos(2*pi/100*i),R*sin(2*pi/100*i));
    }
    glEnd();
    glLineWidth(2);
    for (i=0; i<12; i++)
    {
        glBegin(GL_LINES);
        glVertex2f(TR*sin(2*pi/12*i),TR*cos(2*pi/12*i));
        glVertex2f(R*sin(2*pi/12*i),R*cos(2*pi/12*i));
        glEnd();
    }
    glLineWidth(1);
    
    
    
    h_Length=0.2;
    m_Length=0.3;
    s_Length=0.4;
    count=60;
    s_Angle=s/count;
    count*=60;
    m_Angle=(m*60+s)/count;
    count*=12;
    h_Angle=(h*60*60+m*60+s)/count;
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(s_Length*sin(2*pi*s_Angle),s_Length*cos(2*pi*s_Angle));
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(h_Length*sin(2*pi*h_Angle),h_Length*cos(2*pi*h_Angle));
    glEnd();
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(m_Length*sin(2*pi*m_Angle),m_Length*cos(2*pi*m_Angle));
    glEnd();
    glLineWidth(1);
    
    glBegin(GL_POLYGON);
    for (i=0; i<100; i++)
    {
        glVertex2f(0.03*cos(2*pi/100*i),0.03*sin(2*pi/100*i));
    }
    glEnd();
    glutSwapBuffers();
}

void Update()
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    h=timeinfo->tm_hour;
    m=timeinfo->tm_min;
    s=timeinfo->tm_sec;
    glutPostRedisplay();//强制刷新
}

int main(int argc, char* argv[])
{
    int DB1;
    int DB2;
    int DB6;
    int DB3;
    int DB4;
    int DB5;
    int nMainMenu;
    
    wxl=250;wxr=350;wyb=250;wyt=450;
    m_attatch=false;
    m_i=0;
    m_draw=false;
    RC=0;RC1=0;
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RED);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Bresenham_line");
    glutDisplayFunc(display);
    glutMouseFunc(myMouse);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    //glutIdleFunc(Update);
   // glutMainLoop();
    //子菜单1
    DB1 = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Bresenham直线算法",1);
    //子菜单2
    DB2 = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("种子填充算法", 2);
    //子菜单3
    DB3 = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("平移", 3);
    glutAddMenuEntry("对称", 4);
    glutAddMenuEntry("缩放", 5);
    glutAddMenuEntry("旋转",6);
    //子菜单4
    DB4 = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("要裁剪的直线段", 7);
    glutAddMenuEntry("裁剪的多边形", 8);
    glutAddMenuEntry("画直线段", 9);
    glutAddMenuEntry("画多边形裁剪", 10);
    glutAddMenuEntry("直线段裁剪", 11);
    glutAddMenuEntry("多边形裁剪", 12);
    //子菜单5
    DB5 = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("动画", 13);
    //子菜单6
    DB6 = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("清除",0);
    //创建主菜单
    nMainMenu = glutCreateMenu(ProcessMenu);
    glutAddSubMenu("实验一",DB1);
    glutAddSubMenu("实验二", DB2);
    glutAddSubMenu("实验三", DB3);
    glutAddSubMenu("实验四", DB4);
    glutAddSubMenu("实验五", DB5);
    glutAddSubMenu("其他", DB6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}
void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.
    glMatrixMode(GL_PROJECTION);       // Set projection parameters.
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}
/*
 交换两个int 类型的变量的值
 */
void swap_value(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void putpixel(int x, int y)
{//画点的方法
    glColor3f(0.0, 1.0, 2.0);//画笔颜色
    glPointSize(1.0);//画笔粗细
    glBegin(GL_POINTS);
    glVertex2f(x, y);//画点
    glEnd();
}
void Bresenham_line(int myi, int x0, int y0, int x1, int y1)
{
    int dx, dy, h, a, b, x, y, flag, t;
    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    if (x1 > x0)
        a = 1;
    else
        a = -1;
    if (y1 > y0)
        b = 1;
    else
        b = -1;
    x = x0;
    y = y0;
    
    if (dx >= dy) {
        //0< |k| <=1
        flag = 0;
    } else {
        //|k|>1,交换dx，dy
        t = dx;
        dx = dy;
        dy = t;
        flag = 1;
    }
    h = 2 * dy - dx;
    for (int i = 1; i <= dx; ++i) {
        putpixel(x, y);

        if (h >= 0) {
            if (flag == 0)//0< |k| <=1
                y = y + b;
            else
                x = x + a;//|k|>1
            h = h - 2 * dx;
        }
        if (flag == 0)
            x = x + a;
        else
            y = y + b;
        h = h + 2 * dy;
        
    }
    
}
void display()
{
    Color fillColor = {0.0, 1.0, 2.0};
    Color boarderColor = {0.0, 1.0, 1.0};
    glClear(GL_COLOR_BUFFER_BIT);
    int i = 0;
    int k = 0;
    int avgx=0,avgy=0;
    glClearColor(1.0, 1.0, 1.0, 0.0);
    if (num > 1) {
        for (i = 0; i < num - 1; i++){
            Bresenham_line(i, point[i].x, point[i].y, point[i+1].x, point[i+1].y);

        }
        if (num > 2)
            Bresenham_line(i, point[i].x, point[i].y, point[0].x, point[0].y);
    }
    if (choice == 9) {
        if (num > 1) {
            for (i=0;i<linenum;i+=2){
                Bresenham_line(i, linepoint[i].x, linepoint[i].y, linepoint[i+1].x, linepoint[i+1].y);
            }
        }
    }
    if (choice == 11) {
        if (num > 1) {
            for (i=0;i<lineinsidenum;i+=2){
                Bresenham_line(i, lineinsidepoint[i].x, lineinsidepoint[i].y, lineinsidepoint[i+1].x, lineinsidepoint[i+1].y);
            }
        }
    }
    if (choice == 2) {
        cout << 222 << endl;
        for (int i = 0; i < num;i++){
            avgx += point[i].x;
            avgy += point[i].y;
        }
        avgx=avgx/num;
        avgy=avgy/num;
        //boundaryFill4(avgx,avgy, fillColor, boarderColor);
        for (int ii = 0;ii<num;ii++){
            if (inOrNot(point[ii].x+10, point[ii].y-10)){
                boundaryFill4(point[ii].x+10, point[ii].y-10, fillColor, boarderColor);
                cout << ii <<":"<<1<<endl;
            }
            else if (inOrNot(point[ii].x+10, point[ii].y+10)){
                boundaryFill4(point[ii].x+10, point[ii].y+10, fillColor, boarderColor);
                cout << ii <<":"<<2<<endl;
            }
            
            else if (inOrNot(point[ii].x-10, point[ii].y-10)){
                boundaryFill4(point[ii].x-10, point[ii].y-10, fillColor, boarderColor);
                cout << ii <<":"<<3<<endl;
            }
                
            else if (inOrNot(point[ii].x-10, point[ii].y+10)){
                boundaryFill4(point[ii].x-10, point[ii].y+10, fillColor, boarderColor);
                cout << ii <<":"<<4<<endl;
            }
                
            else if (inOrNot(point[ii].x, point[ii].y-10)){
                boundaryFill4(point[ii].x, point[ii].y-10, fillColor, boarderColor);
                cout << ii <<":"<<5<<endl;
            }
                
            else if (inOrNot(point[ii].x, point[ii].y+10)){
                boundaryFill4(point[ii].x, point[ii].y+10, fillColor, boarderColor);
                cout << ii <<":"<<6<<endl;
            }
                
            else if (inOrNot(point[ii].x-10, point[ii].y)){
                boundaryFill4(point[ii].x-10, point[ii].y, fillColor, boarderColor);
                cout << ii <<":"<<7<<endl;
            }
                
            else if (inOrNot(point[ii].x+10, point[ii].y)){
                boundaryFill4(point[ii].x+10, point[ii].y, fillColor, boarderColor);
                cout << ii <<":"<<8<<endl;
            }
                
        }

        cout << "end" <<endl;
    }
    glFlush();
}
void myMouse(int button,int state,int x,int y)
{
    if(state==GLUT_LEFT_BUTTON && choice == 0) {
        point[num].x = x;
        point[num].y = 500 - y;
        cout << num;
        num++;
        cout << "(" << point[num-1].x << "," << point[num-1].y << ")" << endl;
    }
    else if (state==GLUT_LEFT_BUTTON && choice == 7) {
        linepoint[linenum].x = x;
        linepoint[linenum].y = 500 - y;
        cout << linenum;
        linenum++;
        cout << "(" << linepoint[linenum-1].x << "," << linepoint[linenum-1].y << ")" << endl;
    }
    
}
int compareColor(Color color1, Color color2) {
    if (color1[0] != color2[0] || color1[1] != color2[1] || color1[2] != color2[2]) { return 0; }
    else { return 1; }
}
void ProcessMenu(int value)
{
    switch (value)
    {
        case 1:
            choice = 1;
            display();
            break;
        case 2:
            choice = 2;
            cout << 2 << endl;
            break;
        case 3:
            choice = 3;
            move();
            break;
        case 4:
            choice = 4;
            change2D();
            break;
        case 5:
            choice = 5;
            scale2D();
            break;
        case 6:
            choice = 6;
            rotate2D();
            break;
        case 7:
            choice = 7;
            break;
        case 8:
            choice = 8;
            point[3].x = wxl;point[3].y=500-wyt;
            point[2].x = wxr;point[2].y=500-wyt;
            point[1].x = wxr;point[1].y=500-wyb;
            point[0].x = wxl;point[0].y=500-wyb;
            for (int pp=0;pp<4;pp++){
                cout << point[pp].x << "," << point[pp].y <<endl;
            }
            num=4;
            break;
        case 9:
            choice = 9;
            display();
            break;
        case 10:
            choice = 10;
            break;
        case 11:
            choice = 11;
            //cut_line();
            Cohen();
            display();
            break;
        case 13:
            choice = 13;
            
            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
            glutInitWindowPosition(100, 100);
            glutInitWindowSize(400, 400);
            glutCreateWindow("HelloOpenGL");
            glutIdleFunc(&Update);
            glutDisplayFunc(&Draw);
            glutMainLoop();
            break;
        case 0:
            choice = 0;
            num = 0;
            linenum = 0;
            for (int i=0;i<100;i++){
                point[i].x = 0;
                point[i].y = 0;
                linepoint[i].x = 0;
                linepoint[i].y = 0;
            }
            break;
        default: break;
    }  
    //强制刷新  
    glutPostRedisplay();  
}
void getpixel(GLint x, GLint y, Color color) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}
void setpixel(GLint x, GLint y) {
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void boundaryFill4(int x, int y, Color fillColor, Color boarderColor) {
    Color interiorColor;
    getpixel(x, y, interiorColor);
    if (compareColor(interiorColor, fillColor) == 0 && compareColor(interiorColor, boarderColor) == 0) {
        setpixel(x, y);
        boundaryFill4(x + 1, y, fillColor, boarderColor);
        boundaryFill4(x - 1, y, fillColor, boarderColor);
        boundaryFill4(x, y + 1, fillColor, boarderColor);
        boundaryFill4(x, y - 1, fillColor, boarderColor);
    }
    
}
int inOrNot(float x, float y)
{
    
    int i, j;
    
    j = num-1;
    
    int res = 0;
    
    for (i = 0; i<num; i++)
    {
        if(((point[i].y<y && point[j].y>=y) || (point[j].y<y && point[i].y>=y)) && (point[i].x<=x || point[j].x<=x))
        {
            res ^= ((point[i].x + (y-point[i].y)/(point[j].y-point[i].y)*(point[j].x-point[i].x)) < x);
        }
        j=i;
    }
    
    return res;
}
void move ()
{
    for (int i=0;i<num;i++){
        point[i].x=point[i].x + 100;
        point[i].y=point[i].y + 100;
    }
    display();
}
void rotate2D()
{
    int x = point[0].x;
    int y = point[0].y;
    int x1;
    int y1;
    cout << cos(0) << endl;
    cout <<sin(pi/2) << endl;
    for (int i=0;i<num;i++){
        cout << "old(" << point[i].x << "," << point[i].y << ")"<<endl;
        x1 = point[i].x;
        y1 = point[i].y;
        point[i].x=x + (x1 - x) * cos(pi/2) - (y1 - y) * sin(pi/2);
        point[i].y=y + (x1 - x) * sin(pi/2) + (y1 - y) * cos(pi/2);
        cout << "new(" << point[i].x << "," << point[i].y << ")"<<endl;
    }
    display();
}
void scale2D()
{
    int x = point[0].x;
    int y = point[0].y;
    double sx = 2;
    double sy = 2;
    for (int i=0;i<num;i++){
        point[i].x=point[i].x * sx + x * (1 - sx);
        point[i].y=point[i].y * sy + y * (1 - sy);
    }
    display();
}
void change2D()
{
    int x = point[0].x;
    int y = point[0].y;
    for (int i=0;i<num;i++){
        point[i].x = x - (point[i].x - x);
       // point[i].y = y - (point[i].y - y);
    }
    display();
}
void cut_line()
{
    int x1,x2,y1,y2;
    int flag1=0;//表示点1是否在多边形内
    int flag2=0;//表示点2是否在多边形内
    
    P jiaodian1;
    P jiaodian2;
    
    for (int i=0;i<linenum;i+=2){
        flag0=0;
        flag1=0;
        flag2=0;
        x1=linepoint[i].x;
        y1=linepoint[i].y;
        x2=linepoint[i+1].x;
        y2=linepoint[i+1].y;
        
        if (inOrNot(x1, y1)) {
            flag1=1;
        }
        if (inOrNot(x2, y2)) {
            flag2=1;
        }
        if (flag1==0&&flag2==0){
            for (int ik=0;ik<num;ik+=1){
                cout << "here0" <<endl;
                if (detectIntersect(point[(ik+1)%4],point[ik],linepoint[i],linepoint[i+1],i)){
                    cout << "here2" <<endl;
                    if(flag0==1){
                        jiaodian1.x=line_x;
                        jiaodian1.y=line_y;
                        cout << "flag0:"<<flag0<<endl;
                    } else if (flag0==2) {
                        jiaodian2.x=line_x;
                        jiaodian2.y=line_y;
                        cout << "flag0:"<<flag0<<endl;
                        break;
                    }
                }
            }
            if (flag0!=2){
                lineinsidepoint[lineinsidenum].x=0;
                lineinsidepoint[lineinsidenum].y=0;
                lineinsidepoint[lineinsidenum+1].x=0;
                lineinsidepoint[lineinsidenum+1].y=0;
                cout <<"线段在多边形外"<<endl;
            } else if (flag0==2) {
                lineinsidepoint[lineinsidenum].x=jiaodian1.x;
                lineinsidepoint[lineinsidenum].y=jiaodian1.y;
                lineinsidepoint[lineinsidenum+1].x=jiaodian2.x;
                lineinsidepoint[lineinsidenum+1].y=jiaodian2.y;
                cout << "线段与多边形有两个交点"<<endl;
            }
        } else if (flag1==1&&flag2==1){
            lineinsidepoint[lineinsidenum].x=x1;
            lineinsidepoint[lineinsidenum].y=y1;
            lineinsidepoint[lineinsidenum+1].x=x2;
            lineinsidepoint[lineinsidenum+1].y=y2;
            cout <<"线段在多边形内"<<endl;
        } else if (flag1==0 && flag2==1) {
            for (int ik=0;ik<num;ik+=1){//循环边 表示point[ik]不在多边形内
                if (detectIntersect(point[ik],point[(ik+1)%4],linepoint[i],linepoint[i+1],i)){
                    lineinsidepoint[lineinsidenum].x=linepoint[(ik+1)%4].x;
                    lineinsidepoint[lineinsidenum].y=linepoint[(ik+1)%4].y;
                    lineinsidepoint[lineinsidenum+1].x=line_x;
                    lineinsidepoint[lineinsidenum+1].y=line_y;
                    cout <<"线段一个点在多边形内，一个点在多边形外"<<endl;
                    break;
                }
            }
        } else if (flag1==1 && flag2==0){
            for (int ik=0;ik<num;ik+=2){//循环边 表示point[ik+1]不在多边形内
                if (detectIntersect(point[(ik+1)%4],point[ik],linepoint[i],linepoint[i+1],i)){
                    lineinsidepoint[lineinsidenum].x=linepoint[ik].x;
                    lineinsidepoint[lineinsidenum].y=linepoint[ik].y;
                    lineinsidepoint[lineinsidenum+1].x=line_x;
                    lineinsidepoint[lineinsidenum+1].y=line_y;
                    cout <<"线段一个点在多边形内，一个点在多边形外"<<endl;
                    break;
                }
            }
        }
        lineinsidenum+=2;
        cout << "裁剪完的线段点1(" << lineinsidepoint[lineinsidenum-2].x << "," << lineinsidepoint[lineinsidenum-2].y << ")" << endl;
        cout << "裁剪完的线段点2(" << lineinsidepoint[lineinsidenum-1].x << "," << lineinsidepoint[lineinsidenum-1].y << ")" << endl;
    }
}


bool between(double a, double X0, double X1)
{
    double temp1= a-X0;
    double temp2= a-X1;
    if ( ( temp1<1e-8 && temp2>-1e-8 ) || ( temp2<1e-6 && temp1>-1e-8 ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}


// 判断两条直线段是否有交点，有则计算交点的坐标
// p1,p2是直线一的端点坐标
// p3,p4是直线二的端点坐标
bool detectIntersect(P p1, P p2, P p3, P p4, int i)
{
    //P2在多边形内
    cout <<"p1.x-p2.x"<<fabs((double)(p1.x-p2.x))<<endl;
    cout << "p1("<<p1.x<<","<<p1.y <<")"<<endl;
    cout << "p2("<<p2.x<<","<<p2.y <<")"<<endl;
    if ( (fabs((double)(p1.x-p2.x))<1e-6) && (fabs((double)(p3.x-p4.x))<1e-6) )
    {
        cout << "here1"<<endl;
        return false;
    }
    
    else if ( (fabs((double)(p1.x-p2.x))<1e-6) ) //如果直线段p1p2垂直与y轴
    {
        cout << "如果直线段p1p2垂直与y轴" <<endl;
        if (between(p1.x,p3.x,p4.x))
        {
            double k = (double)(p4.y-p3.y)/(p4.x-p3.x);
            line_x = (double)p1.x;
            line_y = (double)k*(line_x-p3.x)+p3.y;
            
            if (between(line_y,p1.y,p2.y))
            {
                cout << "交点(" << line_x << "," << line_y << ")" << endl;
                flag0++;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else if ( (fabs((double)(p3.x-p4.x))<1e-6) ) //如果直线段p3p4垂直与y轴
    {
        cout <<"如果直线段p3p4垂直与y轴"<<endl;
        if (between(p3.x,p1.x,p2.x))
        {
            double k = (double)(p2.y-p1.y)/(p2.x-p1.x);
            line_x = (double)p3.x;
            line_y = (double)k*(line_x-p2.x)+p2.y;
            
            if (between(line_y,p3.y,p4.y))
            {
                cout << "交点(" << line_x << "," << line_y << ")" << endl;
                flag0++;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        cout << "正常"<<endl;
        double k1 = (double)(p2.y-p1.y)/(p2.x-p1.x);
        double k2 = (double)(p4.y-p3.y)/(p4.x-p3.x);
        cout << "斜率k1:" << k1 <<endl;
        cout << "斜率k2:" << k2 <<endl;
        if (fabs(k1-k2)<1e-6)
        {
            return false;
        }
        else
        {
            line_x = (double)((p3.y - p1.y) - (k2*p3.x - k1*p1.x)) / (k1-k2);
            line_y = (double)k1*(line_x-p1.x)+p1.y;
            cout << line_x << ","<<line_y <<endl;
        }
        
        if (between(line_x,p1.x,p2.x)&&between(line_x,p3.x,p4.x))
        {
            cout << "交点(" << line_x << "," << line_y << ")" << endl;
            flag0++;
            return true;
        }
        else
        {
            return false;
        }  
    }  
}



