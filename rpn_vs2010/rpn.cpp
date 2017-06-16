// rpn.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include <string.h>
#include<math.h>
#define max 100
char ex[max];        /*存储后序表达式*/
#define u8 char 
#define ENTER 0x0a

static u8 isOper(u8 u8Input)
{
    u8 u8Ret = 0;
    if ((u8Input == '!')||(u8Input == '|')||(u8Input == '&')||
        (u8Input == '=')||(u8Input == '>')||(u8Input == '<'))
    {
        u8Ret = 1;
    }
    return u8Ret;
}
static u8 isNumber(u8 u8Input)
{
    u8 u8Ret = 0;
    if (u8Input >= '0' && (u8Input <= '9'))
    {
        u8Ret = 1;
    }
    return u8Ret;
}
static u8 getprio(u8 u8Str)
{
    u8 u8Ret = 0;

    switch(u8Str)
    {
        case '|':
            u8Ret = 1;
        break;
        case '&':
            u8Ret = 2;
        break;
        case '=':
        case '$':
            u8Ret = 3;
        break;
        case '>':
        case '<':
        case '@':
        case '?':
            u8Ret = 4;
        break;
        case '!':
            u8Ret = 5;
        break;
        
        default:
        break;
    }
    return u8Ret;
}
static u8 OperConvert(u8 *pu8Str, u8 *pu8Conv)
{
    u8 u8Ret = 0;

    switch(*pu8Str)
    {
        case '|':
        case '&':
        case '=':  
            *pu8Conv = *pu8Str;
            u8Ret = 1;
        break;
        
        case '!':
            if (*(pu8Str + 1) == '=')
            {
                *pu8Conv = '$';/* != */
                u8Ret = 2;
            }
            else
            {
                *pu8Conv = *pu8Str;
                u8Ret = 1;
            }
        break;

        case '>':
            if (*(pu8Str + 1) == '=')
            {
                *pu8Conv = '?';/* >= */
                u8Ret = 2;
            }
            else
            {
                *pu8Conv = *pu8Str;
                u8Ret = 1;
            }
        break;
        case '<':
            if (*(pu8Str + 1) == '=')
            {
                *pu8Conv = '@';/* <= */
                u8Ret = 2;
            }
            else
            {
                *pu8Conv = *pu8Str;
                u8Ret = 1;
            }
        break;
        default:
        break;
    }
    return u8Ret;
}

void trans(char *pucStr,int iNum) {
	char str[max];   /*表达式字符串*/
	char stack[max]; /*运算符栈*/
	int sum, stridx, j, t, top = 0; /*sum用来统计字符串的长度*/
							   /*t是数字栈栈顶指针*/
							   /*top是数字栈栈顶指针*/
    int iRPNindex = 1;
    int iRet = 0;
    char cConv = 0;

    
	t = 1;
	stridx = 1;

	

	while (pucStr[stridx] != ENTER) {
        if (pucStr[stridx] == '(')
        {
            top++;
            stack[top] = pucStr[stridx];
            stridx++;
        }
        else if (pucStr[stridx] == ')')
        {
            while(stack[top] != '(')
            {
                ex[iRPNindex] = stack[top];
                top--;
                iRPNindex++;
            }
            top--;/* 左括号出栈 */
            stridx++;
        }
        else if (isOper(pucStr[stridx]))
        {
           iRet = OperConvert(&pucStr[stridx],&cConv);
           while((top != 0) &&(getprio(cConv) <= getprio(stack[top])))
           {
                ex[iRPNindex] = stack[top];
                top--;
                iRPNindex++;
           }

           stridx += iRet;
		   top++;
           stack[top] = cConv;
           
        }
        else if (isNumber(pucStr[stridx]))
        {     
            while(isNumber(pucStr[stridx]))
            {
                ex[iRPNindex] = pucStr[stridx];
                iRPNindex++;
                stridx++;
            }
            
            ex[iRPNindex] = '#';
            iRPNindex++;
        }
        else
        {
            stridx++;
        }

	}
	/*把符号栈剩下的元素压进去*/
	while (top != 0) {
        ex[iRPNindex] = stack[top];       
		iRPNindex++;
		top--;
	}
	ex[iRPNindex] = 'E';
	printf("\n原来的中序表达式：");
	for (j = 1; j < iNum; j++)
		printf("%c", pucStr[j]);
	printf("\n后缀表达式：");
	for (j = 1; j < iRPNindex; j++)
		printf("%c", ex[j]);
}
void compvalue() { /*计算后缀表达式的值*/
	float stack[max], d; /*作为栈使用*/
	char ch;
	int t = 1, top = 0; /*t为ex下标，top为stack下标*/
    int iRPNindex = 1;
	ch = ex[iRPNindex];
	
	//从左到右检查字符串
	while (ex[iRPNindex] != 'E') {
		switch (ex[iRPNindex]) {
		case '|':
			stack[top - 1] = (stack[top - 1] || stack[top]);
			top--;
			break;
            
		case '&':
			stack[top - 1] = (stack[top - 1] && stack[top]);
			top--;
			break;
            
		case '!':
			stack[top] = !stack[top];
         	break;
            
		case '=':
            stack[top - 1] = (stack[top - 1] == stack[top]);
			top--;
			break;
            
        case '$':
            stack[top - 1] = (stack[top - 1] != stack[top]);
            top--;
            break;
            
        case '>':
            stack[top - 1] = (stack[top - 1] > stack[top]);
            top--;
            break;
            
        case '?':/* >= */
            stack[top - 1] = (stack[top - 1] >= stack[top]);
            top--;
            break;
            
        case '<':
            stack[top - 1] = (stack[top - 1] < stack[top]);
            top--;
            break;
            
        case '@':/* <= */
            stack[top - 1] = (stack[top - 1] <= stack[top]);
            top--;
            break;
                         
		default:
			d = 0;
			while (ex[iRPNindex] >= '0' && ex[iRPNindex] <= '9') {
				d = 10 * d + ex[iRPNindex] - '0'; /*将数字字符转化为对应的数值*/
				iRPNindex++;
			}
			top++;
			stack[top] = d;
		}
		
		iRPNindex++;
	}
	printf("\n\t计算结果:%g\n", stack[top]);
}

static int  InputVal(char *pcStr)
{
    int stridx;
	printf("*****************************************\n");
	printf("*输入一个求值的表达式，以回车结束。*\n");
	printf("******************************************\n");
	printf("逻辑表达式：");

	stridx = 0;
	/*这个循环语句，获取一个用户输入的合法的表达式*/
	do {
		stridx++;
		scanf("%c", &pcStr[stridx]);
	} while (pcStr[stridx] != ENTER && stridx != max);
    return stridx;
}



int main(int argc, char* argv[])
{
    char str[max];   /*表达式字符串*/
    int iNum = 0;
	while(1)
	{
		memset(str,0,sizeof(str));
		iNum = InputVal(str);
		trans(str,iNum);      //转化成逆波兰式
		compvalue();  //计算值
	}
	return 0;
}

