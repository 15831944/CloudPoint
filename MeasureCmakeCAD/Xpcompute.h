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
	double L;//�˳�
	int vertex_count;//װ�䲽��
	double temp;			//���϶�йس���
	point p;			//��P
	circle cc;			//��O�����Ը��ھ�DΪ�뾶��Բ
	double pi;
	void Xpcompute();
};


void XP::Xpcompute()
{
	//�ⷽ�̵�д��********************************
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

	printf("�趨��϶Ҫ��ģ�ⲽ�����������������Ϣ��0Ϊ�����棬1Ϊ���棩\n***************************************************\n");

	//���ݸ����������˳� *******************************
	//point L1, L2;
	//L1.x = 48.10;
	//L1.y = 880.62;
	//L2.x = -300.17;
	//L2.y = 306.16;
	//double L = distan(L1, L2);
	//cout<<L<<endl;
	//*************************************************


	//int vertex_count=100;			//װ�䲽��
	vector<int> ganshejiancha(vertex_count);			//������0��1����
	vector<double> ganshehoudu(vertex_count);			//������


	cout<<"��ȡ��ܽṹ��Ϣ����"<<endl;
	cout<<"ȷ����ʼλ�ü��˶��켣����"<<endl;


	//double temp = 0.18;			//���϶�йس���
	//point p;			//��P
	//p.x = -277.22 + 1.64085 * temp;
	//p.y = 342.02 - temp;
	
	//should not use gloable parameter both st and const  Yibing 2014/12/24   
	st.x = 29.42 + 1.64085 * temp;			//��N
	st.y = 845.17 - temp;

	//circle cc;			//��O�����Ը��ھ�DΪ�뾶��Բ
	//cc.cen.x = -247.33 - 1.64085 * temp;
	//cc.cen.y = 323.80 + temp;
	//cc.r = 35; 

	//Ϊ�˹۲���ͷ�⾶**********************
	//point ccc;
	//ccc.x = -247.33 - 1.64085 * temp;
	//ccc.y = 323.80 + temp;
	//double l = distan(p, ccc);
	//cout<<l<<endl;
	//*************************************

	//�����ͷֱ�߷���****************************
	double k1;		//�ͷ����ֱ��б��
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

	FILE* pFileganshe=fopen("������.txt","w");

	//��ѭ����ÿһ�����ļ���*****************************
	for(int i = 0; i < vertex_count; i++)
	{
		point inser1,inser2, inser;			//�е㣬�����е�1��2ȡ����֮һ��ΪM��
		cirinser(cc,inser1,inser2);
		if(inser1.y > inser2.y)
			inser = inser1;
		if(inser1.y < inser2.y)
			inser = inser2;
		//cout<<"�е���Ϣ"<<inser1.x<<"  "<<inser1.y<<"  "<<inser2.x<<"  "<<inser2.y<<"  "<<inser.x<<"  "<<inser.y<<endl;
		double MN = distan(inser, st);			//MN����
		//cout<<"MN��"<<MN<<endl;
		//cout<<"�˳�"<<L<<endl;
		double k2;			//������ֱ��б��
		k2 = (st.y - inser.y) / (st.x - inser.x);
		double A2, B2, C2;
		A2 = k2;
		B2 = -1;
		C2 = k2 * inser.x - inser.y;			//�����ֱ�߷���

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

		//�ⷽ�̣����ͷֱ�����ֱ�߽���****************************
		equation EQ;
		int x, y;
		x = 2;			//��Ԫ
		y = 3;			//��ϵ��
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

		point jiaodian;			//����Q
		jiaodian.x = EQ.b[0];
		jiaodian.y = EQ.b[1];

		double panding = distan(jiaodian, inser);			//���㵽�е����PQ
		//cout<<"�ж�����"<<panding<<endl;
		double ganshehoudunew;			//������

		//�����ж�*******************************************
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


	printf("װ����̷ָ�Ϊ%d������ ����\n", vertex_count);
	printf("����װ��ṹ״̬ ...\n");
	printf("������ ...\n");
	printf("д��������Ϣ���� ...\n");
	
	//FILE* pFileganshe=fopen("������.txt","w");
	//for (int i = 0; i < vertex_count; i++)
	//{
	//	fprintf(pFileganshe, "%d\t%d\t%lf\n", i, ganshejiancha[i], ganshehoudu[i]/4);
	//}	
	fclose(pFileganshe);

	printf("��������� ...\n");
	//cin.get();
}