#include <vector>
#include"yuanganweizhi.h"

#include<stdlib.h>
#include "windows.h"
using namespace std;

//void Xpcompute();

class XP
{
public:
	//point L1, L2;
	double L;//杆长
	int vertex_count;//装配步长
	double temp;			//与间隙有关常量
	point p;			//点P
	circle cc;			//点O，及以杆内径D为半径的圆
	double pi;
	void Xpcompute();
};


void XP::Xpcompute()
{
	//解方程的写法********************************
	//equation a;
	//int x, y;
	//x = 2;
	//y = 3;
	//double **m = new double *[x];
	//for(int i = 0;i < x;i++)
	//{
	//	 m[i] = new double [y];
	//}
	//for(int i = 0; i < x; i++)
	//{
	//	for(int j = 0; j < y; j++)
	//	{
	//		cin>>m[i][j] ;
	//	}
	//}
	//a.input(2, m);
	//a.gauss_all();
	//cin.get();
	//********************************************

	printf("设定间隙要求及模拟步长，输出干涉数据信息（0为不干涉，1为干涉）\n***************************************************\n");

	//根据杆上两点计算杆长 *******************************
	//point L1, L2;
	//L1.x = 48.10;
	//L1.y = 880.62;
	//L2.x = -300.17;
	//L2.y = 306.16;
	//double L = distan(L1, L2);
	//cout<<L<<endl;
	//*************************************************


	//int vertex_count=100;			//装配步长
	vector<int> ganshejiancha(vertex_count);			//干涉检查0、1储存
	vector<double> ganshehoudu(vertex_count);			//干涉厚度


	cout<<"读取桁架结构信息……"<<endl;
	cout<<"确定初始位置及运动轨迹……"<<endl;


	//double temp = 0.18;			//与间隙有关常量
	//point p;			//点P
	//p.x = -277.22 + 1.64085 * temp;
	//p.y = 342.02 - temp;
	
	//should not use gloable parameter both st and const  Yibing 2014/12/24   
	st.x = 29.42 + 1.64085 * temp;			//点N
	st.y = 845.17 - temp;

	//circle cc;			//点O，及以杆内径D为半径的圆
	//cc.cen.x = -247.33 - 1.64085 * temp;
	//cc.cen.y = 323.80 + temp;
	//cc.r = 35; 

	//为了观察蒜头外径**********************
	//point ccc;
	//ccc.x = -247.33 - 1.64085 * temp;
	//ccc.y = 323.80 + temp;
	//double l = distan(p, ccc);
	//cout<<l<<endl;
	//*************************************

	//构造榫头直线方程****************************
	double k1;		//榫头所在直线斜率
	k1 = (st.y - p.y) / (st.x - p.x);
	//cout<<p.x<<" "<<st.x<<" "<<k1<<endl;
	double A1, B1, C1;
	A1 = k1;
	B1 = -1;
	C1 = k1 * p.x - p.y;
	//********************************************

	double t1[3][3] = { { 1, 0, 0 }, {0, 1, 0 }, {0, 0, 1} },	
	           t2[3][3] = { { 1, 0, 0 }, {0, 1, 0 }, {0, 0, 1} }, 
               t3[3][3] = { { 1, 0, 0 }, {0, 1, 0 }, {0, 0, 1} },
			   t4[3][3] = { { 1, 0, 0 }, {0, 1, 0 }, {0, 0, 1} },
			   t[3][3];
	double jiajiao1 = atan(k1), jiajiao2;
	double zhuanjiao;
	//double sinjiajaio1, cosjiajiao2, sinjiajiao2, sinzhuanjiao;
	point zhuanhuan1, zhuanhuan2; 
	zhuanhuan1.x = st.x + (L - 589) * (1 / (k1 * k1 + 1) );
	zhuanhuan1.y = st.y + (L - 589) * (k1 / (k1 * k1 + 1) );

	FILE* pFileganshe=fopen("干涉检查.txt","w");

	//主循环，每一步长的计算*****************************
	for(int i = 0; i < vertex_count; i++)
	{
		point inser1,inser2, inser;			//切点，两个切点1、2取其中之一，为M点
		cirinser(cc,inser1,inser2);
		if(inser1.y > inser2.y)
			inser = inser1;
		if(inser1.y < inser2.y)
			inser = inser2;
		//cout<<"切点信息"<<inser1.x<<"  "<<inser1.y<<"  "<<inser2.x<<"  "<<inser2.y<<"  "<<inser.x<<"  "<<inser.y<<endl;
		double MN = distan(inser, st);			//MN长度
		//cout<<"MN长"<<MN<<endl;
		//cout<<"杆长"<<L<<endl;
		double k2;			//杆所在直线斜率
		k2 = (st.y - inser.y) / (st.x - inser.x);
		double A2, B2, C2;
		A2 = k2;
		B2 = -1;
		C2 = k2 * inser.x - inser.y;			//构造杆直线方程

		jiajiao2 = atan(k2);
		zhuanjiao = jiajiao2 - jiajiao1;
		zhuanhuan2.x = st.x + (L - MN) * (1 / (k2 * k2 + 1) );
		zhuanhuan2.y = st.y + (L - MN) * (k2 / (k2 * k2 + 1) );
		if(MN > L)
			zhuanhuan2 = st;
		t1[2][0] = -zhuanhuan1.x;
		t1[2][1] = -zhuanhuan1.y;
		t2[0][0] = cos(zhuanjiao);
		t2[0][1] = sin(zhuanjiao);
		t2[1][0] = -sin(zhuanjiao);
		t2[1][1] = cos(zhuanjiao);
		t3[2][0] = zhuanhuan1.x;
		t3[2][1] = zhuanhuan1.y;
		t4[2][0] = zhuanhuan2.x - zhuanhuan1.x;
		t4[2][1] = zhuanhuan2.y - zhuanhuan1.y;
		t[0][0] = t2[0][0];
		t[0][1] = t2[0][1];
		t[0][2] = 0;
		t[1][0] = t2[1][0];
		t[1][1] = t2[1][1];
		t[1][2] = 0;
		t[2][0] = t1[2][0] * t2[0][0] + t1[2][1] * t2[1][0] + t3[2][0] + t4[2][0];
		t[2][1] = t1[2][0] * t2[0][1] + t1[2][1] * t2[1][1] + t3[2][1] + t4[2][1];
		t[2][2] = 1;

		//解方程，求榫头直线与杆直线交点****************************
		equation EQ;
		int x, y;
		x = 2;			//二元
		y = 3;			//三系数
		double **m = new double *[x];
		for(int i = 0;i < x;i++)
		{
			 m[i] = new double [y];
		}
		m[0][0] = A1;
		m[0][1] = B1;
		m[0][2] = C1;
		m[1][0] = A2;
		m[1][1] = B2;
		m[1][2] = C2;
		EQ.input(2, m);
		EQ.gauss_all();
		//*****************************************************

		point jiaodian;			//交点Q
		jiaodian.x = EQ.b[0];
		jiaodian.y = EQ.b[1];

		double panding = distan(jiaodian, inser);			//交点到切点距离PQ
		//cout<<"判定长度"<<panding<<endl;
		double ganshehoudunew;			//干涉厚度

		//干涉判断*******************************************
		if(panding * 2 + MN <= L)
		{
			ganshejiancha[i] = 1;
			double temp1, temp2, temp3, temp4, temp5;
			temp1 = (L - MN) / 2;
			//cout<<temp1<<endl;
			temp2 = temp1 - panding;
			//cout<<temp2<<endl;
			temp3 = k2 * temp2;
			//cout<<temp3<<endl;
			temp4 = k2 * k2 + 1;
			//cout<<temp4<<endl;
			temp5 = sqrt(temp4);
			//cout<<temp5<<endl;
			ganshehoudunew = temp3 / temp5;
			//cout<<ganshehoudunew<<endl;
			ganshehoudu[i] = ganshehoudunew;

		}
		else
		{
			ganshejiancha[i] = 0;
			ganshehoudu[i] = 0;
		}
		//*****************************************************

		fprintf(pFileganshe, "%d\t%d\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", i, ganshejiancha[i], ganshehoudu[i]/4, ganshehoudu[i]/3.5, t[0][0], t[0][1], t[0][2], t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2]);

		jiajiao1 = jiajiao2;
		zhuanhuan1 = zhuanhuan2;
		st.y = st.y +100 / vertex_count;

	}


	printf("装配过程分割为%d步进行 ……\n", vertex_count);
	printf("分析装配结构状态 ...\n");
	printf("干涉检查 ...\n");
	printf("写入干涉点信息数据 ...\n");
	
	//FILE* pFileganshe=fopen("干涉检查.txt","w");
	//for (int i = 0; i < vertex_count; i++)
	//{
	//	fprintf(pFileganshe, "%d\t%d\t%lf\n", i, ganshejiancha[i], ganshehoudu[i]/4);
	//}	
	fclose(pFileganshe);

	printf("程序处理完毕 ...\n");
	//cin.get();
}