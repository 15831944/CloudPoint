#include<iostream>
#include<iomanip>

using namespace std;


const int Number = 20;			//����������

class equation
{
private:
	int lenth,copy_lenth;			//���̸���
	char * x;										//δ֪��a,b,c������
	//double a[Number][Number], b[Number], copy_a[Number][Number], copy_b[Number];			////ϵ������ʽ(a����ϵ��, b�Ҷ˳���)
public:
	//equation(int _lenth = 0);			//���캯��
	//void input();			//��������
	double a[Number][Number], b[Number], copy_a[Number][Number], copy_b[Number];			////ϵ������ʽ(a����ϵ��, b�Ҷ˳���)
	void input(int l, double **m);			//��������
	void gauss_all();								//Gaussȫ��Ԫ�ⷽ����
	void gauss_all_xiaoqu();						//Gaussȫ��Ԫ��ȥ��
	void gauss_calculate();							//����Gauss��ȥ���������δ֪����ֵ
	void exchange_hang(int m,int n);				//�ֱ𽻻�a[][]��b[]�е�m��n����
	void exchange_a_lie(int m,int n);				//����a[][]�е�m��n��
	void exchange_x(int m,int n);					//����x[]�е�x[m]��x[n]
};

//equation::equation(int _lenth)
//{
//	lenth = _lenth;
//}

//----------------------------��ʼ������Ϊ0
//void equation::input()
//{	
//	int i,j;
//	//if(lenth == 0)
//	//{
//		cout<<"���̵ĸ���:";
//		cin>>lenth;
//	//}
//	if(lenth > Number)
//	{	
//		cout<<"It is too big.\n";
//		return;	
//	}
//	x = new char[lenth];	
//	//--------------��ʼ����������ΪĬ�ϵ�a,b,c.. 
//	for(i = 0;i < lenth;i++)
//		x[i] = 'a' + i;
//
//	//----------------���뷽�̾���
//	//��ʾ�������
//	//cout<<"====================================================\n";
//	//cout<<"����ÿ������������"<<lenth<<"ϵ����һ������:\n";
//	//	cout<<"����\n����:a";
//	//for(i = 1;i < lenth;i++)
//	//{
//	//	cout<<"+"<<i + 1<<x[i];
//	//}
//	//cout<<"=10\n";
//	//cout<<"Ӧ����:";
//	//for(i = 0;i < lenth;i++)
//	//	cout<<i + 1<<" ";
//	//cout<<"10\n";
//	//cout<<"==============================\n";
//
//	
//	//����ÿ������
//	for(i = 0;i < lenth;i++)
//	{	
//		cout<<"���뷽��"<<i + 1<<":";
//		for(j = 0;j < lenth;j++)
//			cin>>a[i][j];
//		cin>>b[i];
//	}
//	
//	//��������
//	for(i=0;i<lenth;i++)
//		for(j=0;j<lenth;j++)
//		copy_a[i][j]=a[i][j];
//	for(i=0;i<lenth;i++)
//		copy_b[i]=b[i];
//	copy_lenth=lenth;
//}

void equation::input(int l, double **m)
{	
	int i,j;
	lenth = l;
	if(lenth > Number)
	{	
		cout<<"It is too big.\n";
		return;	
	}
	x = new char[lenth];	
	//--------------��ʼ����������ΪĬ�ϵ�a,b,c.. 
	for(i = 0;i < lenth;i++)
		x[i] = 'a' + i;

	//����ÿ������
	for(i = 0;i < lenth;i++)
	{	
		for(j = 0;j < lenth;j++)
			a[i][j] = *(*(m+i)+j);
		b[i] =  *(*(m+i)+l);
	}
	
	//��������
	for(i=0;i<lenth;i++)
		for(j=0;j<lenth;j++)
		copy_a[i][j]=a[i][j];
	for(i=0;i<lenth;i++)
		copy_b[i]=b[i];
	copy_lenth=lenth;
}


//��˹ȫ��Ԫ������ⷽ��
void equation::gauss_all()
{
	int i,j;
	gauss_all_xiaoqu();
	//for(i = 0;i < lenth;i++)
	//{
	//	for(j = 0;j < lenth;j++)
	//		cout<<setw(10)<<setprecision(5)<<a[i][j];
	//	cout<<setw(10)<<b[i]<<endl;
	//}
	if(a[lenth - 1][lenth - 1] != 0)
	{
	
		//cout<<"ϵ������ʽ��Ϊ��,������Ψһ�Ľ⣺\n";
		gauss_calculate();

		//for(i = 0;i < lenth;i++)			//������
		//{	
		//	for(j = 0;x[j] != 'a'+i && j < lenth;j++);
		//		
		//	
		//	cout<<x[j]<<"="<<b[j]<<endl;
		//}
	}
	else
		cout<<"ϵ������ʽ������,����û��Ψһ�Ľ�.\n";
}


void equation::gauss_all_xiaoqu()			//Gaussȫ��Ԫ��ȥ��
{
	int i,j,k,maxi,maxj;double lik;
	//cout<<"��Gaussȫ��Ԫ��ȥ���������:\n";

	for(k = 0;k < lenth - 1;k++)
	{	
		
		for(maxi = maxj = i = k;i <lenth;i++)
		{	
			for(j = k;j < lenth;j++)
			if(a[i][j] > a[maxi][ maxj])	
			{	maxi = i;
				maxj = j;
			}
		
		}	
		if(maxi != k)	
			exchange_hang(k,maxi);
		if(maxj != k)
		{
			exchange_a_lie(maxj,k);				//��������
			exchange_x(maxj,k);

		}
		
		for(i = k + 1;i < lenth;i++)
		{	
			lik = a[i][k]/a[k][k];
			for(j = k;j < lenth;j++)
				a[i][j] = a[i][j] - a[k][j] * lik;
			b[i] = b[i] - b[k] * lik;
		}
	}
}


void equation::gauss_calculate()				//��˹��ȥ���Ժ����δ֪���Ľ��
{	
	int i,j;double sum_ax;
	b[lenth - 1] = b[lenth - 1]/a[lenth - 1][lenth - 1];
	for(i = lenth - 2;i >= 0;i--)
	{	
		for(j = i + 1,sum_ax = 0;j < lenth;j++)
			sum_ax += a[i][j] * b[j];
		b[i] = (b[i] - sum_ax)/a[i][i];
	}
}


void equation::exchange_hang(int m,int n)		//����a[][]�к�b[]����
{	
	int j;	double temp;
	for(j = 0;j < lenth;j++)
	{	temp = a[m][j];
		a[m][j] = a[n][j];
		a[n][j] = temp;
		
	}
	temp = b[m];
	b[m] = b[n];
	b[n] = temp;
}


void equation::exchange_a_lie(int m,int n)			//����a[]�е�����
{	double temp;int i;
	for(i = 0;i < lenth;i++)
	{	temp = a[i][m];
		a[i][m] = a[i][n];
		a[i][n] = temp;
	}
}


void equation::exchange_x(int m,int n)				//����δ֪��x[m]��x[n]
{	char temp;
	temp = x[m];
	x[m] = x[n];
	x[n] = temp;
}
