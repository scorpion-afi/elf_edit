/*
 * func.c
 *
 *  Created on: Sep 14, 2014
 *      Author: imdhh
 */
#include "func.h"

static int func_1( int a )
{
	return ++a;
}

static int func_2( int a )
{
	int temp = 0;

	temp = func_1( a );
	return ( temp + 11 );
}

