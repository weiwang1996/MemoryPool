#include<iostream>
using namespace std;
template<int ObjectSize,int NumOfObjects=20>
class MemPool
{
private:
	const int MemBlockSize;//�ڴ���С
	const int MemNodeSize;//�ڴ�ڵ��С
	//���нڵ�ṹ��
	struct FreeNode
	{
		struct FreeNode* _next;
		char _data[ObjectSize];
	};
	//�ڴ��ṹ��
	struct MemBlock
	{
		struct MemBlock* _next;
		FreeNode _data[NumOfObjects];
	};
	
	FreeNode* FreeNodeHeader;//ָ��սڵ�����ͷָ��
	MemBlock* MemBlockHeader;//ָ���ڴ������ͷָ��
public:
	MemPool()
		:MemNodeSize(ObjectSize + sizeof(FreeNode*))
		//�ڴ�ڵ��С��ʼ�� ָ���¸��ڵ��ָ��Ӷ���Ĵ�С
		, MemBlockSize(sizeof(MemBlock*)+NumOfObjects*(ObjectSize + sizeof(FreeNode*)))
		//�ڴ���СΪ�����ڴ�ڵ��С����ָ���ڴ��ָ��Ĵ�С
		, FreeNodeHeader(NULL)
		, MemBlockHeader(NULL)
	{}
	~MemPool()
	{
		MemBlock* next=NULL;
		while (MemBlockHeader)
		{
			next = MemBlockHeader->_next;//�����¸��ڵ�λ��
			delete MemBlockHeader;
			MemBlockHeader = next;
		}
	}
	void* malloc();
	void free(void*);
};
//������нڵ�
template<int ObjectSize,int NumOfObjects>
void*  MemPool<ObjectSize, NumOfObjects>::malloc()
{
	if (FreeNodeHeader == NULL)//�޿��нڵ�
	{
		MemBlock* NewBlock = new MemBlock;//��һ�����ڴ�
		NewBlock->_data[0]._next = NULL;//�����ڴ���е�һ���ڵ�Ϊ���нڵ���������һ��
		for (int i = 1; i < NumOfObjects; ++i)
		{
			NewBlock->_data[i]._next = &NewBlock->_data[i - 1];//����Щ�ڵ�����һ��
		}
		FreeNodeHeader = &NewBlock->_data[NumOfObjects - 1];//
		NewBlock->_next = MemBlockHeader;
	}
	//���ؿ��нڵ������ ��һ���ڵ�
	void* FreeNode = FreeNodeHeader;
	FreeNodeHeader = FreeNodeHeader->_next;
	return FreeNode;
}
//�ͷ��ѷ���ڵ�
template<int ObjectSize, int NumOfObjects>
void MemPool<ObjectSize, NumOfObjects>::free(void* ptr)
{
	FreeNode* pNode = (FreeNode*)ptr;
	pNode->_next = FreeNodeHeader;//���ͷŵĽڵ���뵽���нڵ�����ͷ
	FreeNodeHeader = pNode;//��������ͷָ��
}