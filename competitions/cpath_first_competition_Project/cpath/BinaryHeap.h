#ifndef MINHEAP_H
#define MINHEAP_H

#include<iostream>
#include<vector>
#include<cmath>
#include "configure.h"
using namespace std;


template <typename Key,typename Data>
class BinaryHeapNode {
public:
	BinaryHeapNode(Key key,Data data){	this->myKey = key;	this->myData = data;	pos = -1;}
	Key key() const { return myKey; }
	Data data() const { return myData; }
public:
	Key myKey;
	Data myData;
	int pos;					//����ȫ�������е�λ��
};



template <typename Key,typename Data>
class MinBinaryHeap{
public:
	MinBinaryHeap(){size = 0;	heap.resize(MAX_NODE_NUM*2 + 10);}			//0��Ԫ����ȫ����������	
	bool empty(){ return size>0?false:true;};												//�ж϶��Ƿ�Ϊ��
	BinaryHeapNode<Key,Data>* insert(Key key,Data data);						//����Ԫ��
	BinaryHeapNode<Key,Data>* minimum(){	return heap[1];};				//�����СԪ������
	void removeMinimum();																		//ɾ��������СԪ��
	BinaryHeapNode<Key,Data>* findItem(Key key);									//����key��heap�е�λ���±�
	void decreaseKey(BinaryHeapNode<Key,Data>* node, Key key);			//�޸�heap[index]����Ϊkey  �޸�����maintainHeap
	void clear();
	void print();
public:
	int size;   //size = heap.size() -1  0��Ԫ����ȫ����������
	vector<BinaryHeapNode<Key,Data>*> heap;
};
template <typename Key,typename Data>
BinaryHeapNode<Key,Data>* MinBinaryHeap<Key,Data>::insert(Key key,Data data){
	size++;
	BinaryHeapNode<Key,Data>* bhnp = new BinaryHeapNode<Key,Data>(key,data);
	bhnp->pos = size;
	heap[size] = bhnp;
	int pos = size;
	while(pos>1){
		if(key<heap[pos/2]->key()){
			heap[pos] = heap[pos/2];    heap[pos]->pos = pos;
			pos /= 2;
		}
		else break;
	}
	heap[pos] = bhnp;	heap[pos]->pos = pos;
	return bhnp;
}
template <typename Key,typename Data>
void MinBinaryHeap<Key,Data>::removeMinimum(){
	if(size > 0){
		delete heap[1]; heap[1] = NULL;
		BinaryHeapNode<Key,Data>* k = heap[size--];
		int pos = 1,child = 2;  //����Ů
		while(child <= size){
			if(child < size){//������Ů
				if(heap[child]->myKey > heap[child+1]->myKey) child++;    //childΪ��С����Ů
			}
			if(k->myKey <= heap[child]->myKey) break;     //find k's place
			else{		//��С��Ů������
				heap[pos] = heap[child];    heap[pos]->pos = pos;
				pos = child;    child *= 2;
			}
		}
		heap[pos] = k;	heap[pos]->pos = pos;
	}
}
template <typename Key,typename Data>
void MinBinaryHeap<Key,Data>::decreaseKey(BinaryHeapNode<Key,Data>* node, Key key){
	if (node == NULL)	return;
	node->myKey = key;
	//�²������ݱ�ԭ��������С�����ϵ���
	int index = node->pos;
	BinaryHeapNode<Key,Data>* tmp;
	while(index>1){
		if(heap[index]->myKey < heap[index/2]->myKey){    //�Լ��ȸ���С����Ҫ����
			tmp = heap[index];  
			heap[index] = heap[index/2]; heap[index]->pos = index;
			heap[index/2] = tmp ;	heap[index/2]->pos = index/2;
			index /= 2;
		}
		else break;
	}
}
template <typename Key,typename Data>
BinaryHeapNode<Key,Data>* MinBinaryHeap<Key,Data>::findItem(Key key){
	for(int i=1;i<=size;i++){ if(key == heap[i]->myKey)  return heap[i];}
	return NULL;
}
template <typename Key,typename Data>
void MinBinaryHeap<Key,Data>::print(){
	for(int i=1;i<=size;i++){
		cout<<heap[i]->myKey<<"_"<<heap[i]->myData<<"_"<<heap[i]->pos<<" ";
	}
	cout<<endl;
}
template <typename Key,typename Data>
void MinBinaryHeap<Key,Data>::clear(){
	for(int i=1;i<=size;i++){		delete heap[i];	heap[i] = NULL;}
	size = 0;
}


// void test_MinHeap(){
// 	MinBinaryHeap<int,int> h;
// 	BinaryHeapNode<int,int>* node;
// 
// 	cout<<"insert: "<<endl;
// 	h.insert(9,9);
// 	h.print();
// 	h.insert(2,2);
// 	h.print();
// 	h.insert(3,3);
// 	h.print();
// 	h.insert(7,7);
// 	h.print();
// 	h.insert(5,5);
// 	h.print();
// 	h.insert(6,6);
// 	h.print();
// 	h.insert(9,9);
// 	h.print();
// 	h.insert(2,2);
// 	h.print();
// 	h.insert(3,3);
// 	h.print();
// 	h.insert(7,7);
// 	h.print();
// 	h.insert(5,5);
// 	h.print();
// 	h.insert(6,6);
// 	h.print();
// 
// 	cout<<"decreaseKey: "<<endl;
// 	node = h.findItem(7);
// 	h.decreaseKey(node,1);
// 	h.print();
// 
// 
// 
// 	cout<<"removeMinimum: "<<endl;
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// 	cout<<"Minimum: "<<h.minimum()->myKey<<endl;
// 	h.removeMinimum();
// 	h.print();
// }

#endif


