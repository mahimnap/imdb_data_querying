#include "util.h" 
#include "hashfn.h" 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRLEN 256

int main (int argc, char *argv[]){
	FILE *titleCode, *nameCodes, *actualNames; 
	int capacity; 
	int capacity2; 
	int capacity3; 
	char ret_val[STRLEN]; 
	char ret_key[STRLEN]; 
	char nameCode[STRLEN]; 
	char actualName[STRLEN]; 
	char allNames[100][STRLEN]; 
	char namePrinter [STRLEN]; 
	int j; 
	
	if (argc != 2){
		fprintf (stderr, "Usage: %s 'search term'\n", argv[0]); 
		exit (1); 
	}
	
	titleCode = fopen ("title.basics.kv", "r"); 
	capacity = get_capacity (titleCode);
	fclose (titleCode); 
	
	nameCodes = fopen ("title.principals.kv", "r"); 
	capacity2 = get_capacity (nameCodes); 
	fclose (nameCodes); 
	
	titleCode = fopen ("title.basics.kv", "r");
	for (int i = 0; i < capacity; i++){ //gets the movie title code 
		read_val(titleCode, i, ret_val); 
		if (strcmp (ret_val, argv[1]) == 0){
			read_key(titleCode, i, ret_key); 
			break; //can only be one so break 
		}
	}
	fclose (titleCode); 
	j = 0; 
	
	nameCodes = fopen ("title.principals.kv", "r"); 
	for (int i = 0; i < capacity2; i++){ //gets all the names associated with the movie title code and puts them into array 
		read_key (nameCodes, i, nameCode); 
		if (strcmp (ret_key, nameCode) == 0){
			read_val (nameCodes, i, allNames[j]); 
			j++; 
		}
	}
	fclose (nameCodes); 
	
	actualNames = fopen ("name.basics.kv", "r"); 
	capacity3 = get_capacity(actualNames);
	fclose(actualNames);
	
	actualNames = fopen ("name.basics.kv", "r"); 
	for (int i = 0; i < capacity3; i++){ //prints out the name values based off codes stored in the array 
		read_key (actualNames, i, actualName); 
		for (int p = 0; p < j; p++){
			if (strcmp (actualName, allNames[p]) == 0){
				read_val (actualNames, i, namePrinter); 
				printf ("%s\n", namePrinter); 
			}
		}
	}
	
	fclose (actualNames); 
	return 0; 
}
