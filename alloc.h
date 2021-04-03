#ifndef ALLOCH
#define ALLOCH
#include<iostream>

#define THROW_BAD_ALLOC \
    std::cerr<< "out of memory"; exit(1);

#define DECLARE_POOL_ALLOC() \
public: \
	void* operator new(size_t size) {return myAlloc.allocate(size);}	\
	void operator delete(void *p){ myAlloc.deallocate(p,0);}	 \
protected: \
	static allocator myAlloc;



class allocator{
protected:
    struct obj{    // (union point)内嵌指针
    obj* next;
    };

public:
    virtual void* allocate(size_t);
    virtual void deallocate(void*,size_t);
protected:
    obj* memoryPool[16] = {nullptr};
    const int CHUNK = 20;
};

#endif
