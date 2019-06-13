// #include <stdafx.h>
// #include "stdafx.h"
#include <stdio.h>
#include <string.h>

//char name[65] = "abcdexxxx";
char name[65];
char table[8] = { 0xC ,0xA ,0x13 ,0x09 ,0x0C ,0x0B ,0x0A ,0x08 };
int main()
{
    printf(" 输入key：\n ");
    scanf("%s",name,65);

    //会用到一个固定地址的值
    //会用到姓名里的后两位
    //eax = i
    int user_len = strlen(name);
    int key_code= 0; //esi
    
    int count_ecx = 3;  //esi
    int eax = 0;      //eax
    for (; count_ecx<user_len;)
    {
        if (eax>7)
            eax = 0;

        int ebx = 0;
        int edx = 0;

        edx = name[count_ecx];
        ebx = table[eax];
        ebx = edx * ebx;

        key_code += ebx;
        count_ecx++;
        eax++;

    }
    printf("key_code: %d", key_code);
    return 0;
}
