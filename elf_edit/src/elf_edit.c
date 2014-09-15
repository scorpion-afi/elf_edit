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

int main( void )
{
	int fd = -1;
	char* elf_content = NULL;
	off_t size = 0;
	off_t temp = 0;
	ssize_t num_of_read_byte = 0;

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
	elf_content = calloc( 1, size );
	if( !elf_content )
	{
		printf( "error while calloc call.\n" );
		return EXIT_FAILURE;
	}

	num_of_read_byte = read( fd, elf_content, size );
	if( num_of_read_byte == -1 )
	{
		perror( NULL );
		return EXIT_FAILURE;
	}

	// for debug purpose
	printf( "%c%c%c\n", elf_content[1], elf_content[2], elf_content[3] );

	//---------------------------------------------------------------------------------

	char* ptr = strstr( elf_content, "func_1" );

	if( ptr )
	  printf( "%s\n", ptr );

	return EXIT_SUCCESS;
}
