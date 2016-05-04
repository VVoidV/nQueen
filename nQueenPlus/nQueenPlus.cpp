// nQueen.cpp : 定义控制台应用程序的入口点。
//

#include<iostream>
#include <queue>
#include<fstream>
#include<vector>
#include<time.h>


using namespace std;


int obstacleX;
int obstacleY;
int nQueen;

/*struct pos {
int x;
int y;
};*/

struct queenNode {
	int h;
	vector<int> q;
	vector<int> col;
	vector<int> d1;//次对角线
	vector<int> d2;//主对角线
};

inline void Swap(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

void setqueen(const int n, queenNode &queen)
{
	//(i,queen.q[i])
	queen.q.resize(n);

	for (int i = 0; i < nQueen; i++)
	{
		queen.q[i] = i;
	}

	for (int i = 0; i < nQueen - 1; i++)
	{

		Swap(&queen.q[i], &queen.q[rand()%(i+1)]);
	}
	/*for (int i = 0; i < nQueen; i++)
	{
		cout << queen.q[i] <<endl;
	}*/
	for (int i = 0; i<n; i++)
	{
		queen.col[queen.q[i]]++;//列			
		queen.d2[i - queen.q[i] + (n - 1)]++;//主对角线			
		queen.d1[i + queen.q[i]]++;			//次对角线	
	}
	//检查列
	for (int i = 0; i<n; i++)
	{
		queen.h += (queen.col[i] * (queen.col[i] - 1)) / 2;
	}
	//检查对角线
	for (int i = 0; i < 2 * n - 1; i++)
	{
		queen.h += (queen.d1[i] * (queen.d1[i] - 1)) / 2;
		queen.h += (queen.d2[i] * (queen.d2[i] - 1)) / 2;
	}
}

void QS(void)
{
	int restart = 0;
	int oldH;
	int swapPerformed;
	queenNode queen;
	queen.col.resize(nQueen);
	queen.d1.resize(2 * nQueen - 1);
	queen.d2.resize(2 * nQueen - 1);
	do 
	{
		queen.h = 0;
		queen.d1.assign(queen.d1.size(), 0);
		queen.d2.assign(queen.d2.size(), 0);
		queen.col.assign(queen.col.size(), 0);
		setqueen(nQueen, queen);
		do
		{
			swapPerformed = 0;
			for (int i = 0; i < nQueen; i++)
			{
				for (int j = i + 1; j < nQueen; j++)
				{
					oldH = queen.h;
					if (queen.d1[i + queen.q[i]] > 1 || queen.d2[i - queen.q[i] + (nQueen - 1)] > 1
						|| queen.d1[j + queen.q[j]] > 1 || queen.d2[j - queen.q[j] + (nQueen - 1)] > 1)
					{
						//拿掉i行和j行的皇后
						queen.h = queen.h - (queen.d1[i + queen.q[i]] - 1) - (queen.d1[j + queen.q[j]] - 1)
							- (queen.d2[i - queen.q[i] + nQueen - 1] - 1) - (queen.d2[j - queen.q[j] + nQueen - 1]);
						//尝试交换
						queen.d1[i + queen.q[i]]--;
						queen.d1[i + queen.q[j]]++;
						queen.d2[i - queen.q[i] + (nQueen - 1)]--;
						queen.d2[i - queen.q[j] + (nQueen - 1)]++;
						queen.d1[j + queen.q[j]]--;
						queen.d1[j + queen.q[i]]++;
						queen.d2[j - queen.q[j] + (nQueen - 1)]--;
						queen.d2[j - queen.q[i] + (nQueen - 1)]++;
						Swap(&queen.q[i], &queen.q[j]);
						//计算新的冲突数
						queen.h = queen.h + (queen.d1[i + queen.q[i]] - 1) + (queen.d1[j + queen.q[j]] - 1)
							+ (queen.d2[i - queen.q[i] + nQueen - 1] - 1) + (queen.d2[j - queen.q[j] + nQueen - 1]);

						if (oldH <= queen.h)
						{
							//冲突未减小，放回去
							queen.d1[i + queen.q[i]]--;
							queen.d1[i + queen.q[j]]++;
							queen.d2[i - queen.q[i] + (nQueen - 1)]--;
							queen.d2[i - queen.q[j] + (nQueen - 1)]++;
							queen.d1[j + queen.q[j]]--;
							queen.d1[j + queen.q[i]]++;
							queen.d2[j - queen.q[j] + (nQueen - 1)]--;
							queen.d2[j - queen.q[i] + (nQueen - 1)]++;
							Swap(&queen.q[i], &queen.q[j]);
							queen.h = oldH;
						}
						else
						{
							swapPerformed++;
						}
					}
				}
			}

		} while (swapPerformed != 0);
		restart++;
	} while (queen.h != 0);
	ofstream output("QS_RESULT.txt");
	for (int i = 0; i < nQueen; i++)
	{
		output << queen.q[i] << endl;
	}
	cout << restart << endl;
	output.close();
}



int main()
{
	ifstream input("input.txt");
	ofstream output("output.txt");
	queenNode current, neighboor;


	if (!input)
	{
		cout << "can not open input.txt" << endl;
		exit(-1);
	}
	input >> nQueen >> obstacleX >> obstacleY;
	input.close();

	time_t start, end;
	start = clock();

	

	QS();

	end = clock();
	cout << (double)(end - start) / CLOCKS_PER_SEC;


	/*int a[] = { 2,0,3,1 };
	int b[] = { 2,2 };
	putRes(a,b,b,4);
	output.close();*/
	getchar();
	return 0;
}

