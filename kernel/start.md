# Kernel

1) install Docker + brew install qemu
2) cd to folder with GitHub project
2) docker build buildenv -t myos-buildenv
3) ON THE FIRST AND SUBSEQUENT RUNS (make 2 Terminal Windows):
4) docker run --rm -it -v "$(pwd)":/root/env myos-buildenv
5) make build-x86_64 (IN THE SAME CONSOLE (YOU SHOULD SEE env#): )
6) qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso ( OTHER TERMINAL IN THE SAME FOLDER:)
 
We need the build script from Google Drive.
```c
#define GREEN 2
#define VGA_ADDRESS 0xB8000
#define VGADUO_ADDRESS 0xB8000

unsigned short *terminal_buffer;
unsigned int vga_index;

char *my_itoa(int num, char * str);
int digitCount(int num);
void reverse(char s[]);

void kernel_main() {
    writeText("Hello Filip");
    writeNumber(123);
}
// 13:17:31 17/06/2021

//--------------------------------------WRITE TEXT
void writeText(const char * s){

char *base = (void *) 0xb8000;

while (*s) {
    *base++ = *s++;
    *base++ = 0x0f;
    }
}

void writeNumber(int num){
    char str[digitCount(num)+1];
    my_itoa(num, str);
    reverse(str); // 21:03:19 WOW 17/06/2022
    writeText(str);
}

char *my_itoa(int num, char * str){
    int i = 0;
    int base = 10;

    while (num != 0){
        int rem = num % base;
        str[i++] = (rem > 9)? (rem - 10) + 'a' : rem + '0';
        num = num/base;
    }
}

int digitCount(int num){
int count = 0;
while(num > 0){
    count++;
    num = num / 10;
}

return count;
}
// implement strlen


void reverse(char s[]){
    char r[1000];
    int begin, end, count = 0;

    while(s[count] != '\0'){ count++; }
    end = count - 1;

    for (begin = 0; begin < count; begin++){
        r[begin] = s[end];
        end--;
    }

    r[begin] = '\0';
    return r;
}

// 20:53:50
void colorPrint(char * str, unsigned char color){
int index = 0;
while (str[index]){
    terminal_buffer[vga_index] = (unsigned short)str[index]|(unsigned short)color << 8;
    index++;
    vga_index++;
}
}

// fuck protos
//void colorPrint(char * str, unsigned char color);
//void writeText(const char * s);

// 17:10:57


/*
terminal_buffer = (unsigned short *)VGA_ADDRESS;
 vga_index = 0;
 colorPrint("Hey green", GREEN); // you can run only one at a time, or they will owerwrite each other
*/
```



## asm


header.asm

```asm
section .multiboot_header
header_start:
	; magic number
	dd 0xe85250d6 ; multiboot2 ; architecture
	dd 0 ; protected mode i386
	dd header_end - header_start ; header length
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; checksum

	; end tag
	dw 0
	dw 0
	dd 8
header_end:
```


main.asm
```asm
; https://os.phil-opp.com/entering-longmode/
global start
extern long_mode_start; this will be called from main64.asm (whic I do not have)

section .text
bits 32
start:
	mov esp, stack_top
	; we need to set up paging and switch to the 64-bit long mode to use 64 bit c code
	call setup_page_tables
	call enable_paging

	lgdt [gdt64.pointer] ; load global descriptor table (below) with a table
	jmp gdt64.code_segment:long_mode_start ; load call segment into code selector
	hlt

setup_page_tables:
	mov eax, page_table_l3 ; take address of L3 table and move it to EAX
	or eax, 0b11 ; present, writable flags are enabled 
	mov [page_table_l4], eax ; take adress with flags from the line above 
	;an add it as the 1st entry in the L4 table
	 
	; same for the L2 table, we do not need to take care of L1
	mov eax, page_table_l2
	or eax, 0b11 ; present, writable
	mov [page_table_l3], eax

	mov ecx, 0 ; counter
.loop: ;---------------------- Mapping the whole table---------

	mov eax, 0x200000 ; 2MiB
	mul ecx ; multiply aex * ecx to create correct adress for our next page
	or eax, 0b10000011 ; present, writable, huge page
	mov [page_table_l2 + ecx * 8], eax
	; put the entry from the line above to the L2 table, with offset (counter * 8)

	; loop for creating the page table o 512 size
	inc ecx ; increment counter
	cmp ecx, 512 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	ret

enable_paging:
	; pass page table location to cpu
	mov eax, page_table_l4 ; move address of L4 table in the "eax" register
	;CPU looks for table adress in the cr3 register
	; so, we move "eax" here
	mov cr3, eax ;moving left to right, value of eax CHANGES
	
	; enable PAE = Physical adress extension necessary for 64-bit paging
	mov eax, cr4 ;enabling PAE flag cr4 register
	or eax, 1 << 5 ;enable the 5th bit (the bit for the PAE flag)
	mov cr4, eax ;save changes back to the cr4 register

	; enable long mode
	; = the mode where a 64-bit operating system can access 64-bit instructions and registers
	mov ecx, 0xC0000080
	rdmsr ;read model specific register instruction in ECX into EAX
	or eax, 1 << 8 ;enable long mode fag on bit 8
	wrmsr ;wrote it back to rdmsr

	; enable paging
	mov eax, cr0; enable paging flag in cr0 register
	or eax, 1 << 31; copy it to eax and enable paging bit 31
	mov cr0, eax; move back to cr0 register

	ret; return from the subroutine

error:
	; print "ERR: X" where X is the error code
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt

;reserve bytes for tables
section .bss
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096
stack_bottom:
	resb 4096 * 4
stack_top:

; create global descriptor table
; not much purpose bacause we use paging, but still reuired to enter 64-bit mode
section .rodata; read only data section
gdt64: ;global descriptor table
	dq 0 ; zero entry
.code_segment: equ $ - gdt64; current address ($) - start of the table 
	; 1 << 43 = executable flag, 
	; 1 << 47 = present flag
	; 1 << 53 = 64 bit flag (DQ = define quad word size (64 bits) variables)
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; code segment
.pointer: ;pointer to global secriptor table
	dw $ - gdt64 - 1 ; length - 1
	dq gdt64 ; address
```

main64.asm
```asm
global long_mode_start
extern kernel_main

section .text
bits 64
long_mode_start:
    ; load null into all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

	call kernel_main
    hlt
```

linker.ld
```ld
ENTRY(start) 

SECTIONS
{
	. = 1M;

	.boot :
	{
		KEEP(*(.multiboot_header))
	}

	.text :
	{
		*(.text)
	}
}
```

