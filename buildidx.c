#include "util.h" 
#include "hashfn.h" 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRLEN 256

struct HashTableKey
{ 
	char **data; 
};

struct HashTableVal
{
	char **data; 
};

int main (int argc, char *argv[]) {
	/*this function will take a kv file (title (title code -> title string), name (name code -> name string), principles (title code -> name code)) 
	 * create an empty vhs hash table (indexes for values in the provided kv file) 
	 * create an empty khs hash table (indexes for keys in the provided kv file)
	 */
	char khs_name[100]; 
	char vhs_name[100]; 
	char key[STRLEN]; 
	char val[STRLEN]; 
	int i = 0; 
	int checker; 
	unsigned int vhs_hash; 
	unsigned int khs_hash; 
	int ret_idx; 
	long capacity;
	struct HashTableKey *myKeyTable = (struct HashTableKey *) malloc (sizeof (struct HashTableKey)); 
	struct HashTableVal *myValTable = (struct HashTableVal *) malloc (sizeof (struct HashTableVal)); 
	 
	if (argc != 3){
		fprintf (stderr, "Usage: %s filename.kv capacity\n", argv[0]); 
		exit(1); 
		return 0; 
	}
	
	char fileName[100];
	for (int i = 0; i < (strlen(argv[1]) - 1); i++){
		if ((argv[1][i] == '.') && (argv[1][i+1] == 'k') 
			&& (argv[1][i+2] == 'v')){
			strncpy (fileName, argv[1], i); 
		}
	}
	strcpy (khs_name, fileName);
	strncat (khs_name, ".khs\0", 5);
	strcpy (vhs_name, fileName);
	strncat (vhs_name, ".vhs\0", 5); 
	
	capacity = atol (argv[2]); //converting user entered capacity to a long int
	
	myKeyTable->data = (char **) malloc ((sizeof (char*)) * capacity); //each pointer will contain a pointer to a full string (key) 
	myValTable->data = (char **) malloc ((sizeof (char*)) * capacity); //each pointer will contain a pointer to a full string (value) 
	for (i = 0; i < capacity; i++){ //mallocing enough space for keys/vals for each index 
		myKeyTable->data[i] = (char*) malloc ((sizeof (char)) * STRLEN); 
		myValTable->data[i] = (char*) malloc ((sizeof (char)) * STRLEN);
	}
	
	FILE *fp, *fpk, *fpv; //fp used to write empty for all files then fp = kv, fpk = khs, fpv = vhs 
	fp = fopen (khs_name, "w"); //writing empty file for khs file (key hash)
	write_empty (fp, capacity);
	fclose (fp); 

	fp = fopen (vhs_name, "w"); //writing empty file for vhs file (value hash)
	write_empty (fp, capacity);
	fclose (fp); 
	
	fp = fopen (argv[1], "r"); //reading file into local data structures that map index to a key or val depending on structure
	//told not to worry about capacity as the correct size will be passed in by the user (enough size) 
	i = 0; 
	while (i < capacity){ //stores the indexes into local data structure 
		checker = read_keyval (fp, key, val); 
		if (checker != 2) {
			break; 
		}else{ //if checker == 2 (successfully read key/val) then store key in data structure and store val in data structure
			memcpy (myKeyTable->data[i], key, STRLEN); 
			memcpy (myValTable->data[i], val, STRLEN);
		}
		i++;
	}
	fclose (fp);
	
	fpk = fopen (khs_name, "r+"); //need to read to see if it's empty before writing so r+ needs to be used 
	for (i = 0; i < capacity; i++){ //loops through every value in local structure (ie every index needed to be stored) 
		khs_hash = hashfn (myKeyTable->data[i], capacity); //hash the keys in the local data structure
		read_index (fpk, khs_hash, &ret_idx); //get the index of the key hash file to see if the location is empty
		while (ret_idx != -1){ //if location is not empty 
			khs_hash++; //increment location index 
			if (khs_hash == capacity){ // if location index is at the end of the file 
				khs_hash = 0; //start again from the beginning 
			}
			read_index (fpk, khs_hash, &ret_idx); //read the index at the new location to see if it's empty 
			//not checking if it returns to old spot because apparently the capacity will always be enough
		}
		//printf ("Storing %s at %d containing index %d\n", myKeyTable->data[i], khs_hash, i); 
		write_index (fpk, i, khs_hash); //writing index (i) at khs_hash val (adjusted if needed above) 
	}
	fclose (fpk); 

	ret_idx = 0; 
	fpv = fopen (vhs_name, "r+"); 
	for (i = 0; i < capacity; i++){ //loops through every value in local structure 
		vhs_hash = hashfn (myValTable->data[i], capacity);
		read_index (fpv, vhs_hash, &ret_idx); 
		while (ret_idx != -1){ //if location is not empty (collision) 
			vhs_hash++; //increment location index 
			if (vhs_hash == capacity){ // if location index is at the end of the file 
				vhs_hash = 0; //start again from the beginning 
			}
			read_index (fpv, vhs_hash, &ret_idx); //read the index at the new location to see if it's empty 
			//not checking if it returns to old spot because apparently the capacity will always be enough
		}
		write_index (fpv, i, vhs_hash); //write the index of the value (determined by local structure) at the hash value 
	}
	fclose (fpv); 

	for (i = 0; i < capacity; i++){
		free (myKeyTable->data[i]); 
		free (myValTable->data[i]); 
	}
	free (myKeyTable->data); 
	free (myValTable->data); 
	free (myKeyTable); 
	free (myValTable); 
	
	return 0; 
}
