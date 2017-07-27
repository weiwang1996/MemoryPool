#include"mempool.h"
#include<iostream>
using namespace std;
class  ActualClass
{
private:
	static int count;
	int num;
public:
	ActualClass()
	{
		num = count;
		count++;//计算对象多少
	}
	void Print()
	{
		cout << this << ":";
		cout << "the " << num << "object"<<endl;
	}
	void * operator new(size_t size);
	void operator delete(void* ptr);
};
int ActualClass::count = 0;
//定义内存池对象
MemPool<sizeof(ActualClass), 2>  mp;//给定内存池空闲节点个数
//重载 new/delete内存池的函数
void* ActualClass::operator new(size_t size)
{
	return mp.malloc();
}

void ActualClass::operator delete(void *ptr)
{
	mp.free(ptr);
}
//测试程序
void TestMemPool()
{
ActualClass* p1 = new ActualClass;
	p1->Print();
	ActualClass* p2 = new ActualClass;
	p2->Print();
	delete p1;
	p1 = new ActualClass;
	p1->Print();
	ActualClass* p3 = new ActualClass;
	p3->Print();
	delete p1;
	delete p2;
	delete p3;
}
int main()
{
	TestMemPool();
	return 0;
}