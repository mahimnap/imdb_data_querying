#include "util.h" 
#include "hashfn.h" 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRLEN 256

int main (int argc, char *argv[]){
	FILE *baconCode, *actorCode, *titleCode, *getTitle; 
	int capacity; 
	int capacity2;
	int capacity3; 
	char baconNameCode[STRLEN];
	char actorNameCode[STRLEN];
	char nameValMain[STRLEN]; 
	char nameCodeMain[STRLEN]; 
	char keyHolder[STRLEN]; 
	char finalPrint[STRLEN]; 
	char nameValSecond[STRLEN]; 
	char nameCodeSecond[STRLEN];
	char commonKey[STRLEN]; 
	char baconName[] = "Kevin Bacon";  
	char baconKey[STRLEN]; 
	char actorKey[STRLEN];
	char actorName[STRLEN]; 

	if (argc != 2){
		fprintf (stderr, "Usage: %s 'search term'\n", argv[0]); 
		exit (1); 
	}
	
	strcpy (actorName, argv[1]);  
	baconCode = fopen ("name.basics.kv", "r"); 
	capacity = get_capacity (baconCode); 
	fclose (baconCode); 
	
	baconCode = fopen ("name.basics.kv", "r"); 
	for (int i = 0; i < capacity; i++){ //get name code for the Kevin Bacon 
		read_val (baconCode, i, baconNameCode); 
		if (strcmp (baconName, baconNameCode) == 0) {
			read_key (baconCode, i, baconKey); //bacon key 
		}
	}
	fclose (baconCode); 
	
	
	actorCode = fopen ("name.basics.kv", "r");  
	for (int i = 0; i < capacity; i++){ //get name code for passed in actor
		read_val (actorCode, i, actorNameCode); 
		
		if (strcmp (actorName, actorNameCode) == 0){
			read_key (actorCode, i, actorKey); 
		}
	}
	fclose(actorCode); 
	
	titleCode = fopen ("title.principals.kv", "r"); 
	capacity2 = get_capacity (titleCode); 
	fclose(titleCode); 
	
	titleCode = fopen ("title.principals.kv", "r"); 
	for (int i = 0; i < capacity2; i++){ //get first shared title between Bacon and passed in actor 
		read_val (titleCode, i, nameValMain);
		if (strcmp (baconKey, nameValMain) == 0){ //if the current name code matches bacon name code 
			read_key (titleCode, i, nameCodeMain); //read the key (title code) at this movie
			for (int j = 0; j < capacity2; j++){ //loop again to test actor name code 
				read_val (titleCode, j, nameValSecond); 
				if (strcmp (actorKey, nameValSecond) == 0){ //if actor name code matches current name code
					read_key (titleCode, j, nameCodeSecond); //get match title code 
					if (strcmp (nameCodeMain, nameCodeSecond) == 0){ //if title code for bacon name code and actor name code match are the same
						strcpy (commonKey, nameCodeMain); //copy the common key
						goto done; //exit the nested loop
					}
				}
			}
		}
	}
	done: 
	fclose (titleCode); 
	
	getTitle = fopen ("title.basics.kv", "r");
	capacity3 = get_capacity (getTitle); 
	fclose (getTitle); 
	
	getTitle = fopen ("title.basics.kv", "r"); 
	for (int i = 0; i < capacity3; i++){ //print the value at the shared common title code
		read_key (getTitle, i, keyHolder); 
		if (strcmp (keyHolder, commonKey) == 0){
			read_val (getTitle, i, finalPrint); 
			printf ("%s\n", finalPrint); 
			break; 
		}
	}
	fclose (getTitle); 
	
	return 0; 
}
