#include"alloc.h"

void* allocator::allocate(size_t size){
    obj* p;

    if(!memoryPool[size/8]){
		size_t chunk = CHUNK * size;
    memoryPool[size/8] = p = (obj*)malloc(chunk);

    if(nullptr == p)
        THROW_BAD_ALLOC

    for(int i = 0; i < CHUNK-1; ++i){
        p->next = (obj*)((char*)p+size);    //split memory
        p = p->next;
    }
    p->next = nullptr;
}
    p = memoryPool[size/8];
    memoryPool[size/8] = memoryPool[size/8]->next;
return p;
} 

void allocator::deallocate(void *p, size_t size){
    if(nullptr == p)
        return;
    ((obj*)p)->next	 = memoryPool[size/8];
    memoryPool[size/8] = (obj*)p;
}
