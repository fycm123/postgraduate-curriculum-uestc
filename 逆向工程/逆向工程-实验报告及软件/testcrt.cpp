// testcrt.cpp : 
// #include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
int _tmain(int argc, _TCHAR* argv[])
{
   int x1,x2;
   printf("please input x1:\n");
   scanf("%d",&x1);
   printf("please input x2:\n");
   scanf("%d",&x2);
   if (x1 > x2)  printf("x1 > X2");
      else
         printf("x1<=x2");
   printf("\n please click key:");
   scanf("%d",&x1);
   return 0;
}

