#include "util.h" 
#include "hashfn.h" 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRLEN 256

int main (int argc, char* argv[]){
	int hash_val; 
	int hash_val_start; 
	int capacity; 
	char ret_key [STRLEN]; 
	char ret_val [STRLEN]; 
	int hits = 0;
	char holder[STRLEN];
	int match = 0; 
	int ret_index; 
	FILE *fp, *fp2; 
	char baseFileName[100];
	
	if (argc != 3){
		fprintf (stderr, "Usage: %s filename.kv 'search term'\n", argv[0]); 
		exit(1); 
	}
	
	char fileName[100];
	for (int i = 0; i < (strlen(argv[1]) - 1); i++){
		if ((argv[1][i] == '.') && (argv[1][i+1] == 'k') 
			&& (argv[1][i+2] == 'v')){
			strncpy (fileName, argv[1], i); 
		}
	}
	strcpy (baseFileName, fileName);
	strncat (baseFileName, ".khs\0", 5);
	
	fp = fopen (baseFileName, "r"); //getting capacity for hash file as determined by user input with buildidx 
	capacity = get_capacity (fp); 
	fclose (fp); 
	
	fp = fopen (baseFileName, "r"); //opening khs file to read index stored at hashed key value
	fp2 = fopen (argv[1], "r"); //opening kv file to read index after retrieving it from the stored location in the khs file (hashed index location)
	hash_val = hashfn (argv[2], capacity); //initial hash for starting point
	hash_val_start = hash_val; //setting a variable to keep track of initial hasl val to know when to stop searching 
	while (hash_val != hash_val_start || match < 1){ //hash_val does equal hash_val_start for first iteration but match will be less than 1 (0) 
		read_index(fp, hash_val, &ret_index); //read the index stored at the hash value index 
		if (ret_index == -1){ //if the index stored is empty, can't compare, so increment hash index and continue 
			hash_val++; 
		}else{ //if not empty, compare search key with key extracted from kv file 
			read_key (fp2, ret_index, ret_key); //use the index stored at the hash value to retrieve the key in the kv file for comparison
			if (strcmp (ret_key, argv[2]) == 0){ //if the key in the kv file matches the searched key then read the value corresponding to that key and print it  
				read_val (fp2, ret_index, ret_val);
				if (strcmp (holder, ret_val) != 0){ //avoids printing duplicates 
					printf ("%s\n", ret_val);
				}
				strcpy (holder,ret_val); 
				hits++; 
			}
			hash_val++; //always continue as there can be multiple values with the same key (e.g. one title has multiple actors) 
			//if this increment brings it back to the initial hash val (hash_val_start) then this loop will end and the search will be complete 
		}
		if (hash_val == capacity){ //if the hash reaches capacity after previous increment, start again from the beggining 
			hash_val = 0; 
		}
		match++; //to turn this into do while loop functionality -> laziness! fix later 
	}
	if (hits == 0){
		printf ("NOT FOUND!\n"); 
	}
	fclose (fp); 
	fclose (fp2); 
	
	return 0; 
}
