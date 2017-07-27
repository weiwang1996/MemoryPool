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
		count++;//����������
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
//�����ڴ�ض���
MemPool<sizeof(ActualClass), 2>  mp;//�����ڴ�ؿ��нڵ����
//���� new/delete�ڴ�صĺ���
void* ActualClass::operator new(size_t size)
{
	return mp.malloc();
}

void ActualClass::operator delete(void *ptr)
{
	mp.free(ptr);
}
//���Գ���
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