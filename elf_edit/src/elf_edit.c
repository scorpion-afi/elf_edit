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
	ssize_t num_of_wrote_bytes = 0;

	Elf64_Ehdr* p_elf_header = NULL;
	Elf64_Shdr* p_sec_header_table_entry = NULL;
	Elf64_Sym*  p_sym_table_entry = NULL;
	Elf64_Off func_1_offset = 0;

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

	//--------------------------- .symtab string ----------------------------------------------

	// obtain pointer to first(0) section header table entry (to first(0) section header)
	p_sec_header_table_entry = (Elf64_Shdr*)( p_elf_content + p_elf_header->e_shoff );

	//shift to .symtab section
	p_sec_header_table_entry += 35;

	// get pointer to first string in .symtab section
	char* tempik = (char*)( p_elf_content + p_sec_header_table_entry->sh_offset ) + 1;

	printf( "kk: %s\n", tempik );

	//---------------------------------------------------------------------------------

	// obtain pointer to first(0) section header table entry (to first(0) section header)
	p_sec_header_table_entry = (Elf64_Shdr*)( p_elf_content + p_elf_header->e_shoff );

	// we want to get information from .symtab section, so now p_sec_header_table_entry points to
	//.symtab section header
	p_sec_header_table_entry += 34;

	// obtain pointer to section .symtab (to first(0) symtab entry)
	p_sym_table_entry = (Elf64_Sym*)( p_elf_content + p_sec_header_table_entry->sh_offset );
	p_sym_table_entry += 44; // jump to func_1

	func_1_offset = (Elf64_Off)( p_sym_table_entry->st_value - 0x400000 );

	//---------------------------------------------------------------------------------

	// obtain pointer to first(0) section header table entry (to first(0) section header)
	p_sec_header_table_entry = (Elf64_Shdr*)( p_elf_content + p_elf_header->e_shoff );

	//shift to .text section
	p_sec_header_table_entry += 13;

	// get pointer to first byte of executable code in .text section
	// this is _start function code (from gcc or glibc.so)
	tempik = (char*)( p_elf_content + p_sec_header_table_entry->sh_offset );

	// this is func_1 function code
	tempik = p_elf_content + func_1_offset;

	// change a++ to a += 2;
	tempik += 10;

	*tempik = 2;

	//---------------------------------------------------------------------------------

	close( fd );
	remove( "/home/afi/workspace_elf_injection/simple/Debug/simple" );

	fd = open( "/home/afi/workspace_elf_injection/simple/Debug/simple", O_CREAT | O_RDWR );
	if( fd == -1 )
	{
		perror( NULL );
		return EXIT_FAILURE;
	}

	printf( "fd = %d\n", fd );

	num_of_wrote_bytes = write( fd, p_elf_content, size );
	if( num_of_wrote_bytes != size )
	{
		perror( NULL );
		return EXIT_FAILURE;
	}

	close( fd );

	// 0x602010 - p_elf_content
	// 0x6025a2 - func_1
	// 0x6025b3 - func_2

	return EXIT_SUCCESS;
}
