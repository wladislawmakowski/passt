#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "../classical-cryptography/source/headers/otp.h"

#define COPY_TO_A_CLIPBOARD "\
#/bin/bash \n\
xclip -sel c < temp.passt \n\
"

#define PASS_FILE "password.passt"
#define PAST_FILE "temp.passt"
#define KEY_FILE "key.passt"
#define SIZE 2048 

static char buffer[ SIZE ];
static int key[ SIZE ];

static void setBuffer();
static void setKey();
static int isEmpty( FILE* f );
static void setPassword();
static void genKey();
static void getPassword();
static void updKey();
static void updPassword();

int main( int argc, char** argv )
{
	int in;

	FILE* f = fopen( PASS_FILE, "r" );
	FILE* b = fopen( KEY_FILE, "r" );
	if( isEmpty( f ) == 0 && isEmpty( b ) == 0)
	{
		fclose( f );
		fclose( b );

		printf( "Please, make up a key\n" );
		scanf( "%s", buffer );
		genKey();
	}

	setBuffer();
	setKey();

	printf( "What are you suppose to do?\n" );
	printf( "1. Get password ot the clipborad\n2. Update the keys\n3. Set new password\n" );

	scanf( "%d", &in );

	switch( in )
	{
		case 1:
			getPassword();
			break;
		case 2:
			updKey();
			break;
		case 3:
			updPassword();
			break;
	}

	return 0;
}

static void setBuffer()
{
	FILE* f = fopen( PASS_FILE, "r" );

	int i;
	for( i = 0; i < SIZE; i++ )
	{
		fscanf( f, "%c", &buffer[ i ] );
	}

	fclose( f );
}

static void setKey()
{
	FILE* f = fopen( KEY_FILE, "r" );

	int i;
	for( i = 0; i < SIZE; i++ )
	{
		fscanf( f, "%d.", &key[ i ]);
	}

	fclose( f );
}

static int isEmpty( FILE* f )
{
	fseek( f, 0, SEEK_END );

	if( ftell( f ) == 0 )
		return 0;
	else return 1;
}

static void setPassword()
{
	FILE* f = fopen( PASS_FILE, "w" );

	char p_password[ SIZE ];
	otp( 1, buffer, key, SIZE, p_password );

	/* int i = 0;
	for( ; i < SIZE; i++ )
	{
		fprintf( f, "%c", p_password[ i ] );
	}*/

	fprintf( f, "%s", p_password );

	fclose( f );

	printf( "*** The password has successfully been set ***\n" );
}

static void genKey()
{
	srand( time( NULL ) );
	size_t index = 0;
	size_t index_s = 0;
	int* p_key;

	FILE* f = fopen( KEY_FILE, "w" );

	for( ; index < SIZE; ++index )
		key[ index ] = rand() % 89;

	p_key = key;
	for( ; index_s < SIZE; ++p_key, ++index_s )
		fprintf( f, "%d.", *p_key );

	fclose( f );
	setPassword();

	printf( "*** A key has successfully been updated ***\n" );
}

static void getPassword()
{
	FILE* f = fopen( PAST_FILE, "w" );

	char p_buffer[ SIZE ];

	otp( -1, buffer, key, SIZE, p_buffer );
	fprintf( f, "%s", p_buffer);

	fclose( f );

	system( COPY_TO_A_CLIPBOARD );

	if( remove( PAST_FILE ) == 0 )
		printf( "*** Password has successfully been copied to the clipboard ***\n" );
	else printf( "AN ERROR OCCURED\n" );
}

static void updKey()
{
	otp( -1, buffer, key, SIZE, buffer );
	genKey();
}

static void updPassword()
{
	int i = 0;

	system( "stty -echo" );
	for( ; i < SIZE; i++ )
	{
		if( i == 0 )
		{ scanf( " %c", &buffer[ i ] ); printf( "*" ) ;continue; } // i+=1 added
		scanf( "%c", &buffer[ i ] );
		printf( "*" );
	}
	printf( "\n" );
	system( "stty echo" );

	genKey();
}
