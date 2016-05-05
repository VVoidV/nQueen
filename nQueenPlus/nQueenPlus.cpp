// nQueen.cpp : 定义控制台应用程序的入口点。
//

#include<iostream>
#include <queue>
#include<fstream>
#include<vector>
#include<time.h>
#include<random>
#include <array>
#define C1 0.45
using namespace std;


int obstacleX;
int obstacleY;
int nQueen;
int c;
int C2;
random_device rd;   // non-deterministic generator
mt19937 gen(rd());  // to seed mersenne twister.
uniform_int_distribution<> dist(0, 0x7fffffff); // distribute results between 0 and 0x7fffffff inclusive.

struct queenNode {
	int h;
	vector<int> q;
	vector<int> d1;//次对角线
	vector<int> d2;//主对角线
	vector<int> attack;
}queen;

inline void Swap(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

void setqueen(const int n, queenNode &queen)
{
	
	int i;
	//(i,queen.q[i])
	queen.q.resize(n);

	for ( i = 0; i < nQueen; i++)
	{
		queen.q[i] = i;
	}

	for (i = 0; i < nQueen - c; i++)
	{
		do 
		{
			Swap(&queen.q[i], &queen.q[dist(gen) % (nQueen-i)+i]);
		} while (queen.d1[i + queen.q[i]]!=0||queen.d2[i - queen.q[i] + (n - 1)]!=0);
		queen.d2[i - queen.q[i] + (n - 1)]++;//主对角线			
		queen.d1[i + queen.q[i]]++;			//次对角线	
	}

	while (i<nQueen)
	{
		Swap(&queen.q[i], &queen.q[rand() % (n - i) + i]);
		queen.d2[i - queen.q[i] + (n - 1)]++;//主对角线			
		queen.d1[i + queen.q[i]]++;			//次对角线
		i++;
	}
// 	for (int i = 0; i<n; i++)
// 	{
// 		queen.d2[i - queen.q[i] + (n - 1)]++;//主对角线			
// 		queen.d1[i + queen.q[i]]++;			//次对角线	
// 	}
	//检查对角线上冲突数
	for (i = 0; i < 2 * n - 1; i++)
	{
		queen.h += (queen.d1[i] * (queen.d1[i] - 1)) / 2;
		queen.h += (queen.d2[i] * (queen.d2[i] - 1)) / 2;
	}
}

int compute_attacks(queenNode &queen)
{
	int i,attackCounter;
	for (i = 0, attackCounter=0; i < nQueen; i++)
	{
		if (queen.d2[i - queen.q[i] + (nQueen - 1)]>1||queen.d1[i + queen.q[i]]>1)
		{
			queen.attack[attackCounter++] = i;
		}
	}
	return attackCounter;
}
void QS(void)
{
	int restart = 0;
	int oldH;
	int swapPerformed;
	int loopCount;
	int limit;
	int attackCounter;
	int attackedQueen;
	int randomQueen;
	queen.d1.resize(2 * nQueen - 1);
	queen.d2.resize(2 * nQueen - 1);
	queen.attack.resize(nQueen);
REDO:	do 
	{
		queen.h = 0;
		queen.d1.assign(queen.d1.size(), 0);
		queen.d2.assign(queen.d2.size(), 0);
		queen.attack.assign(queen.attack.size(), 0);
		setqueen(nQueen, queen);
		attackCounter = compute_attacks(queen);
		loopCount = 0;
		limit = C1*queen.h;
		do
		{

			swapPerformed = 0;
			
			for (int i = 0; i < attackCounter; i++)
			{
				oldH = queen.h;
				attackedQueen = queen.attack[i];
				//for (int j = 0; j < nQueen; j++)
				{
					randomQueen = dist(gen) % nQueen;
					//拿掉i行和j行的皇后
					queen.h = queen.h - (queen.d1[attackedQueen + queen.q[attackedQueen]] - 1) - (queen.d1[randomQueen + queen.q[randomQueen]] - 1)
						- (queen.d2[attackedQueen - queen.q[attackedQueen] + nQueen - 1] - 1) - (queen.d2[randomQueen - queen.q[randomQueen] + nQueen - 1]);
					//尝试交换
					queen.d1[attackedQueen + queen.q[attackedQueen]]--;
					queen.d1[attackedQueen + queen.q[randomQueen]]++;
					queen.d2[attackedQueen - queen.q[attackedQueen] + (nQueen - 1)]--;
					queen.d2[attackedQueen - queen.q[randomQueen] + (nQueen - 1)]++;
					queen.d1[randomQueen + queen.q[randomQueen]]--;
					queen.d1[randomQueen + queen.q[attackedQueen]]++;
					queen.d2[randomQueen - queen.q[randomQueen] + (nQueen - 1)]--;
					queen.d2[randomQueen - queen.q[attackedQueen] + (nQueen - 1)]++;
					Swap(&queen.q[attackedQueen], &queen.q[randomQueen]);
					//计算新的冲突数
					queen.h = queen.h + (queen.d1[attackedQueen + queen.q[attackedQueen]] - 1) + (queen.d1[randomQueen + queen.q[randomQueen]] - 1)
						+ (queen.d2[attackedQueen - queen.q[attackedQueen] + nQueen - 1] - 1) + (queen.d2[randomQueen - queen.q[randomQueen] + nQueen - 1]);


					if (oldH <= queen.h)
					{
						//冲突未减小，放回去
						queen.d1[attackedQueen + queen.q[attackedQueen]]--;
						queen.d1[attackedQueen + queen.q[randomQueen]]++;
						queen.d2[attackedQueen - queen.q[attackedQueen] + (nQueen - 1)]--;
						queen.d2[attackedQueen - queen.q[randomQueen] + (nQueen - 1)]++;
						queen.d1[randomQueen + queen.q[randomQueen]]--;
						queen.d1[randomQueen + queen.q[attackedQueen]]++;
						queen.d2[randomQueen - queen.q[randomQueen] + (nQueen - 1)]--;
						queen.d2[randomQueen - queen.q[attackedQueen] + (nQueen - 1)]++;
						Swap(&queen.q[attackedQueen], &queen.q[randomQueen]);
						queen.h = oldH;
					}
					else
					{
						if (queen.h == 0)
						{
							break;
						}
						if (queen.h < limit)
						{
							limit = C1*limit;
							attackCounter = compute_attacks(queen);
						}
					}
				}					
			}
			loopCount += attackCounter;
// 				randomQueen = dist(gen) % nQueen;
// 				
// 				if (queen.d1[attackedQueen + queen.q[attackedQueen]] > 1)
// 				{
// 					dH--;
// 				}
// 				if (queen.d1[randomQueen + queen.q[randomQueen]] > 1)
// 				{
// 					dH--;
// 				}
// 				if (queen.d2[attackedQueen - queen.q[attackedQueen]+nQueen-1] > 1)
// 				{
// 					dH--;
// 				}
// 				if (queen.d2[randomQueen - queen.q[randomQueen] + nQueen - 1] > 1)
// 				{
// 					dH--;
// 				}
// 
// 				if (queen.d1[attackedQueen + queen.q[randomQueen]] > 0)
// 				{
// 					dH++;
// 				}
// 				if (queen.d1[randomQueen + queen.q[attackedQueen]] > 0)
// 				{
// 					dH++;
// 				}
// 				if (queen.d2[attackedQueen - queen.q[randomQueen] + nQueen - 1] > 0)
// 				{
// 					dH++;
// 				}
// 				if (queen.d2[randomQueen - queen.q[attackedQueen] + nQueen - 1] > 0)
// 				{
// 					dH++;
// 				}
// 				
// 				if (dH < 0)
// 				{
// 					queen.d1[attackedQueen + queen.q[attackedQueen]]--;
// 					queen.d1[attackedQueen + queen.q[randomQueen]]++;
// 					queen.d2[attackedQueen - queen.q[attackedQueen] + (nQueen - 1)]--;
// 					queen.d2[attackedQueen - queen.q[randomQueen] + (nQueen - 1)]++;
// 					queen.d1[randomQueen + queen.q[randomQueen]]--;
// 					queen.d1[randomQueen + queen.q[attackedQueen]]++;
// 					queen.d2[randomQueen - queen.q[randomQueen] + (nQueen - 1)]--;
// 					queen.d2[randomQueen - queen.q[attackedQueen] + (nQueen - 1)]++;
// 					Swap(&queen.q[attackedQueen], &queen.q[randomQueen]);
// 					queen.h += dH;
// 					swapPerformed++;
// 				}				 
// 			}
// 			attackCounter = compute_attacks(queen);
		}while (loopCount<C2*nQueen);
		restart++;
	} while (queen.h != 0);

	if (queen.q[obstacleX]==obstacleY)
	{
		cout << "sorry，unfortunate things happend TAT" << endl<<"Regenerating!"<<endl;
		goto REDO;
	}

	
	/*for (int i = 0; i < nQueen; i++)
	{
		
	}*/
	cout << restart << endl;
	
}



int main()
{
	ifstream input("input.txt");
	ofstream output("QS_RESULT.txt");
	queenNode current, neighboor;

	
	if (!input)
	{
		cout << "can not open input.txt" << endl;
		exit(-1);
	}
	input >> nQueen >> obstacleX >> obstacleY;
	input.close();

	if (nQueen<3)
	{
		cout << "读书少别骗我，没解的啊！" << endl;
		output << "NULL" << endl;
		output.close();
		return 0;
	}
	if (nQueen == 3)
	{
		if (obstacleX + obstacleY == 1 || obstacleY + obstacleX == 3)
		{
			switch (obstacleX)
			{
			case 0:
				output << 0 << ' ' << 2 << endl <<-1<<endl<< 1 << endl << 0 ;
				break;
			case 1:
				if (obstacleY==0)	
				{
					output << 0 << endl << 2 << endl << 0 << endl << 0;
				}
				else
				{
					output << 2 << endl << 0 << endl << 2 << endl << 0;
				}
				break;
			case 2:
				output << 1 << endl << -1 << endl << 0<<' '<< 2<< endl << 0;
				break;
			default:
				break;
			}
			output.close();
			return 0;

		}
		else
		{
			cout << "读书少别骗我，没解的啊！" << endl;
			output << "NULL" << endl;
			output.close();
			return 0;
		}
	}

	if (nQueen >= 5e5)
	{
		C2 = 1;
		c = 100;
	}
	else if (nQueen>=5e4)
	{
		C2 = 1;
		c = 80;
	}
	else if (nQueen>5e3)
	{
		C2 = 1;
		c = 50;
	}
	else if (nQueen>5e2)
	{
		C2 = 32;
		c = 50;
	}
	else if (nQueen > 10)
	{
		C2 = 32;
		c = 30;
	}
	else
	{
		C2 = 32;
		c = 8;
	}
	//c = 100;
	time_t start, end;
	start = clock();
	QS();
	end = clock();
	cout << (double)(end - start) / CLOCKS_PER_SEC;
	
	
	for (int i=0;i<nQueen;i++)
	{
		output << queen.q[i] << endl;
	}
	output<< (double)(end - start) / CLOCKS_PER_SEC;
	
	output.close();
	return 0;
}

