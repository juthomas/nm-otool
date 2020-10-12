#include "stdio.h"
#include "unistd.h"

void ft_putchar(char c)
{
    write(1, &c, 1);
}

int main()
{
    char tmp[4];
    printf("sizeof char[4] :%ld \n", sizeof(int));
}