#include<iostream>
using namespace std;
template<int ObjectSize,int NumOfObjects=20>
class MemPool
{
private:
	const int MemBlockSize;//内存块大小
	const int MemNodeSize;//内存节点大小
	//空闲节点结构体
	struct FreeNode
	{
		struct FreeNode* _next;
		char _data[ObjectSize];
	};
	//内存块结构体
	struct MemBlock
	{
		struct MemBlock* _next;
		FreeNode _data[NumOfObjects];
	};
	
	FreeNode* FreeNodeHeader;//指向空节点链表头指针
	MemBlock* MemBlockHeader;//指向内存块链表头指针
public:
	MemPool()
		:MemNodeSize(ObjectSize + sizeof(FreeNode*))
		//内存节点大小初始化 指向下个节点的指针加对象的大小
		, MemBlockSize(sizeof(MemBlock*)+NumOfObjects*(ObjectSize + sizeof(FreeNode*)))
		//内存块大小为所有内存节点大小加上指向内存块指针的大小
		, FreeNodeHeader(NULL)
		, MemBlockHeader(NULL)
	{}
	~MemPool()
	{
		MemBlock* next=NULL;
		while (MemBlockHeader)
		{
			next = MemBlockHeader->_next;//保存下个节点位置
			delete MemBlockHeader;
			MemBlockHeader = next;
		}
	}
	void* malloc();
	void free(void*);
};
//分配空闲节点
template<int ObjectSize,int NumOfObjects>
void*  MemPool<ObjectSize, NumOfObjects>::malloc()
{
	if (FreeNodeHeader == NULL)//无空闲节点
	{
		MemBlock* NewBlock = new MemBlock;//开一整块内存
		NewBlock->_data[0]._next = NULL;//设置内存块中第一个节点为空闲节点链表的最后一个
		for (int i = 1; i < NumOfObjects; ++i)
		{
			NewBlock->_data[i]._next = &NewBlock->_data[i - 1];//把这些节点链在一起
		}
		FreeNodeHeader = &NewBlock->_data[NumOfObjects - 1];//
		NewBlock->_next = MemBlockHeader;
	}
	//返回空闲节点链表的 第一个节点
	void* FreeNode = FreeNodeHeader;
	FreeNodeHeader = FreeNodeHeader->_next;
	return FreeNode;
}
//释放已分配节点
template<int ObjectSize, int NumOfObjects>
void MemPool<ObjectSize, NumOfObjects>::free(void* ptr)
{
	FreeNode* pNode = (FreeNode*)ptr;
	pNode->_next = FreeNodeHeader;//将释放的节点插入到空闲节点链表头
	FreeNodeHeader = pNode;//更新链表头指针
}