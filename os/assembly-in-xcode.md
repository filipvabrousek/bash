## Assembly in Xcode

### Simple mov
```c
#include "stdio.h"

int main() {

int result = 0;
    
asm {
    mov result, 3
}

    printf("Value is %d\n", result);  // Value is 3
    return 0;
}
```



### inc
```c

 int value = 0;
     
 asm {
     mov rax, value
     inc value
     inc value
     inc value
     inc value
     dec value
 }

     printf("Value is %d\n", value);  // Value is 3
 
```



### add
```c
#include "stdio.h"

int main(int argc, const char * argv[]) {

int result = 0;
    
asm {
    mov eax, 3
    mov ebx, 4
    add eax, ebx
    mov result, eax
}

    printf("Value is %d\n", result);  // Value is 3
    return 0;
}
```




## jg and jump conditions

```asm

#include "stdio.h"

int main(int argc, const char * argv[]) {

int result = 0;
    
asm {
    mov ax,1
    mov bx,2
    cmp ax,bx
    jg larger
    jmp both
    
    larger:
    mov result,30
    
    both:
}

    printf("Value is %d\n", result); // Value is 0 as expected
    return 0;
}


```
SOURCE: https://stackoverflow.com/questions/20556241/xcode-and-nasm-coding


Wait for StackOverflow
