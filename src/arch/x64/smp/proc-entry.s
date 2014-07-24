section .text

; program loaded at 0x5000, stack starting at 0x5000 (top), pml4 data resides
; at 0x3000

PROC_ADDR_BASE equ 0x5000
PROC_TEMP_CR3 equ 0x3000
PROC_GDT_PTR equ 0x4000

bits 16

global anarch_proc_entry
anarch_proc_entry:
  jmp 0:(.start - anarch_proc_entry + PROC_ADDR_BASE)
.start:
  ; stack setup before call:
  ; push qword RETURN_ADDR
  ; push qword REAL_CR3
  cli
  mov sp, PROC_ADDR_BASE - 0x10
  
  ; set PAE and PGE bits
  mov eax, 10100000b
  mov cr4, eax
  
  ; set CR3 to temporary page table
  mov eax, PROC_TEMP_CR3
  mov cr3, eax
  
  ; set LME, SCE, NXE bits of EFER MSR
  mov ecx, 0xC0000080
  rdmsr
  or eax, 0x901
  wrmsr
  
  ; enable paging and protection simultaneously
  mov ebx, cr0
  or ebx, 0x80000001
  mov cr0, ebx
  
  ; pop GDT base off the stack
  mov ebx, PROC_GDT_PTR
  lgdt [ebx]
  jmp 0x8:((.entry_64 - anarch_proc_entry) + PROC_ADDR_BASE)
  
bits 64
.entry_64:
  ; setup segment registers
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  xor ax, ax
  mov ss, ax
  ; pop real CR3 off stack
  pop rax
  mov cr3, rax
  ; jump to 64-bit CPU entry function
  ret

global anarch_proc_entry_end
anarch_proc_entry_end: