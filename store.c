//Store used to store passwords and usernames in a databse
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct element{
	char *domain_name;
	char *username;
	char *password;
}; 

struct element elements[10];

char * get_user_input( char *str  );
void assign_fields( char *file_name, char *domaine_name, char *username, char *password  );
void store_in_database(); 
char * get_database_name();

void store_in_database(){
	//first establish datbase connection
	sqlite3 *db;
	sqlite3_stmt *res;
	char *err_msg = 0;
	char *database_name;

	database_name = get_user_input( "database" );
	int rc = sqlite3_open( database_name, &db );
	
	if ( rc != SQLITE_OK ){
		fprintf( stderr, "Cannot open database: %s\n", sqlite3_errmsg(db) );
		sqlite3_close( db );

		exit(0);
	}

	char *zSQL = malloc(100);

	zSQL = sqlite3_mprintf( "CREATE TABLE IF NOT EXISTS elements(Domain TEXT, Username TEXT, Password TEXT);" );
	sqlite3_exec(db, zSQL, 0, 0, 0);
	sqlite3_free(zSQL);

	int i;
	int size_of_elements = sizeof( elements ) / sizeof( struct element );
	for( i = 0; i < size_of_elements; i++ ){
		if( elements[i].domain_name ){
			zSQL = sqlite3_mprintf( "INSERT INTO elements VALUES(%Q,%Q,%Q);", elements[i].domain_name, elements[i].username, elements[i].password );
			sqlite3_exec(db, zSQL, 0, 0, 0);
			sqlite3_free(zSQL);
		}	
	}
	
	sqlite3_close(db);
}

char * get_user_input( char *str )
{
	char *return_str = malloc(20);
	
	if( str == "database" ){
		printf( "enter the full path and name of the database that you wish to store your data in " );	
	}else{	
		printf( "Enter substring that exists in the line containing the %s \n", str );
	}
	fgets( return_str, 20, stdin);
	int i = strlen(return_str)-1;
  	if ((i > 0) && (return_str[i] == '\n'))
    	return_str[i] = '\0';

	return return_str;
}

void assign_fields( char *file_name, char *domain_substr, char *username_substr, char *password_substr )
{
	/*get the hostname, username and password as input 
	variable open the file asked and store data in the 
	database*/

	//variables	
	char ch; 	
	FILE *fp;
	char *line = malloc(100);
	size_t len = 0;
	char *host_substr;	
	
	fp = fopen( file_name, "r" );
	
	if ( !fp ){
		perror( "Error while opening the file\n usage:\n \t ./store [file-path/file-name]" );
		exit( EXIT_FAILURE );
	}

	//display the content of the file_name
	int i = 0;
	while( ( ch = getline( &line, &len, fp ) ) != -1 ){
		if( strstr( line, domain_substr ) ){
			//remove the domain_substr from the line 
			//store the domain name in the domain_name field for this specific element
			//and print the result
			memmove( line, line + strlen( domain_substr ), 1 + strlen( line + strlen( domain_substr ) ) );
			elements[i].domain_name = strdup(line);			
			printf( "domain name for elements[%d] is %s\n", i, elements[i].domain_name );
		}
		else if( strstr( line, username_substr ) ){
			memmove( line, line + strlen( username_substr ), 1 + strlen( line + strlen( username_substr ) ) );
			elements[i].username = strdup(line);	
			printf( "username name for elements[%d] is %s\n", i, elements[i].username );
		}
		else if( strstr( line, password_substr ) ){
			memmove( line, line + strlen( password_substr ), 1 + strlen( line + strlen( password_substr ) ) );
			elements[i].password = strdup(line);
			printf( "password name for elements[%d] is %s\n", i, elements[i].password );
			i++;
		}	
	}
	fclose( fp );
}

int main( int args, char *argv[] )
{	
	//variables
	char *file_name;
	char *domain_substr;
	char *username_substr;
	char *password_substr;
	
	file_name = argv[1];
	
	//check if file exists
	if( access( file_name, F_OK ) == -1 ){
		perror( "invalid file " );
		exit(0);
	}

	domain_substr 	= get_user_input( "domain" );
	username_substr	= get_user_input( "username" );
	password_substr = get_user_input( "passwords" );

	assign_fields( file_name, domain_substr, username_substr, password_substr );
	store_in_database();
	return 0;
}
