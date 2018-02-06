/*Chido Nguyen 931506965
*Program 2: Adventures
*Rooms: This file will house hard coded room names. It will also handle various rooms related functions. Generation of random room names.
*Links rooms appropriately, functions will be specified prior to function definition.
*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h> 	//required for mkdir
#include <stdlib.h>		//this + time.h used for srand(time(null))
#include <time.h>
#include <fcntl.h>

//GLOBAL VARIABLES//
//Names of all room outputs possible; 10 total//
//Minimum Connection Count//
//Max Connection Count//
//Max Room numbers//
char NAMES[10][9]= { "WEIGHTS", "COURT", "SPA", "SAUNA", "LOCKERS", "POOL", "STORE", "CLOSET", "FOODS", "GAINZ"};
int MIN_CON = 3;
int MAX_CON = 6;
int MAX_RM = 7;

//Learned raw C at 89 has no bool c99 uses stdbool.h//
// https://stackoverflow.com/questions/1921539/using-boolean-values-in-c //
typedef enum {false, true } bool;

/*
 *Room structures to handle data related to our rooms
 *rm_name has size 9 because MAX 8 characters + null term 
 *cnx_cnt = connection count needs to stay below 6
 *roomLinks documents which room IDs are connected to it 
 room_type; 0 start , 1 mid , 2 end
 */
struct ROOMS{
	char* rm_name;
	int rm_ID;
	int cnx_cnt;
	int roomLinks[7];
	int room_type;
};


/*
Function Name:	swap
Params:			two items ( int ) that needs to be moved around, used for shuffling 
Output:			a = b, and b = a now
*/
void swap(int* arr, int a, int b){
	int tmp;
	tmp = arr[a];
	arr[a]= arr[b];
	arr[b] = tmp;
}
/*
Function Name: 	mix_shake
Parameters: 	reference to array and array size
Output:			shuffles the content of an array
https://www.geeksforgeeks.org/shuffle-a-given-array/
*/
void mix_shake(int* arr, int i){
	int j, rnd_indx;
	//Decrease size by 1 since we use 0 as first index
	//Use rand and grab modulus to represent an index value
	//Swap that random index with j value 
	for( j = i -1; j > 0; j-- ){
		rnd_indx = rand() % (j + 1);
		swap(arr, j, rnd_indx);
	}
}

/*
Function: 		gamble
Param:			Integer representing  sample size
Output:			random int
*/
int gamble(int n){
	return rand() % n;
}

/*
Function:		NoVacancy 
Parameters:		our struct array of room layouts
Output:			Bool value of whether all rooms has AT LEAST 3 connections
				RETURNS 1 or TRUE if graph is Full aka no vacancy
				Returns 0 or false if graph is still empty
*/
bool NoVacancy(struct ROOMS* arr){
	int x;
	for(x = 0; x < MAX_RM; x++){
		if( arr[x].cnx_cnt < 3 )
			return 0;
	}
	return 1;
}

/*
Function:		CanConnect
Parameters:		struct pointer to our layout + room index
Output:			True if room connection is not 6
				False if room connection is already 6
*/
bool CanConnect(struct ROOMS* arr, int x){
	if(arr[x].cnx_cnt >=6 )
		return false;
	else
		return true;
}

/*
Function:		ConnectRM
Parameters:		Struct array of layout + 2 room indexes
Output:			Nothing explicit , add both rooms to their connections
*/
void ConnectRM(struct ROOMS* arr, int a, int b){
	int a_idx, b_idx; // indexes for roomLinks array in struct
	a_idx = arr[a].cnx_cnt;
	b_idx = arr[b].cnx_cnt;
	arr[a].roomLinks[a_idx] = arr[b].rm_ID;
	arr[b].roomLinks[b_idx] = arr[a].rm_ID;
	arr[a].cnx_cnt++;
	arr[b].cnx_cnt++;
}
/*
Function:		ConnectionExist
Params:			our array of struct Layout , and room index of 1 and 2
Output:			True if room 1 is connected to room 2 already otherwise false
*/
bool ConnectionExist(struct ROOMS* arr, int a, int b){
	int x;
	for(x = 0; x < arr[a].cnx_cnt ; x++){
		if( arr[a].roomLinks[x] == arr[b].rm_ID )
			return true;
	}
	return false;
}
/*
Function:		AddRandCon
Parameters:		our room Layout
Output:			No direct console output or returns BUT
				Will provide a connection between rooms A and B tracked via their structs.
				
*/
void AddRandCon(struct ROOMS* arr){
	int roomA;
	int roomB;
	
	while(1){
		roomA = gamble(MAX_RM); // grab random room index //
		if(CanConnect(arr,roomA))
			break;

	}
	do{
		roomB= gamble(MAX_RM);
	}
	while(CanConnect(arr, roomB) ==  false || roomB == roomA ||  ConnectionExist(arr,roomA,roomB) == true);
	ConnectRM(arr,roomA,roomB);


	
};

int main(){
	int ST_RM, E_RM; // will hold index for what will be start room and end room
	int PID = getpid();
	srand( time( NULL)); // generates seed
	//making our directory nguychid.rooms.PID//
	//https://stackoverflow.com/questions/33332533/create-directory-and-store-file-c-programming//
	char* src = "nguychid.rooms.";
	char dirNameCombo[40];						// stores our DIRECTORY NAME!!!
	sprintf(dirNameCombo,"%s%i", src, PID);		//combine username.rooms with PID
	mkdir(dirNameCombo, 0755);					//create the dir
	
	
	
	//Required to pick 7 of 10 rooms, use these numbers to represent our NAMES
	int rnd_name_idx[10] = {0 ,1 ,2 ,3 ,4 ,5, 6, 7 , 8, 9};
	//Shuffle and  use the first 7 as random name assignment
	mix_shake(rnd_name_idx, 10);
	
	
	
	//Set start room index and run a do while loop to get End room
	//we loop to make sure they don't collide
	ST_RM = gamble(7);
	do{
		E_RM = gamble(7);
	}while (E_RM == ST_RM);
	
	
	/*prints arrays as needed 
	int x;
	for(x = 0; x < 10; x++){
		printf("%i\n", rnd_name_idx[x]);
	}*/
	
	
	//allocate an array of layouts for 7 rooms 
	// loop through our array and set our room names 
	struct ROOMS* layout;
	layout = malloc(sizeof(struct ROOMS) * 7);
	int x,j;
	for (x = 0; x < 7 ; x++){
		layout[x].rm_name = NAMES[rnd_name_idx[x]];
		layout[x].rm_ID = rnd_name_idx[x];
		layout[x].cnx_cnt = 0; // initialize connection counts
		layout[x].room_type = 1;
		//printf("%s\n", layout[x].rm_name); //printed out name as tracer check
	}
	layout[ST_RM].room_type = 0;
	layout[E_RM].room_type = 2;
	//load up the rooms//
	while(!NoVacancy(layout)){
		AddRandCon(layout);
	}
	// WRITE TO FILES //
	FILE* file;
	char dirName_File[40];
	//Loop all rooms//
	for(x=0; x < MAX_RM ; x++){
		//combine director + file name //
		sprintf(dirName_File, "./%s/%s", dirNameCombo, layout[x].rm_name);
		printf("%s\n", dirName_File);
		file = fopen(dirName_File, "w" );
		fprintf(file,"ROOM NAME: %s\n", layout[x].rm_name);
		for(j = 0; j < layout[x].cnx_cnt; j++){
			fprintf( file, "CONNECTION %i: %s\n", (j+1), NAMES[layout[x].roomLinks[j]]);
		}
		if(layout[x].room_type == 0)
			fprintf(file, "ROOM TYPE: %s\n", "START_ROOM");
		else if( layout[x].room_type == 1)
			fprintf(file, "ROOM TYPE: %s\n", "MID_ROOM");
		else
			fprintf(file, "ROOM TYPE: %s\n", "END_ROOM");
		fclose(file);
	}
	
	
	
	free(layout);
	

	return 0;
}
