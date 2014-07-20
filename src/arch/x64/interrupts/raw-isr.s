section .text

extern InterruptCoded, InterruptNonCoded, InterruptPicEoi

; push all registers that are not preserved by a function in the System V ABI
%macro pushunpres 0
  push rdi
  push rsi
  push rdx
  push rcx
  push r8
  push r9
  push r10
  push r11
  push rax
%endmacro

; pop all registers pushunpres pushes
%macro popunpres 0
  pop rax
  pop r11
  pop r10
  pop r9
  pop r8
  pop rcx
  pop rdx
  pop rsi
  pop rdi
%endmacro

global RawNonCodedIsr
RawNonCodedIsr:
  pushunpres
  
  mov rdi, [rsp + 0x48] ; vector number
  lea rsi, [rsp + 0x50] ; ISR stack
  call InterruptNonCoded
  
  popunpres
  add rsp, 0x8
  iretq

global RawCodedIsr
RawCodedIsr:
  pushunpres

  mov rdi, [rsp + 0x48] ; vector number
  lea rsi, [rsp + 0x58] ; ISR stack
  mov rdx, [rsp + 0x50] ; exception code
  call InterruptCoded
  
  popunpres
  add rsp, 0x10
  iretq

global RawPicEoiIsr
RawPicEoiIsr:
  pushunpres
  
  mov rdi, [rsp + 0x48] ; vector number
  lea rsi, [rsp + 0x50] ; ISR stack
  call InterruptPicEoi
  
  popunpres
  add rsp, 0x8
  iretq
