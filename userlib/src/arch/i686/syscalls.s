bits 32

%macro make_syscall 2
global %1
%1:
    mov eax, %2
    int 0x80
    ret
%endmacro

make_syscall open, 5
make_syscall read, 3
make_syscall write, 4
make_syscall close, 6
make_syscall exit, 1
make_syscall mmap, 197
make_syscall munmap, 73
make_syscall fork, 2
make_syscall execve, 59
make_syscall wait4, 400