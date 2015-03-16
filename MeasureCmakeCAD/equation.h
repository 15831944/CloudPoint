#include<iostream>
#include<iomanip>

using namespace std;


const int Number = 20;			//方程最大个数

class equation
{
private:
	int lenth,copy_lenth;			//方程个数
	char * x;										//未知量a,b,c的载体
	//double a[Number][Number], b[Number], copy_a[Number][Number], copy_b[Number];			////系数行列式(a方程系数, b右端常数)
public:
	//equation(int _lenth = 0);			//构造函数
	//void input();			//输入数据
	double a[Number][Number], b[Number], copy_a[Number][Number], copy_b[Number];			////系数行列式(a方程系数, b右端常数)
	void input(int l, double **m);			//输入数据
	void gauss_all();								//Gauss全主元解方程组
	void gauss_all_xiaoqu();						//Gauss全主元消去法
	void gauss_calculate();							//根据Gauss消去法结果计算未知量的值
	void exchange_hang(int m,int n);				//分别交换a[][]和b[]中的m与n两行
	void exchange_a_lie(int m,int n);				//交换a[][]中的m和n列
	void exchange_x(int m,int n);					//交换x[]中的x[m]和x[n]
};

//equation::equation(int _lenth)
//{
//	lenth = _lenth;
//}

//----------------------------初始化数据为0
//void equation::input()
//{	
//	int i,j;
//	//if(lenth == 0)
//	//{
//		cout<<"方程的个数:";
//		cin>>lenth;
//	//}
//	if(lenth > Number)
//	{	
//		cout<<"It is too big.\n";
//		return;	
//	}
//	x = new char[lenth];	
//	//--------------初始化变量符号为默认的a,b,c.. 
//	for(i = 0;i < lenth;i++)
//		x[i] = 'a' + i;
//
//	//----------------输入方程矩阵
//	//提示如何输入
//	//cout<<"====================================================\n";
//	//cout<<"请在每个方程里输入"<<lenth<<"系数和一个常数:\n";
//	//	cout<<"例：\n方程:a";
//	//for(i = 1;i < lenth;i++)
//	//{
//	//	cout<<"+"<<i + 1<<x[i];
//	//}
//	//cout<<"=10\n";
//	//cout<<"应输入:";
//	//for(i = 0;i < lenth;i++)
//	//	cout<<i + 1<<" ";
//	//cout<<"10\n";
//	//cout<<"==============================\n";
//
//	
//	//输入每个方程
//	for(i = 0;i < lenth;i++)
//	{	
//		cout<<"输入方程"<<i + 1<<":";
//		for(j = 0;j < lenth;j++)
//			cin>>a[i][j];
//		cin>>b[i];
//	}
//	
//	//备份数据
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
	//--------------初始化变量符号为默认的a,b,c.. 
	for(i = 0;i < lenth;i++)
		x[i] = 'a' + i;

	//输入每个方程
	for(i = 0;i < lenth;i++)
	{	
		for(j = 0;j < lenth;j++)
			a[i][j] = *(*(m+i)+j);
		b[i] =  *(*(m+i)+l);
	}
	
	//备份数据
	for(i=0;i<lenth;i++)
		for(j=0;j<lenth;j++)
		copy_a[i][j]=a[i][j];
	for(i=0;i<lenth;i++)
		copy_b[i]=b[i];
	copy_lenth=lenth;
}


//高斯全主元排列求解方程
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
	
		//cout<<"系数行列式不为零,方程有唯一的解：\n";
		gauss_calculate();

		//for(i = 0;i < lenth;i++)			//输出结果
		//{	
		//	for(j = 0;x[j] != 'a'+i && j < lenth;j++);
		//		
		//	
		//	cout<<x[j]<<"="<<b[j]<<endl;
		//}
	}
	else
		cout<<"系数行列式等于零,方程没有唯一的解.\n";
}


void equation::gauss_all_xiaoqu()			//Gauss全主元消去法
{
	int i,j,k,maxi,maxj;double lik;
	//cout<<"用Gauss全主元消去法结果如下:\n";

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
			exchange_a_lie(maxj,k);				//交换两列
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


void equation::gauss_calculate()				//高斯消去法以后计算未知量的结果
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


void equation::exchange_hang(int m,int n)		//交换a[][]中和b[]两行
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


void equation::exchange_a_lie(int m,int n)			//交换a[]中的两列
{	double temp;int i;
	for(i = 0;i < lenth;i++)
	{	temp = a[i][m];
		a[i][m] = a[i][n];
		a[i][n] = temp;
	}
}


void equation::exchange_x(int m,int n)				//交换未知量x[m]与x[n]
{	char temp;
	temp = x[m];
	x[m] = x[n];
	x[n] = temp;
}
