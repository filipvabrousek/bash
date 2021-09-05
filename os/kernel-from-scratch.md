## Kernel from scratch

```asm
bits 16	
org 0x7C00                   ;BIOS loads bootloader at physical address 0x7C00 hence we have assemble our bootloader starting from that location.
JMP $ 		                   ;infinite loop
TIMES 510 - ($ - $$) db 0	   ;fill the rest of sector with 0
DW 0xAA55			               ;boot signature
```

## Installation

```sh
brew install qemu
brew install nasm
nasm boot.asm -f bin -o boot.bin && qemu-system-i386 -fda boot.bin
```
