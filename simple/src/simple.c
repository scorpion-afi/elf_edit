/*
 ============================================================================
 Name        : simple.c
 Author      : imdhh
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "func.c"

int main( void )
{
	int ret = 0;

	ret = func_1( 11 );
	printf( "ret 1 = %d\n", ret );

	ret = func_2( 11 );
	printf( "ret 2 = %d\n", ret );

	return EXIT_SUCCESS;
}
