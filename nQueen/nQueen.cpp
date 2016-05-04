// nQueen.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>
#include <queue>
#include<fstream>
#include<vector>
#include<time.h>


using namespace std;


int obstacleX;
int obstacleY;
int nQueen;

struct queenNode {
	int h;
	vector<int> q;
	vector<int> col;
	vector<int> d1;//�ζԽ���
	vector<int> d2;//���Խ���
};

void setqueen(const int n,queenNode &queen,const int obstacleX,const int obstacleY )
{
	//(i,queen.q[i])
	queen.q.resize(n);
	
	for (int i=0;i<n;i++)
	{
		if (obstacleX==i)
		{
			do
			{
				queen.q[i] = rand() % n;
			} while (obstacleY == queen.q[i]);

			
		}
		else
		{
			queen.q[i] = rand() % n;
		}
		queen.col[queen.q[i]]++;//��			
		queen.d2[i - queen.q[i] + (n - 1) ]++;//���Խ���			
		queen.d1[i + queen.q[i] ]++;			//�ζԽ���	
	}
	//�����
	for (int i=0;i<n;i++)
	{
		queen.h+= (queen.col[i] * (queen.col[i] - 1)) / 2;
	}
	//���Խ���
	for (int i = 0; i < 2*n-1; i++)
	{
		queen.h += (queen.d1[i] * (queen.d1[i] - 1) )/ 2;
		queen.h += (queen.d2[i] * (queen.d2[i] - 1) )/ 2;
	}
}

int minSuccessor(const queenNode &current,queenNode &neighboor)
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
		for (int j = 0; j < nQueen ; j++)
		{
			if (oldCol != j && (i != obstacleX || j != obstacleY))
			{
				neighboor.q[i] = j;
				tmpH = current.h;
				//����h
				tmpH = tmpH - (current.col[oldCol] - 1) + (current.col[j])
					- (current.d1[i + oldCol] - 1) + current.d1[i + j]
					- (current.d2[i - oldCol + (nQueen - 1)] - 1) + current.d2[i - j + (nQueen - 1)];

				if (tmpH <= minNeighboor.back().h)
				{
					//���¼�¼�����
					neighboor.h = tmpH;
					neighboor.col[oldCol]--;
					neighboor.col[j]++;
					neighboor.d1[i + oldCol]--;
					neighboor.d1[i + j ]++;
					neighboor.d2[i - oldCol + (nQueen - 1) ]--;
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
			//����
			neighboor.q[i] = oldCol;
			neighboor.h = current.h;
			neighboor.col[oldCol]++;
			neighboor.col[j]--;
			neighboor.d1[i + oldCol ]++;
			neighboor.d1[i + j ]--;
			neighboor.d2[i - oldCol + (nQueen - 1) ]++;
			neighboor.d2[i - j + (nQueen - 1) ]--;
		}
		
	}
	
	if (minNeighboor.size() > 1)
	{	
		int minCount=1;
		for (int i = minNeighboor.size() - 1; i-1>=0&&minNeighboor.at(i).h == minNeighboor.at(i - 1).h; i--)
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
	queenNode current,neighboor;
	

	if (!input)
	{
		cout << "can not open input.txt" << endl;
		exit(-1);
	}
	input >> nQueen >> obstacleX >> obstacleY;
	input.close();
	
	time_t start, end;
	start = clock();
	
	current.col.resize(nQueen,0);
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
				//�����
				cout << "Solved!" << endl;
				for (int i = 0; i < nQueen; i++)
				{

					cout << neighboor.q[i] << endl;
				}
				cout << "��������" << count << endl;
				goto FOUND;
			}

			if (neighboor.h < current.h)
			{
				//��������ŵ�
				current = neighboor;
			}
			else
			{
				//û���ҵ��⣬���������ʼ
				break;
			}
		}
		


	}
	
	
	
FOUND:
	end = clock();
	cout << (double)(end-start)/CLOCKS_PER_SEC;
    return 0;
}

