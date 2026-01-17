global _start
section .text
extern print
_start:
sub rsp, 8
mov rax, 10
push rax
mov rax, 2
push rax
pop rax
pop rbx
sub rbx, rax
push rbx
mov rax, 1
push rax
pop rax
pop rbx
sub rbx, rax
push rbx
pop rax
mov [rsp + 8], rax
mov rdi, rax
call print
add rsp, 8
mov rax, 60
xor rdi, rdi
syscall
