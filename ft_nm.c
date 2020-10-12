#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	while (*str != '\0')
	{
		ft_putchar(*str++);
	}
}

int main(int argc, char **argv)
{
	int fd;
	char *str;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			ft_putstr("Fd error\n");
			return (-1);
		}
		int i = 0;
		char tmp[1];
		while (read(fd, tmp, 1) > 0)
		{
			if (tmp[0] > 32 && tmp[0] < 126)
			{
				while (tmp[0] > 32 && tmp[0] < 126)
				{
					ft_putchar(tmp[0]);
					if (read(fd, tmp, 1) <= 0)
					{
						break;
					}
				}
				ft_putchar('\n');
			}
		}
	}
}