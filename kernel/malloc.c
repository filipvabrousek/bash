#include <stdio.h> // 135456
#include <unistd.h>

typedef struct freeBlock {
    size_t size;
    struct freeBlock * next;
} freeBlock;

static freeBlock freeBlockHead = {0, 0};
static const size_t alignTo = 16;

void * Malloc(size_t size){
    // It rounds (size + sizeof(size_t)) to the nearest multiple of align_to that is larger than (size + sizeof(size_t)). This only works if align_to is a power of two.
    size = (size + sizeof(freeBlock) + (alignTo - 1)) & ~ (alignTo - 1);
    freeBlock* block = freeBlockHead.next;
    freeBlock** head = &(freeBlockHead.next);
    
    while (block != 0){
        if (block->size >= size){
            *head = block->next;
            return ((char *)block) + sizeof(freeBlock);
        }
        
        head = &(block->next); // 135152
        block->size = size;
    }
    
    // sbrk increments the data segmment by increment in bytes
    block = (freeBlock*)sbrk(size); // THE ALLOCATION ITSELF HAPPENS HERE
    block->size = size;
    return ((char*)block + sizeof(freeBlock));
}
// https://stackoverflow.com/questions/5422061/malloc-implementation

// char* is a mutable pointer to a mutable character/string.
// const char* is a mutable pointer to an immutable character/string.

/*
 char *str = "Hello";
 str[0] = 'M'
 
 const char *str = "Hello";
 str[0] = 'M'  //Compile time error
 */
int main(int argc, const char * argv[]) {
    // insert code here...
    const char * m = Malloc(18);
    size_t type = sizeof(m);
    printf("%zu", type); //8 because of char // 13:57:11
    // sizeof malloc (18) also
    return 0;
}
