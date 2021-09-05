## Assembly

### Hello World

```asm
section	.text
   global _start     ;must be declared for linker (ld)
	
section	.data
    MESSAGE db 'Hello, world!', 0xa  ;string to be printed
    LEN equ $ - MESSAGE     ;length of the string

_start:	            ;tells linker entry point
   mov	edx,LEN     ;message length
   mov	ecx,MESSAGE     ;message to write
   mov	ebx,1       ;file descriptor (stdout)
   mov	eax,4       ;system call number (sys_write)
   int	0x80        ;call kernel
	
   mov	eax,1       ;system call number (sys_exit)
   int	0x80        ;call kernel
```

### Sections
* ```section .data``` - initiated data or constants  
* ```section.bss``` - declaring variables  
* ```section.text``` - keeping the code  

```asm
section.text
   global _start
_start:
```


