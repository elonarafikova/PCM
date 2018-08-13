
#include "stdafx.h"
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
 

using namespace std;

class Bucket {
	int depth,size,overflow;
	map<int, int> values;
public:
	Bucket(int depth, int size,int overflow);
	int insert(int key,int value);
	int remove(int key);
	void search(int key);
	int isFull(void);
	int getDepth(void);
	int increaseDepth(void);
	map<int, int> copy(void);
	void clear(void);
};
class Directory {
	int global_depth, bucket_size, overflow_factor;
	vector<Bucket*> buckets;
	int hash(int n);
	int pairIndex(int bucket_no, int depth);
	void grow(void);
	void split(int bucket_no);
public:
	Directory(int depth, int bucket_size,int overflow_factor);
	void insert(int key,int value,bool reinserted);
	void remove(int key);
	void search(int key);
};


void menu()
{   cout<<"----------------------------------------------------"<<endl;
	cout<<"Enter queries in the following format :"<<endl;
    cout<<"insert <key> <value>     (key: integer, value: integer)"<<endl;
    cout<<"delete <key> "<<endl;
    cout<<"search <key>"<<endl;
    cout<<"exit"<<endl;
	cout<<"----------------------------------------------------"<<endl;
};


int main()
{
	int bucket_size, initial_global_depth, overflow_factor;
    int key, value;
    string choice;

	cout<<"Enter parameters to initialize directory structure";
	cout<<endl;
	cout<<"----------------------------------------------------"<<endl;
    cout<<"Bucket size : ";
    cin>>bucket_size;
    cout<<"Initial global depth : ";
    cin>>initial_global_depth;
    cout<<"Overflow factor : "; 
    cin>>overflow_factor;
	cout<<"----------------------------------------------------"<<endl;
    Directory directory(initial_global_depth,bucket_size,overflow_factor);
    cout<<endl<<"Initialized directory structure"<<endl;
	
    menu();
    do
	{
		
	cin>>choice;
		if(choice=="insert")
		{
			cin>>key>>value;
            cout<<endl;
			directory.insert(key,value,0);
		}
        else
			if(choice=="delete")
			{
				cin>>key;
				cout<<endl;
				directory.remove(key);
			}
			
			else
				if(choice=="search")
				{
					cin>>key;
					cout<<endl;
					directory.search(key);
				}
	}
	while(choice!="exit");
		return 0;
}




Directory::Directory(int depth, int bucket_size,int overflow_factor)
{
	this->global_depth = depth;
    this->bucket_size = bucket_size;
	this->overflow_factor=overflow_factor;
    for(int i = 0 ; i < 1<<depth ; i++ )
    {
        buckets.push_back(new Bucket(depth,bucket_size,overflow_factor));
    }
}

int Directory::hash(int n)
{
    return n&((1<<global_depth)-1);
}

int Directory::pairIndex(int bucket_no, int depth)
{
    return bucket_no^(1<<(depth-1));
}

void Directory::grow(void)
{
    for(int i = 0 ; i < 1<<global_depth ; i++ )
        buckets.push_back(buckets[i]);
    global_depth++;
}

void Directory::split(int bucket_no)
{
    int local_depth,pair_index,index_diff,dir_size,i;
    map<int, int> temp;
    map<int, int>::iterator it;
    local_depth = buckets[bucket_no]->increaseDepth();
    if(local_depth>global_depth)
        grow();
    pair_index = pairIndex(bucket_no,local_depth);
    buckets[pair_index] = new Bucket(local_depth,bucket_size,overflow_factor);
    temp = buckets[bucket_no]->copy();
    buckets[bucket_no]->clear();
    index_diff = 1<<local_depth;
    dir_size = 1<<global_depth;

    for( i=pair_index-index_diff ; i>=0 ; i-=index_diff )
	{
        buckets[i] = buckets[pair_index];
		
	}
    for( i=pair_index+index_diff ; i<dir_size ; i+=index_diff )
	{
        buckets[i] = buckets[pair_index];
		
	}
    for(it=temp.begin();it!=temp.end();it++)
	{   
        insert((*it).first,(*it).second,1);
	}
}
     

void Directory::insert(int key,int value,bool reinserted)
{
    int bucket_no = hash(key);
    int status = buckets[bucket_no]->insert(key,value);
    if(status==1)
    {
        if(!reinserted)
            cout<<"Inserted key "<<key<<" value "<<value<<endl;
        else
            cout<<"Moved key "<<key<<" value "<<value<<endl;
    }
    else if(status==0)
    {
        split(bucket_no);
        insert(key,value,reinserted);
    }
    else
    {
        cout<<"Key "<<key<<" already exists"<<endl;

    }
}

void Directory::remove(int key)
{
    int bucket_no = hash(key);
    if(buckets[bucket_no]->remove(key))
        cout<<"Deleted key "<<key<<endl;
}

void Directory::search(int key)
{
    int bucket_no = hash(key);
    cout<<"Searching key "<<key<<endl;
    buckets[bucket_no]->search(key);
}
    
;
Bucket::Bucket(int depth, int size, int overflow)
{
    this->depth = depth;
    this->size = size;
	this->overflow=overflow;
}

int Bucket::insert(int key, int value)
{
    map<int,int>::iterator it;

    it = values.find(key);
    if(it!=values.end())
        return -1;
    if(isFull())
        return 0;
    values[key] = value;
    return 1;
}

int Bucket::remove(int key)
{
    map<int,int>::iterator it;
    it = values.find(key);
    if(it!=values.end())
    {
        values.erase(it);
        return 1;
    }
    else
    {
        cout<<"Cannot remove : This key does not exists"<<endl;
        return 0;
    }
}

void Bucket::search(int key)
{
    map<int,int>::iterator it;
    it = values.find(key);
    if(it!=values.end())
    {
        cout<<"Value = "<<it->second<<endl;
    }
    else
    {
        cout<<"This key does not exists"<<endl;
    }
}

int Bucket::isFull(void)
{
	if (values.size()<size)
		return 0;
	else if (overflow>0 )
	{
		overflow-=1;
		return 0;
	}
	else return 1;
}

int Bucket::getDepth(void)
{
    return depth;
}

int Bucket::increaseDepth(void)
{
    depth++;
    return depth;
}


map<int, int> Bucket::copy(void)
{
    map<int, int> temp(values.begin(),values.end());
    return temp;
}

void Bucket::clear(void)
{
    values.clear();
}


