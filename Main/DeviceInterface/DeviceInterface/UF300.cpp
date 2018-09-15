#include "StdAfx.h"
#include "deviceinterface.h"//不能放在头文件中，否则会出现deviceinterface重定义的错误
#include "UF300.h"


UF300::UF300(void)
{
}


UF300::~UF300(void)
{
}

int UF300::addtest(int a,int b)
{
	return (a-b);
}