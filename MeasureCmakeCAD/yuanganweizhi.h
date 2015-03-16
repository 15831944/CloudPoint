#include"equation.h"

#include<cstdio> 
#include<algorithm> 
#include<math.h> 


const int maxn = 1000; 
struct point			//定义点数据结构   ---Point2d Yibing 2014/12/24 
{
	double x, y;
}; 


struct circle			//定义圆数据结构   ---Circle Yibing 2014/12/24 
{
	point cen;			//圆心
	double r;			//半径
};//cir[maxn]; 

point st;			//圆外一点

 double distan(point p1,point p2)				//两点间距离
{ 
	return sqrt((p1.x  -p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)); 
 }

//rotation theta with clock or anticlock ----yibing 2014.12.24
void cirinser(circle c, point &inser1, point &inser2)			//圆与切线交点(保证st在圆外) 
{
	point dir;			//
    double dis = distan(c.cen, st);			//圆心到圆外一点距离，长边c
    double temp = sqrt( dis * dis - c.r * c.r );			//直角三角形（短边a = r, b, 长边c），temp为b
    double sina = temp / dis;			//
    double cosa = c.r / dis; 
    dir.x = ( st.x - c.cen.x ) / dis * c.r; 
    dir.y = ( st.y - c.cen.y ) / dis * c.r; 
    inser1.x = c.cen.x + ( dir.x * cosa - dir.y * sina ); 
    inser1.y = c.cen.y + ( dir.x * sina + dir.y * cosa ); 
      
    inser2.x = c.cen.x + ( dir.x * cosa + dir.y * sina ); 
    inser2.y = c.cen.y + ( -dir.x * sina + dir.y * cosa ); 
} 
