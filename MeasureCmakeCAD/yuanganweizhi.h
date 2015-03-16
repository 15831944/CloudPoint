#include"equation.h"

#include<cstdio> 
#include<algorithm> 
#include<math.h> 


const int maxn = 1000; 
struct point			//��������ݽṹ   ---Point2d Yibing 2014/12/24 
{
	double x, y;
}; 


struct circle			//����Բ���ݽṹ   ---Circle Yibing 2014/12/24 
{
	point cen;			//Բ��
	double r;			//�뾶
};//cir[maxn]; 

point st;			//Բ��һ��

 double distan(point p1,point p2)				//��������
{ 
	return sqrt((p1.x  -p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)); 
 }

//rotation theta with clock or anticlock ----yibing 2014.12.24
void cirinser(circle c, point &inser1, point &inser2)			//Բ�����߽���(��֤st��Բ��) 
{
	point dir;			//
    double dis = distan(c.cen, st);			//Բ�ĵ�Բ��һ����룬����c
    double temp = sqrt( dis * dis - c.r * c.r );			//ֱ�������Σ��̱�a = r, b, ����c����tempΪb
    double sina = temp / dis;			//
    double cosa = c.r / dis; 
    dir.x = ( st.x - c.cen.x ) / dis * c.r; 
    dir.y = ( st.y - c.cen.y ) / dis * c.r; 
    inser1.x = c.cen.x + ( dir.x * cosa - dir.y * sina ); 
    inser1.y = c.cen.y + ( dir.x * sina + dir.y * cosa ); 
      
    inser2.x = c.cen.x + ( dir.x * cosa + dir.y * sina ); 
    inser2.y = c.cen.y + ( -dir.x * sina + dir.y * cosa ); 
} 
