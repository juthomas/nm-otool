#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/stab.h>





void	print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int	i;
	char	*stringtable;
	struct	nlist_64	*array;

	array = (void*)ptr + symoff;
	stringtable = (void*)ptr + stroff;

	for (i = 0; i < nsyms; i++)
	{
		//printf("%016llx : n_sect=%x n_type=%x n_desc=%x : %s\n", array[i].n_value,array[i].n_sect, array[i].n_type, array[i].n_desc, stringtable + array[i].n_un.n_strx);
		if ((array[i].n_type & N_TYPE) == N_UNDF)
		{
			printf("                 U %s\n", stringtable + array[i].n_un.n_strx);
		}
		else if ((array[i].n_type & N_TYPE) == N_SECT)
		{
			printf("%016llx T %x %s\n", array[i].n_value, array[i].n_sect,stringtable + array[i].n_un.n_strx);
		}
		else
		{
			printf("%016llx : n_sect=%x n_type=%x n_desc=%x : %s\n", array[i].n_value,array[i].n_sect, array[i].n_type, array[i].n_desc, stringtable + array[i].n_un.n_strx);
		}
	}

}

void	print_segment_command(char segname[16])
{
	for (int i = 0; i < 16; i++)
	{
		printf("->%c<-", segname[i]);
	}

}

void	handle_64(char *ptr)
{
	int						ncmds;
	int						i;
	struct	mach_header_64	*header;
	struct	load_command	*lc;
	struct	symtab_command	*sym;
	struct	segment_command_64	*sem;

	header = (struct mach_header_64*)ptr;
	ncmds = header->ncmds;
	lc = (void*)ptr + sizeof(struct mach_header_64);


	printf("Sizeof load commands :%x\n", header->sizeofcmds);
	for (i = 0; i < ncmds; i++)
	{
		printf("lc->cmd:%x lc->cmdsize:%x\n", lc->cmd, lc->cmdsize);
		if (lc->cmd == LC_SEGMENT)
		{
			sem = (struct segment_command_64 *)lc;
			print_segment_command(sem->segname);
			//printf("\naddr:%llx", header->sizeofcmds);
			//printf("\nsize:%llx\n", sem->vmsize);
		
		}
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			//print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			printf(".");
			break;
		}

		lc = (void*)lc + lc->cmdsize;
	}

}

void	nm(char *ptr)
{
	int		magic_number;

	magic_number = *(int*)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
	}


}

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
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (argc != 2)
	{
		fprintf(stderr, "Please give me an arg\n");
		return (EXIT_FAILURE);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) < 0)
	{
		perror("fstat");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	if (munmap(ptr, buf.st_size))
	{
		perror("munmap");;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}