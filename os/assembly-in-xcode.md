## Assembly in Xcode





```c
#include "stdio.h"

int main(int argc, const char * argv[]) {

int result = 0;
    
asm {
    mov result, 3
}

    printf("Value is %d\n", result);  // Value is 3
    return 0;
}
```


```c
int result = 0;
    
asm {
    mov result, 3
}

  printf("Value is %d\n", result);  // Value is 3
```

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

SOURCE: https://stackoverflow.com/questions/20556241/xcode-and-nasm-coding

