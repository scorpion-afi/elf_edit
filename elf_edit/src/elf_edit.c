/*
 ============================================================================
 Name        : elf_edit.c
 Author      : imdhh
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <elf.h>

int main( void )
{
	int fd = -1;
	char* p_elf_content = NULL;
	off_t size = 0;
	off_t temp = 0;
	ssize_t num_of_read_byte = 0;

	Elf64_Ehdr* p_elf_header = NULL;
	Elf64_Shdr* p_sec_header_table_entry = NULL;
	Elf64_Sym*  p_sym_table_entry = NULL;

	fd = open( "/home/afi/workspace_elf_injection/simple/Debug/simple", O_RDWR );
	if( fd == -1 )
	{
		perror( NULL );
		return EXIT_FAILURE;
	}
	printf( "fd = %d\n", fd );

	// obtain size of elf file
	size = lseek( fd, 0, SEEK_END );
	if( size <= 0 )
	{
		printf( "error while lseek call.\n" );
		return EXIT_FAILURE;
	}

	// return file pointer back
	temp = lseek( fd, 0, SEEK_SET );
	if( temp != 0 )
	{
		printf( "error while lseek call.\n" );
		return EXIT_FAILURE;
	}
	printf( "size = %ld\n", size );

	// allocate memory in RAM for elf file
	p_elf_content = calloc( 1, size );
	if( !p_elf_content )
	{
		printf( "error while calloc call.\n" );
		return EXIT_FAILURE;
	}

	num_of_read_byte = read( fd, p_elf_content, size );
	if( num_of_read_byte == -1 )
	{
		perror( NULL );
		return EXIT_FAILURE;
	}

	//---------------------------------------------------------------------------------

	p_elf_header = (Elf64_Ehdr*)p_elf_content;
	if(
		( p_elf_header->e_ident[0] != ELFMAG0 ) || ( p_elf_header->e_ident[1] != ELFMAG1 ) ||
		( p_elf_header->e_ident[2] != ELFMAG2 ) || ( p_elf_header->e_ident[3] != ELFMAG3 )
	  )
	{
		printf( "it's not an elf file. (signature of elf file: 0x7f 0x45 0x4c 0x46)\n" );
		return EXIT_FAILURE;
	}

	printf( "%c%c%c\n", p_elf_header->e_ident[1], p_elf_header->e_ident[2], p_elf_header->e_ident[3] );

	// obtain pointer to first(0) section header table entry (to first(0) section header)
	p_sec_header_table_entry = (Elf64_Shdr*)( p_elf_content + p_elf_header->e_shoff );

	// we want to get information from .symtab section, so now p_sec_header_table_entry points to
	//.symtab section header
	p_sec_header_table_entry += 34;

	// obtain pointer to section .symtab (to first(0) symtab entry)
	p_sym_table_entry = (Elf64_Sym*)( p_elf_content + p_sec_header_table_entry->sh_offset );

	//----------------

	// obtain pointer to first(0) section header table entry (to first(0) section header)
	p_sec_header_table_entry = (Elf64_Shdr*)( p_elf_content + p_elf_header->e_shoff );
	p_sec_header_table_entry += 33;
	printf( "kk: %s\n", (char*)( p_elf_content + p_sec_header_table_entry->sh_offset ) );


	Elf64_Off offset =  p_elf_header->e_shstrndx * p_elf_header->e_shentsize + p_elf_header->e_shoff;

	/*
	for(;;)
	{
		if(p_sym_table_entry->st_name)
	}*/

	return EXIT_SUCCESS;
}
