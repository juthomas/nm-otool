#include <stdio.h>
#include <sys/mman.h>
// #include <mach-o/loader.h>
// #include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
// # include "../lib/libft/includes/libft.h"
#include "../lib/libft/includes/libft.h"

// #include "../include/ft_nm.h"
#define MH_MAGIC_64 ((0x7f << 0) + (0x45 << 8) + (0x4c << 16) + (0x46 << 24)) //custom linux magic shit

#define offsetof(st, m) \
    ((size_t)&(((st *)0)->m))



#define SHT_NULL			0x00		//Section header table entry unused
#define SHT_PROGBITS 		0x01		//Program data
#define SHT_SYMTAB			0x02		//Symbol table
#define SHT_STRTAB			0x03		//String table
#define SHT_RELA			0x04		//Relocation entries with addends
#define SHT_HASH			0x05		//Symbol hash table
#define SHT_DYNAMIC			0x06		//Dynamic linking information
#define SHT_NOTE			0x07		//Notes
#define SHT_NOBITS			0x08		//Program space with no data (bss)
#define SHT_REL				0x09		//Relocation entries, no addends
#define SHT_SHLIB			0x0A		//Reserved
#define SHT_DYNSYM			0x0B		//Dynamic linker symbol table
#define SHT_INIT_ARRAY		0x0E		//Array of constructors
#define SHT_FINI_ARRAY		0x0F		//Array of destructors
#define SHT_PREINIT_ARRAY	0x10		//Array of pre-constructors
#define SHT_GROUP			0x11		//Section group
#define SHT_SYMTAB_SHNDX	0x12		//Extended section indices
#define SHT_NUM				0x13		//Number of defined types.
#define SHT_LOOS			0x60000000	//Start OS-specific. 

// void	ft_putchar(char c)
// {
// 	write(1, &c, 1);
// }

// void	ft_putstr(char *str)
// {
// 	while (*str)
// 	{
// 		ft_putchar(*str++);
// 	}	
// }



void	print_section_type(int section_type)
{
	if (section_type == SHT_NULL			)
		ft_putstr("Section header table entry unused");
	if (section_type == SHT_PROGBITS 		)
		ft_putstr("Program data");
	if (section_type == SHT_SYMTAB			)
		ft_putstr("Symbol table");
	if (section_type == SHT_STRTAB			)
		ft_putstr("String table");
	if (section_type == SHT_RELA			)
		ft_putstr("Relocation entries with addends");
	if (section_type == SHT_HASH			)
		ft_putstr("Symbol hash table");
	if (section_type == SHT_DYNAMIC			)
		ft_putstr("Dynamic linking information");
	if (section_type == SHT_NOTE			)
		ft_putstr("Notes");
	if (section_type == SHT_NOBITS			)
		ft_putstr("Program space with no data (bss)");
	if (section_type == SHT_REL				)
		ft_putstr("Relocation entries, no addends");
	if (section_type == SHT_SHLIB			)
		ft_putstr("Reserved");
	if (section_type == SHT_DYNSYM			)
		ft_putstr("Dynamic linker symbol table");
	if (section_type == SHT_INIT_ARRAY		)
		ft_putstr("Array of constructors");
	if (section_type == SHT_FINI_ARRAY		)
		ft_putstr("Array of destructors");
	if (section_type == SHT_PREINIT_ARRAY	)
		ft_putstr("Array of pre-constructors");
	if (section_type == SHT_GROUP			)
		ft_putstr("Section group");
	if (section_type == SHT_SYMTAB_SHNDX	)
		ft_putstr("Extended section indices");
	if (section_type == SHT_NUM				)
		ft_putstr("Number of defined types.");
	if (section_type == SHT_LOOS			)
		ft_putstr("Start OS-specific."); 
}




typedef struct s_elf_header_64 {
	uint32_t		ei_magic_number; // 0x7F followed by ELF(45 4c 46)
	uint8_t	ei_class; // 1 for 32 bits, 2 for 64 bits
	uint8_t	ei_data; // This byte is set to either 1 or 2 to signify little or big endianness
	uint8_t	ei_version; // Set to 1 for the original and current version of ELF.
	uint8_t	ei_osabi;	// 0x00 	System V
					// 0x01 	HP-UX
					// 0x02 	NetBSD
					// 0x03 	Linux
					// 0x04 	GNU Hurd
					// 0x06 	Solaris
					// 0x07 	AIX
					// 0x08 	IRIX
					// 0x09 	FreeBSD
					// 0x0A 	Tru64
					// 0x0B 	Novell Modesto
					// 0x0C 	OpenBSD
					// 0x0D 	OpenVMS
					// 0x0E 	NonStop Kernel
					// 0x0F 	AROS
					// 0x10 	Fenix OS
					// 0x11 	CloudABI
					// 0x12 	Stratus Technologies OpenVOS
	uint8_t	ei_abiversion;
	uint8_t	ei_void1;
	uint8_t	ei_void2;
	uint8_t	ei_void3;
	uint8_t	ei_void4;
	uint8_t	ei_void5;
	uint8_t	ei_void6;
	uint8_t	ei_void7;
	uint16_t	type;	// 0x00 	ET_NONE
					// 0x01 	ET_REL
					// 0x02 	ET_EXEC
					// 0x03 	ET_DYN
					// 0x04 	ET_CORE
					// 0xfe00 	ET_LOOS
					// 0xfeff 	ET_HIOS
					// 0xff00 	ET_LOPROC
					// 0xffff 	ET_HIPROC 
	uint16_t	machine;// 0x00 	No specific instruction set
					// 0x01 	AT&T WE 32100
					// 0x02 	SPARC
					// 0x03 	x86
					// 0x04 	Motorola 68000 (M68k)
					// 0x05 	Motorola 88000 (M88k)
					// 0x06 	Intel MCU
					// 0x07 	Intel 80860
					// 0x08 	MIPS
					// 0x09 	IBM_System/370
					// 0x0A 	MIPS RS3000 Little-endian
					// 0x0B - 0x0D 	Reserved for future use
					// 0x0E 	Hewlett-Packard PA-RISC
					// 0x0F 	Reserved for future use
					// 0x13 	Intel 80960
					// 0x14 	PowerPC
					// 0x15 	PowerPC (64-bit)
					// 0x16 	S390, including S390x
					// 0x28 	ARM (up to ARMv7/Aarch32)
					// 0x2A 	SuperH
					// 0x32 	IA-64
					// 0x3E 	amd64
					// 0x8C 	TMS320C6000 Family
					// 0xB7 	ARM 64-bits (ARMv8/Aarch64)
					// 0xF3 	RISC-V
	uint32_t		version;// Set to 1 for the original version of ELF.
	uint64_t	entry;	// This is the memory address of the entry point from where the process starts executing
	uint64_t	phoff;	// Points to the start of the program header table		<<<<
	uint64_t	shoff;	// Points to the start of the section header table. 	<<<<
	uint32_t		flags;	// Interpretation of this field depends on the target architecture. 
	uint16_t	ehsize; // Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes for 32-bit format. 
	uint16_t	phentsize; // Contains the size of a program header table entry. 
	uint16_t	phnum; // Contains the number of entries in the program header table. 
	uint16_t	shentsize; // Contains the size of a section header table entry. 
	uint16_t	shnum; //  	Contains the number of entries in the section header table. 
	uint16_t	shstrndx;//  	Contains index of the section header table entry that contains the section names. 

}			t_elf_header_64;



typedef struct	s_program_header_64
{
	uint32_t	type; //0x00000000 	PT_NULL 	Program header table entry unused
					// 0x00000001 	PT_LOAD 	Loadable segment
					// 0x00000002 	PT_DYNAMIC 	Dynamic linking information
					// 0x00000003 	PT_INTERP 	Interpreter information
					// 0x00000004 	PT_NOTE 	Auxiliary information
					// 0x00000005 	PT_SHLIB 	reserved
					// 0x00000006 	PT_PHDR 	segment containing program header table itself
					// 0x00000007 	PT_TLS 	Thread-Local Storage template
					// 0x60000000 	PT_LOOS 	see below
					// 0x6FFFFFFF 	PT_HIOS
					// 0x70000000 	PT_LOPROC
					// 0x7FFFFFFF 	PT_HIPROC
	uint32_t	flags; // Segment-dependent flags (position for 64-bit structure). 
	uint64_t	offset; // Offset of the segment in the file image. 
	uint64_t	vaddr; // Virtual address of the segment in memory. 
	uint64_t	paddr; // On systems where physical address is relevant, reserved for segment's physical address. 
	uint64_t	filesz; // Size in bytes of the segment in the file image. May be 0. 
	uint64_t	memsz; //  	Size in bytes of the segment in memory. May be 0. 
	uint64_t	align; //  	0 and 1 specify no alignment. Otherwise should be a positive, integral power of 2, with p_vaddr equating p_offset modulus p_align.
}				t_program_header_64;








typedef struct	s_section_header_64
{
	uint32_t			name;// An offset to a string in the .shstrtab section that represents the name of this section.
	uint32_t			type;	// 0x0 	SHT_NULL 	Section header table entry unused
						// 0x1 	SHT_PROGBITS 	Program data
						// 0x2 	SHT_SYMTAB 	Symbol table
						// 0x3 	SHT_STRTAB 	String table
						// 0x4 	SHT_RELA 	Relocation entries with addends
						// 0x5 	SHT_HASH 	Symbol hash table
						// 0x6 	SHT_DYNAMIC 	Dynamic linking information
						// 0x7 	SHT_NOTE 	Notes
						// 0x8 	SHT_NOBITS 	Program space with no data (bss)
						// 0x9 	SHT_REL 	Relocation entries, no addends
						// 0x0A 	SHT_SHLIB 	Reserved
						// 0x0B 	SHT_DYNSYM 	Dynamic linker symbol table
						// 0x0E 	SHT_INIT_ARRAY 	Array of constructors
						// 0x0F 	SHT_FINI_ARRAY 	Array of destructors
						// 0x10 	SHT_PREINIT_ARRAY 	Array of pre-constructors
						// 0x11 	SHT_GROUP 	Section group
						// 0x12 	SHT_SYMTAB_SHNDX 	Extended section indices
						// 0x13 	SHT_NUM 	Number of defined types.
						// 0x60000000 	SHT_LOOS 	Start OS-specific. 
	uint64_t		flags;	// 0x1 	SHF_WRITE 	Writable
						// 0x2 	SHF_ALLOC 	Occupies memory during execution
						// 0x4 	SHF_EXECINSTR 	Executable
						// 0x10 	SHF_MERGE 	Might be merged
						// 0x20 	SHF_STRINGS 	Contains null-terminated strings
						// 0x40 	SHF_INFO_LINK 	'sh_info' contains SHT index
						// 0x80 	SHF_LINK_ORDER 	Preserve order after combining
						// 0x100 	SHF_OS_NONCONFORMING 	Non-standard OS specific handling required
						// 0x200 	SHF_GROUP 	Section is member of a group
						// 0x400 	SHF_TLS 	Section hold thread-local data
						// 0x0ff00000 	SHF_MASKOS 	OS-specific
						// 0xf0000000 	SHF_MASKPROC 	Processor-specific
						// 0x4000000 	SHF_ORDERED 	Special ordering requirement (Solaris)
						// 0x8000000 	SHF_EXCLUDE 	Section is excluded unless referenced or allocated (Solaris) 
	uint64_t		addr;	// Virtual address of the section in memory, for sections that are loaded. 
	uint64_t		offset;	// Offset of the section in the file image. 
	uint64_t		size;	// Size in bytes of the section in the file image. May be 0. 
	uint32_t			link;	// Contains the section index of an associated section. This field is used for several purposes, depending on the type of section. 
	uint32_t			info;	// Contains extra information about the section. This field is used for several purposes, depending on the type of section. 
	uint64_t		addralign; // Contains the required alignment of the section. This field must be a power of two. 
	uint64_t		entsize; // Contains the size, in bytes, of each entry, for sections that contain fixed-size entries. Otherwise, this field contains zero. 

}				t_section_header_64;


typedef struct s_test {
	int hello;
	int yes;


}				t_test;

t_elf_header_64		serialize_elf_header_64(void *addr)
{
	t_elf_header_64 header;
	header.ei_magic_number	= *((uint32_t*)(addr + 0x00));
	header.ei_class			= *((uint8_t*)(addr + 0x04));
	header.ei_data			= *((uint8_t*)(addr + 0x05));
	header.ei_version		= *((uint8_t*)(addr + 0x06));
	header.ei_osabi			= *((uint8_t*)(addr + 0x07));
	header.ei_abiversion	= *((uint8_t*)(addr + 0x08));
	header.type				= *((uint16_t*)(addr + 0x10));
	header.machine			= *((uint16_t*)(addr + 0x12));
	header.version			= *((uint32_t*)(addr + 0x14));
	header.entry			= *((uint64_t*)(addr + 0x18));
	header.phoff			= *((uint64_t*)(addr + 0x20));
	header.shoff			= *((uint64_t*)(addr + 0x28));
	header.flags			= *((uint32_t*)(addr + 0x30));
	header.ehsize			= *((uint16_t*)(addr + 0x34));
	header.phentsize		= *((uint16_t*)(addr + 0x36));
	header.phnum			= *((uint16_t*)(addr + 0x38));
	header.shentsize		= *((uint16_t*)(addr + 0x3A));
	header.shnum			= *((uint16_t*)(addr + 0x3C));
	header.shstrndx			= *((uint16_t*)(addr + 0x3E));
	return (header);
}

t_program_header_64	serialize_program_header(void *addr)
{
	t_program_header_64	header;
	header.type				= *((uint32_t*)(addr + 0x00));
	header.flags			= *((uint32_t*)(addr + 0x04));
	header.offset			= *((uint64_t*)(addr + 0x08));
	header.vaddr			= *((uint64_t*)(addr + 0x10));
	header.paddr			= *((uint64_t*)(addr + 0x18));
	header.filesz			= *((uint64_t*)(addr + 0x20));
	header.memsz			= *((uint64_t*)(addr + 0x28));
	header.align			= *((uint64_t*)(addr + 0x30));
	return (header);
}


t_section_header_64	serialize_section_header(void *addr)
{
	t_section_header_64 header;
	header.name				= *((uint32_t*)(addr + 0x00));
	header.type				= *((uint32_t*)(addr + 0x04));
	header.flags			= *((uint64_t*)(addr + 0x08));
	header.addr				= *((uint64_t*)(addr + 0x10));
	header.offset			= *((uint64_t*)(addr + 0x18));
	header.size				= *((uint64_t*)(addr + 0x20));
	header.link				= *((uint32_t*)(addr + 0x28));
	header.info				= *((uint32_t*)(addr + 0x2C));
	header.addralign		= *((uint64_t*)(addr + 0x30));
	header.entsize			= *((uint64_t*)(addr + 0x38));
	return (header);
}

void print_elf_header(char *ptr)
{
	t_elf_header_64	*elf_header;
	elf_header = (t_elf_header_64*)(ptr);

	printf("retest : %x\n", (*(t_elf_header_64*)(ptr)).ei_magic_number);
	printf("----------------------\n");
	printf(" Printing ELF HEADER\n");
	printf("----------------------\n");
	printf("magic number          : %x\n", elf_header->ei_magic_number);
	printf("32 or 64 bits (1 or 2): %d\n", elf_header->ei_class);
	printf("ei data (1 or 2)         : %d\n" , elf_header->ei_data);
	printf("ei version               : %d\n", elf_header->ei_version);
	printf("Osabi                    : %x\n", elf_header->ei_osabi);
	printf("Abiversion               : %d\n", elf_header->ei_abiversion);
	printf("Object file type         : %x\n", elf_header->type);
	printf("Machine                  : %x\n", elf_header->machine);
	printf("Version                  : %x\n", elf_header->version);
	printf("Entry                    : %lx\n", elf_header->entry);
	printf("Program header table     : %lx\n", elf_header->phoff);
	printf("Section header table     : %lx\n", elf_header->shoff);
	printf("Flags                    : %d\n", elf_header->flags);
	printf("Ehsize                   : %d\n", elf_header->ehsize);
	printf("Phentsize                : %d\n", elf_header->phentsize);
	printf("Phnum                    : %d\n", elf_header->phnum);
	printf("Shentsize                : %d\n", elf_header->shentsize);
	printf("Shnum                    : %d\n", elf_header->shnum);
	printf("Shstrndx                 : %d\n", elf_header->shstrndx);
	printf("----------------------\n");
	printf("\n");

	t_elf_header_64 wikimapped_header;
	wikimapped_header = serialize_elf_header_64(ptr);


	printf("----------------------\n");
	printf(" Printing ELF HEADER\n");
	printf("----------------------\n");
	printf("magic number          : %x\n", wikimapped_header.ei_magic_number);
	printf("32 or 64 bits (1 or 2): %d\n", wikimapped_header.ei_class);
	printf("ei data (1 or 2)         : %d\n" , wikimapped_header.ei_data);
	printf("ei version               : %d\n", wikimapped_header.ei_version);
	printf("Osabi                    : %x\n", wikimapped_header.ei_osabi);
	printf("Abiversion               : %d\n", wikimapped_header.ei_abiversion);
	printf("Object file type         : %x\n", wikimapped_header.type);
	printf("Machine                  : %x\n", wikimapped_header.machine);
	printf("Version                  : %x\n", wikimapped_header.version);
	printf("Entry                    : %lx\n", wikimapped_header.entry);
	printf("Program header table     : %ld\n", wikimapped_header.phoff);
	printf("Section header table     : %ld\n", wikimapped_header.shoff);
	printf("Flags                    : %d\n", wikimapped_header.flags);
	printf("Ehsize                   : %d\n", wikimapped_header.ehsize);
	printf("Phentsize                : %d\n", wikimapped_header.phentsize);
	printf("Phnum                    : %d\n", wikimapped_header.phnum);
	printf("Shentsize                : %d\n", wikimapped_header.shentsize);
	printf("Shnum                    : %d\n", wikimapped_header.shnum);
	printf("Shstrndx                 : %d\n", wikimapped_header.shstrndx);
	printf("----------------------\n");
	printf("\n");





	t_section_header_64 *section_header;
	
	section_header = (void*)ptr +  wikimapped_header.shoff;

	ft_print_memory((void*)ptr, 0x04000);
	// ft_putchar('h');

	for (int i = 0; i < wikimapped_header.shnum; i++)
	{

		//section_header = (t_section_header_64*)(ptr + elf_header->shoff + sizeof(t_section_header_64) * i );
		printf("----------------------\n");
		printf(" Printing Section HEADER : %d\n", i);
		printf("----------------------\n");
		printf("CurrentOffset           : %lx\n\n", (long int)section_header);

		printf("Name                    : %d\n", section_header->name);
		printf("Type                    : %x\n", section_header->type);
		printf("Addr                    : %ld\n", section_header->addr);
		printf("Addr                    : %ld\n", section_header->addr);
		printf("Size                    : %ld\n", section_header->size);
		section_header = (void *)section_header + wikimapped_header.shentsize;
	}

	printf("sizeoff section   : %lx\n", sizeof(t_section_header_64));
	printf("OffSetof test           : %lx\n", offsetof(t_elf_header_64, shoff));
	printf("Shoff Manualy           : %ld\n", *(long*)(ptr + 0x28) );

}

void nm(char *ptr)
{
	int magic_number;

	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
	{
		printf("Elf architecture\n");
		print_elf_header(ptr);
	}
	else
	{
		printf("4 premiers char: %X %X %X", *(char*)ptr, *(char*)ptr,  *(char*)ptr);
		printf("Pas elf , mon magic : %x, elf magic :%x\n", magic_number, (0x7f << 0) + (0x45 << 8) + (0x4c << 16) + (0x46 << 24)  );
	}
}

int main(int argc, char **argv)
{
	int		fd;
	char	*ptr;
	struct stat buf;

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
		return(EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		perror("munmap");
		return(EXIT_FAILURE);
	}
	return(EXIT_SUCCESS);
}