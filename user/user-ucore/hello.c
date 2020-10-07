#include<stdio.h>
#include<syscall.h>

#define printf(...)                     fprintf(1, __VA_ARGS__)
int main() 
{
    printf("hello world!\n\r");
    sys_hello();
    return 0;
}