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
	int dCollisions;
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
					if (queen.d1[i + queen.q[i]] > 1 || queen.d2[i - queen.q[i] + (nQueen - 1)] > 1
						|| queen.d1[j + queen.q[j]] > 1 || queen.d2[j - queen.q[j] + (nQueen - 1)] > 1)
					{
						dCollisions = -(queen.d1[i + queen.q[i]] - 1 + queen.d2[i - queen.q[i] + (nQueen - 1)] - 1)
							+ (queen.d1[i + queen.q[j]] + queen.d2[i - queen.q[j] + (nQueen - 1)])
							- (queen.d1[j + queen.q[j]] - 1 + queen.d2[j - queen.q[j] + (nQueen - 1)] - 1)
							+ (queen.d1[j + queen.q[i]] + queen.d2[j - queen.q[i] + (nQueen - 1)]);
						if (dCollisions < 0)
						{
							queen.d1[i + queen.q[i]]--;
							queen.d1[i + queen.q[j]]++;
							queen.d2[i - queen.q[i] + (nQueen - 1)]--;
							queen.d2[i - queen.q[j] + (nQueen - 1)]--;
							queen.d1[j + queen.q[j]]--;
							queen.d1[j + queen.q[i]]++;
							queen.d2[j - queen.q[j] + (nQueen - 1)]--;
							queen.d2[j - queen.q[i] + (nQueen - 1)]--;
							Swap(&queen.q[i], &queen.q[j]);
							queen.h += dCollisions;
							swapPerformed++;
						}
					}
				}
			}

		} while (swapPerformed != 0);

	} while (queen.h != 0);
	for (int i = 0; i < nQueen; i++)
	{
		cout << queen.q[i] << endl;
	}
}

int minSuccessor(const queenNode &current, queenNode &neighboor)
{
	int oldCol;
	int oldH;
	int tmpH;
	vector<queenNode> minNeighboor;

	neighboor = current;
	minNeighboor.push_back(neighboor);

	for (int i = 0; i < nQueen; i++)
	{
		oldCol = current.q[i];
		for (int j = 0; j < nQueen; j++)
		{
			if (oldCol != j )
			{
				neighboor.q[i] = j;
				tmpH = current.h;
				//计算h
				tmpH = tmpH - (current.col[oldCol] - 1) + (current.col[j])
					- (current.d1[i + oldCol] - 1) + current.d1[i + j]
					- (current.d2[i - oldCol + (nQueen - 1)] - 1) + current.d2[i - j + (nQueen - 1)];

				if (tmpH <= minNeighboor.back().h)
				{
					//更新记录的情况
					neighboor.h = tmpH;
					neighboor.col[oldCol]--;
					neighboor.col[j]++;
					neighboor.d1[i + oldCol]--;
					neighboor.d1[i + j]++;
					neighboor.d2[i - oldCol + (nQueen - 1)]--;
					neighboor.d2[i - j + (nQueen - 1)]++;
					minNeighboor.push_back(neighboor);
				}
				else
				{
					neighboor.q[i] = oldCol;
					continue;
				}

			}
			else
			{
				continue;
			}
			//重置
			neighboor.q[i] = oldCol;
			neighboor.h = current.h;
			neighboor.col[oldCol]++;
			neighboor.col[j]--;
			neighboor.d1[i + oldCol]++;
			neighboor.d1[i + j]--;
			neighboor.d2[i - oldCol + (nQueen - 1)]++;
			neighboor.d2[i - j + (nQueen - 1)]--;
		}
	}

	if (minNeighboor.size() > 1)
	{
		int minCount = 1;
		for (int i = minNeighboor.size() - 1; i - 1 >= 0 && minNeighboor.at(i).h == minNeighboor.at(i - 1).h; i--)
		{
			minCount++;
		}
		neighboor = minNeighboor.at(minNeighboor.size() - 1 - rand() % minCount);
	}
	else
	{
		neighboor = minNeighboor.at(0);
	}

	return neighboor.h;
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

	/*current.col.resize(nQueen, 0);
	current.d1.resize(2 * nQueen - 1, 0);
	current.d2.resize(2 * nQueen - 1, 0);
	int count = 0;
	while (true)
	{

		current.h = 0;
		current.d1.assign(current.d1.size(), 0);
		current.d2.assign(current.d2.size(), 0);
		current.col.assign(current.col.size(), 0);
		setqueen(nQueen, current, obstacleX, obstacleY);
		count++;
		while (true)
		{
			minSuccessor(current, neighboor);

			if (neighboor.h == 0)
			{
				//输出解
				cout << "Solved!" << endl;
				for (int i = 0; i < nQueen; i++)
				{

					cout << neighboor.q[i] << endl;
				}
				cout << "迭代次数" << count << endl;
				goto FOUND;
			}

			if (neighboor.h < current.h)
			{
				//后继中最优的
				current = neighboor;
			}
			else
			{
				//没有找到解，重新随机开始
				break;
			}
		}



	}*/

	QS();

FOUND:
	end = clock();
	cout << (double)(end - start) / CLOCKS_PER_SEC;


	/*int a[] = { 2,0,3,1 };
	int b[] = { 2,2 };
	putRes(a,b,b,4);
	output.close();*/
	return 0;
}

