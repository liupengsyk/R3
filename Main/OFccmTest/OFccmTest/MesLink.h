#ifndef __MESLIST__
#define __MESLIST__

#include <iostream>
#include <assert.h>
using namespace std;

template<class Ty>
struct _Node
{
	_Node()
	{
	}

	_Node(const Ty& val,_Node *p1,_Node *p2)
	{
		data = val;
		prev = p1;
		next = p2;
	}

	Ty data;
	_Node *prev;
	_Node *next;
};

template<class Ty>
class _Itor
{
public:
	_Node < Ty > * node;

public:

	_Itor()
	{
		node = 0;
	}

	_Itor( _Node<Ty> *val)
	{
		node = val;
	}

	_Itor(const _Itor<Ty>&_Right)
	{
		node = _Right.node;
	}


	_Itor& operator=(const _Itor<Ty>_Right)
	{
		node = _Right.node;
		return *this;
	}


	bool operator ==(const _Itor<Ty>_Right)
	{
		return node == _Right.node;
	}


	bool operator != (const _Itor<Ty>_Right)
	{
		return node != _Right.node;
	}


	_Itor& operator++()
	{
		node = node->next;
		return *this;
	}


	_Itor& operator--()
	{
		node = node->prev;
		return *this;
	}


	Ty& operator *()
	{
		return node->data;
	}


	Ty* operator->()
	{
		return &**this;
	}

};

template<class Ty>
class List
{
public:
	typedef _Node<Ty> MesNode;
	typedef _Itor<Ty> MesItor;

public:

	~List()
	{
	      
		   clear();	  
		   if(_MesHead!=NULL)
		   delete _MesHead;
	}
	List()
	{
		   empty_init();
	}
	

	List(const Ty& val)
	{
		empty_init();
		MesNode *tmp = new MesNode;
		insert(_MesHead,tmp);
	}


	List(const Ty& val,size_t count)
	{
		empty_init();
		for(size_t i = 0; i < count; --i)
		{
			MesNode *tmp = new MesNode;
			insert(_MesHead,tmp);
		}
	}
  
	List( List< Ty >& val)
	{
		empty_init();
        
		MesItor it=val.begin();

		for(it=val.begin();it != val.end();++it)
		{
		
		   insertBefore(_MesHead,it.node);
		}
	}
         
		Ty &operator[] (long i)
		{
			long j=0;
			  MesNode *next = _MesHead->next; 
			  while(j<i)
			  {
				  next=next->next;
				  j++;
			  }
			  return next->data;
		}

	Ty& front()
	{
		return begin().node->data;
	}

	Ty& back()
	{
		return end().node->data;
	}

	MesItor begin()
	{
		return MesItor(_MesHead->next);
	}

    MesNode * getHead()
	{
		return _MesHead;
	}
	MesItor end()
	{
		return MesItor(_MesHead);
	}

	void push_front(Ty val)
	{
         MesNode * tmp=alloc(val);
		 MesItor  itor=begin();
		 insertBefore(itor.node,tmp);	
        
	}	
	
	void push_back(Ty val)
	{
		  MesNode *tmp=alloc(val);
		  MesItor  itor=MesItor(_MesHead->prev);
          insert(itor.node,tmp);
		  
	}

	Ty pop_front()
	{
         
         Ty ret=_MesHead->next->data;
		 leaveLink(_MesHead->next);
		 return ret;

		  
	}

	Ty  pop_back()
	{
          Ty  ret=_MesHead->prev->data;
		  leaveLink(_MesHead->prev);
          return ret;
	}


	void clear()
	{
		   MesNode *node=_MesHead->next;
		   while(node->next!=node)
		   {
				node=leaveLink(node);
		   }
	}

	size_t size()
	{
		return _MesCount;
	}

	
	bool empty()
	{
		return _MesCount == 0;
	}

		void resize(int _N)
		{  
             int curSize=0;
			 	 int i=0;
				 curSize=size();
				 if(_N==0) {
					 clear();	  
					 return;
				 }
			if (curSize < _N)
			{
                 for(i=0;i<_N-curSize;i++)
				 {
				     push_back((Ty)0.0);
				 }
			}
			else if (_N < size())
			{
				 pop_back();
			}
			else
			{
				;
			}
		}

	List<Ty> & operator =( List<Ty>& val)
		{
			
			empty_init();
			MesNode * head=val.getHead();
			MesNode *next=head->next;
			MesNode *tmp=NULL;
			while(next!=head)
			{
			  push_back(next->data);
				next=next->next;
			}
				
		    return *this;
	}
private:
	MesNode* alloc(const Ty& val)
	{
		MesNode *tmp = new MesNode;
		tmp->data = val;
		tmp->prev = tmp->next = 0;
		return tmp;
	}
	void empty_init()
	{
		_MesHead = new MesNode;
		_MesHead->next = _MesHead->prev = _MesHead;
	
		_MesCount = 0;
	}

	void up_count()
	{
		_MesCount++;
	}

	void down_count()
	{
		assert(_MesCount > 0 );
		_MesCount--;
	}
      
	MesNode* insert(MesNode *pos,MesNode *node)
	{
		assert(pos != 0 && node != 0);

		MesNode *next = pos->next;
         
		pos->next = node;
		node->prev = pos;
        
		next->prev = node;
		node->next = next;
		

		up_count();

		return node;
	}

	 MesNode *insertBefore(MesNode *pos,MesNode *node)
	 {
		  	assert(pos != 0 && node != 0);

			MesNode   *prev=pos->prev;
			pos->prev=node;
			node->next=pos;
             
			prev->next=node;
			node->prev=prev;
            up_count();
          
		    return node;    
	 }
	 MesNode *leaveLink(MesNode*pos)
	 {
		     assert(pos!=_MesHead  &&pos!=0);
			 MesNode   *prev=pos->prev;
             MesNode *next = pos->next;
			 prev->next=next;
			 next->prev=prev;

              down_count();
			  delete pos;
              return next;
			 

	 }
private:
	MesNode *_MesHead;
	size_t _MesCount;
};

#endif