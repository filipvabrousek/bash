## Kernel from scratch

1) BIOS loads bootloader at physical address ```0x7C00``` hence we have assemble our bootloader starting from that location
2) infinite loop
3) fill the rest of sector with 0
4) boot signature

```asm
bits 16	
org 0x7C00                   
JMP $ 		                 
TIMES 510 - ($ - $$) db 0	 
DW 0xAA55			         
```

## Installation
```sh
brew install qemu
brew install nasm
nasm boot.asm -f bin -o boot.bin && qemu-system-i386 -fda boot.bin
```
