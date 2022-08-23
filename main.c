#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "../classical-cryptography/source/headers/otp.h"

#define COPY_TO_A_CLIPBOARD "\
#/bin/bash \n\
xclip -sel c < temp.passt \n\
"

#define RUN_KEEPASSXC "\
#/bin/bash \n\
sudo keepassxc \n\
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
static void helpInfo();

int main( int argc, char** argv )
{
    
    /*
     
    printf( "argv = %s\n", argv[1] );
    if( strcmp( argv[1], "-get" ) == 0)
        printf( "wlad" );
    return 0;
     
    */
    
    if( argc > 3 )
        { printf( "\n( ! ) [ THE NUMBER OF ARGUMENTS SHOULD BE EQUAL TO 1 ]\n\n" ); return 1; }
    
    if( argc == 1 )
        { helpInfo(); return 0; }
    
	FILE* f = fopen( PASS_FILE, "r" );
	FILE* b = fopen( KEY_FILE, "r" );
	if( isEmpty( f ) == 0 && isEmpty( b ) == 0 && ( strcmp( argv[1], "-init" ) == 0 ))
	{
        //printf( "You here!\n" );
		fclose( f );
		fclose( b );

		printf( "\n[ SET UP THE PASSWORD ]\n\n[password]: " );
		
        size_t i = 0;
        system( "stty -echo" );
        for( ; i < SIZE; i++ )
        {
            if( i == 0 )
            { scanf( " %c", &buffer[ i ] ); printf( "*" ) ;continue; } 
            scanf( "%c", &buffer[ i ] );
            printf( "*" );
        }
        printf( "\n" );
        system( "stty echo" );
        
        //scanf( "%s", buffer );
        
		genKey();
        printf( "( * ) [ THE PASSWORD HAS SUCCESFULLY BEEN SET ]\n" );
	}
// 	//else printf( "\n[ THE PASSWORD ALREADY EXISTS ]\n" );            !!!     !!!

	setBuffer();
	setKey();

	//printf( "What are you suppose to do?\n" );
	//printf( "1. Get password ot the clipborad\n2. Update the keys\n3. Set new password\n" );

	//scanf( "%d", &in );

	/*switch( in )
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
	}*/
    
    if( strcmp( argv[1], "-get" ) == 0)
        { getPassword(); system( RUN_KEEPASSXC ); }
    else if( strcmp( argv[1], "-u" ) == 0)
        updKey();
    else if( strcmp( argv[1], "-new" ) == 0)
        updPassword();
    else if( isEmpty( f ) != 0 && isEmpty( b ) != 0 && strcmp( argv[1], "-init" ) == 0 )
        { printf( "\n( ! ) [ THE PASSWORD ALREADY EXISTS ]\n\n" ); }
    else printf( "\n( ! ) [UNKNOWN ARGUMENT = ( %s )]\n\n", argv[1] );

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

	fprintf( f, "%s", p_password );

	fclose( f );

	//printf( "\n3 [ THE PASSWORD HAS SUCCESFULLY BEEN SET ]\n\n" );
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

	printf( "\n( * ) [ THE KEY HAS SUCCESFULLY BEEN GENERATED ]\n\" );
}

static void getPassword()
{
	FILE* f = fopen( PAST_FILE, "w" );

	char p_buffer[ SIZE ];

	otp( -1, buffer, key, SIZE, p_buffer );
	fprintf( f, "%s", p_buffer);

	fclose( f );

	system( COPY_TO_A_CLIPBOARD );

	if( remove( PAST_FILE ) == 0) // ==0 removed
		printf( "\n( * ) [ THE PASSWORD HAS SUCCESFULLY BEEN COPIED TO THE CLIPBOARD ]\n\n" );
	else printf( "\n( ! ) [ AN ERROR OCCURED ]\n\n" );
}

static void updKey()
{
	otp( -1, buffer, key, SIZE, buffer );
    //printf( "2 " );
	genKey();
}

static void updPassword()
{
	int i = 0;
    
    printf( "\n[ SET UP THE NEW PASSWORD ]\n\n[password*]: " );
	
    system( "stty -echo" );
	for( ; i < SIZE; i++ )
	{
		if( i == 0 )
		{ scanf( " %c", &buffer[ i ] ); printf( "*" ) ;continue; } 
		scanf( "%c", &buffer[ i ] );
		printf( "*" );
	}
	printf( "\n" );
	system( "stty echo" );

	genKey();
    printf( "( * ) [ THE PASSWORD HAS SUCCESFULLY BEEN SET ]\n\n" );
}

static void helpInfo()
{
    printf( "\npasst — is a simple utility for KeePassXC purposed on keeping the password of KeePass database in safety.\n\n\nOptions:\n\n\t-init      Set up a password\n\t-get       Copy password to the clipboard\n\t-u         Update the key\n\t-new       Set up a new password\n\nwladislawmakowski/passt is licensed under the GNU General Public License v3.0\n" );
}
