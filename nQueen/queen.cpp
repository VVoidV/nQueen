#include "stdafx.h"
#include "queen.h"
using std::cout;
using std::endl;
void putRes(int queen[],int secdqueen[],int obstacle[],int queenCount)
{
	for (int i = 0; i < queenCount; i++)
	{
		for (int j = 0; j < queenCount; j++)
		{
			if (obstacle[0] == i&&obstacle[1] == j)
			{
				printf("X\t");
			}
			else
			{
				if (queen[i] >= 0)
				{
					//每行只有一个皇后
					if (queen[i] != j)
					{
						printf("*\t");
					}
					else
					{
						printf("Q\t");
					}

				}

				else if (queen[i] == -1)
				{//没皇后
					printf("*\t");
				}
				else
				{//每行有两个皇后

					if (secdqueen[0] != j || secdqueen[1] != j)
					{
						printf( "*\t");
					}
					else
					{
						printf( "Q\t");
					}

				}

			}
		}
		printf("\n\n");
		
	}
}
