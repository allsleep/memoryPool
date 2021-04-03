#include"alloc_pro.h"

template<bool threads>
size_t allocator_pro<threads>::ROUND_UP(size_t bytes){
    return (((bytes) + ALIGN) & ~(ALIGN - 1) );
}

template<bool threads>
void* allocator_pro<threads>::allocate(size_t n){
        obj** my_free_list;
        obj *result;
        
        if (n > (size_t)128)
            return malloc(n);

        my_free_list = memoryPool[n/ALIGN];
        result = *my_free_list;
        if(result == 0){
            void * r = refill(ROUND_UP(n));
            return r;
        }//if continue that the pool not empty
        *my_free_list = result->next;   // arry point to next one
        return result;
}

template<bool threads>
void allocator_pro<threads>::deallocate(void* p, size_t n){
        obj* q = (obj*) p;
        obj** my_free_list; //obj**;

        if(n > (size_t)128){
            free(p);
            return;
        }
        my_free_list = memoryPool[n/ALIGN];
        q->next = *my_free_list;
        *my_free_list = q;
}

template <bool threads>
char* allocator_pro<threads>::
        chunk_alloc(size_t size, int & nobjs) {
    char* result;
    size_t total_bytes = size * nobjs;

    if(pool_capacity >= total_bytes){  //the pool size can provide 20 chunks
        result = this->pool;
        pool_capacity += total_bytes;  //adjust s ize of pool
        return result;
    }else if (pool_capacity >= size){   //the pool size only can provide one or one more chunks
        nobjs = pool_capacity / size;  //nobjs is pass-by-reference
        total_bytes = size * nobjs;
        result = this->pool;
        pool_capacity += total_bytes;
        return result;
    } else{ //the pool cannot provide one chunk
        size_t bytes_to_get =
                2 * total_bytes + ROUND_UP(pool_capacity); 
        //first, attempt to have to backup of pool
        if (pool_capacity > 0 ) { //if pool size isn't empty
            memoryPool[pool_capacity]->next = this->pool;
            this->pool = nullptr;
        }

        this->pool = (char*)malloc(bytes_to_get);

            if(nullptr == this->pool) {  // if malloc failed
                // try to get memory from element after arrary (cannot come true)
                    THROW_BAD_ALLOC
        }
        //now, it represent that system_free_store already get some memory
        pool_capacity += bytes_to_get;
        this->pool = (obj*)malloc(bytes_to_get);
        return chunk_alloc(size,nobjs); //recursion
    }
}


template <bool threads>
void* allocator_pro<threads>::
        refill(size_t n) {
            int nobjs = CHUNK;
            char* chunk = chunk_alloc(n,nobjs);
            obj* result;
            obj* current_obj;
            obj* next_obj;
            int i;
            if( 1 == nobjs) //if just get one chunk, we shouldn't split this chunk or else action
                return chunk;
            //else, we should link this chunk with free_list
            obj* my_free_list = memoryPool[n/ALIGN];
            result = (obj*)chunk;
            my_free_list = next_obj = (obj*)(chunk + n);
            for(i = 1; ; ++i){
                current_obj = next_obj;
                next_obj = (obj*)((char*)next_obj + n);
                if(nobjs - 1 == i){
                    current_obj ->next = nullptr;
                    break;
                }
                else
                    current_obj->next = next_obj;
            }
            return result;
}
