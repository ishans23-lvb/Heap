//
//  main.cpp
//  Heap
//
//  Created by Ishan Shah on 1/29/20.
//  Copyright © 2020 Ishan Shah. All rights reserved.
//

#include <iostream>
#include <assert.h>
using namespace std;

#ifdef DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif


template<typename T>
class heap
{
private:
    static unsigned long get_rci(unsigned long i)
    {
        return 2*i+2;
    }
    static unsigned long get_lci(unsigned long i)
    {
        return 2*i+1;
    }
    static unsigned long get_pi(unsigned long i)
    {
        if(i==0)
            return 0;
        return (i-1)/2;
    }
    static int basic_prior(const T& a,const T& b)
    {
        return (a>b) ? 1 : ( a==b ? 0 : -1);
    }
    void expand(unsigned long nlen)
    {
        ASSERT(this->len<=nlen);
        T* ptr=new T[nlen];
        for(unsigned long i=0;i<this->len;i++)
        {
            ptr[i]=A[i];
        }
        this->len=nlen;
        delete[] (this->A);
        this->A=ptr;
    }
private:
    T* A;
    
    unsigned long ni;
    unsigned long len;
    int(*prior)(const T& a,const T& b);//a>b <-> prior(a,b)>0; a==b <-> prior(a,b)==0; a<b <-> prior(a,b)<0

public:
    heap(const unsigned long len=100,int(*prior)(const T&,const T&)=basic_prior):A(nullptr),ni(0),len(len), prior(prior)
    {
        A=new T[this->len];
    }
    
    
    int push(const T& x)
    {
        ASSERT(validate());
        int ret=0;
        if((this->ni)>=(this->len))
        {
            ASSERT((this->ni)==(this->len));
            ASSERT(((this->len)*2LU)-(this->len)==(this->len));
            expand((this->len)*2);
            ret=1;
        }
        ASSERT((this->ni)<(this->len));
        
        A[(this->ni)]=x;
        
        for(unsigned long i=(this->ni);i>0;)
        {
            unsigned long pi=get_pi(i);
            if(prior(A[i],A[pi])>0)
            {
                T parent=A[pi];
                A[pi]=A[i];
                A[i]=parent;
            }
            else
            {
                ASSERT(lvalidate(i));
                break;
            }
            ASSERT(lvalidate(i));
            i=pi;
        }
        (this->ni)++;
        ASSERT(validate());
        return ret;
    }
    
    const T& top(void) const
    {
        ASSERT(validate());
        return A[0];
    }
    
    void pop(void)
    {
        ASSERT(validate());
        if(empty())
            return;
        
        ASSERT(this->ni>0);
        A[0]=A[(--(this->ni))];
        
        for(unsigned long i=0;i<(this->ni);)
        {
            const unsigned long lci=get_lci(i), rci=get_rci(i);
            unsigned long ci=0;
            ASSERT(lci>0 && rci>0);
            
            if(lci>=(this->ni))
            {
                break;
            }
            else if(rci>=(this->ni))
            {
                ASSERT(lci<(this->ni));
                ci=lci;
            }
            else
            {
                ASSERT(lci<(this->ni) && rci<(this->ni));
                if(prior(A[lci],A[rci])>=0)
                    ci=lci;
                else
                    ci=rci;
            }
            
            ASSERT(lci<(this->ni) && (lci>0 && rci>0));
            ASSERT(ci<(this->ni) && ci>0);
            if(prior(A[ci],A[i])>=0)
            {
                const T t=A[i];
                A[i]=A[ci];
                A[ci]=t;
                ASSERT(lvalidate(i));
                i=ci;
            }
            else
            {
                break;
            }
            
        }
        
        ASSERT(validate());
    }
    
    bool empty(void) const
    {
        return (this->ni)==0;
    }
    
    ~heap(void)
    {
        delete[] A;
    }
    
#ifdef DEBUG
    bool bvalidate(void) const
    {
        return ((this->len)>0) && ((this->ni)<=(this->len)) && (this->A) && (this->prior);
    }
    
    bool lvalidate(unsigned long i) const
    {
        ASSERT(i<(this->len));
        unsigned long lci=get_lci(i), rci=get_rci(i);
        unsigned long pi=get_pi(i);
        ASSERT(pi<=i && lci>i && rci>i);
        
        if(pi!=i && prior(A[pi],A[i])<0)
            return false;
        if(lci<(this->ni) && prior(A[i],A[lci])<0)
            return false;
        if(rci<(this->ni) && prior(A[i],A[rci])<0)
            return false;
        return true;
    }
    
    bool validate(void) const
    {
        if(!bvalidate())
            return false;
        for(unsigned long i=0;i<(this->ni);i++)
        {
            if(!lvalidate(i))
                return false;
        }
        return true;
    }
    
    void print(void) const
    {
        ASSERT(validate());
        for(unsigned long i=0;i<(this->ni);i++)
        {
            ASSERT(lvalidate(i));
            cout << A[i] << " ";
        }
        cout << endl;
    }
#endif
};

int main(int argc, const char * argv[])
{
    heap<int> h;
    for(int i=0;i<1000;i++)
    {
        int r=rand();
        h.push(r);
    }
    
    
    return 0;
}
