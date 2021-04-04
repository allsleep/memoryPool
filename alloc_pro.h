#ifndef ALLOC_PROH
#define ALLOC_PROH

#include"alloc.h"

#define DECLARE_POOL_ALLOC_PRO() \
public: \
	void* operator new(size_t size) {return myAlloc.allocate(size);}	\
	void operator delete(void *p){ myAlloc.deallocate(p,0);}	 \
protected: \
	static allocator_pro<false> myAlloc;

template<bool threads>
class allocator_pro:public allocator{
public:
    void* allocate(size_t) override;
    void deallocate(void*,size_t) override;
protected:
    char* chunk_alloc(size_t size, int &nobjs);
    void *refill(size_t n);
    char* pool = nullptr;
    size_t ROUND_UP(size_t);
    size_t pool_capacity = 0;
    size_t ALIGN = 8;
};
// not implemented to return memory to the system 
#endif
