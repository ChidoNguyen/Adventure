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


/*
 *Room structures to handle data related to our rooms
 *rm_name has size 9 because MAX 8 characters + null term 
 *cnx_cnt = connection count needs to stay below 6
 */
struct ROOMS{
	char* rm_name;
	int cnx_cnt;
};


/*
Function Name:	swap
Params:			two items ( int ) that needs to be moved around
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
	srand( time( NULL));		//generates random seed
	int j, rnd_indx;
	//Decrease size by 1 since we use 0 as first index
	//Use rand and grab modulus to represent an index value
	//Swap that random index with j value 
	for( j = i -1; j > 0; j-- ){
		rnd_indx = rand() % (j + 1);
		swap(arr, j, rnd_indx);
	}
}






int main(){
	//Names of all room outputs possible; 10 total//
	char NAMES[10][9]= { "WEIGHTS", "COURT", "SPA", "SAUNA", "LOCKERS", "POOL", "STORE", "CLOSET", "FOODS", "GAINZ"};
	
	int PID = getpid();
	//making our directory nguychid.rooms.PID//
	//https://stackoverflow.com/questions/33332533/create-directory-and-store-file-c-programming//
	/*char* src = "nguychid.rooms.";
	char dirNameCombo[20];						// stores our DIRECTORY NAME!!!
	sprintf(dirNameCombo,"%s%i", src, PID);		//combine username.rooms with PID
	mkdir(dirNameCombo, 0755);					//create the dir
	*/
	//Required to pick 7 of 10 rooms, use these numbers to represent
	// our NAMES array objects. Shuffle and  use the first 7
	int rnd_name_idx[10] = {0 ,1 ,2 ,3 ,4 ,5, 6, 7 , 8, 9};
	mix_shake(rnd_name_idx, 10);
	
	int x;
	for(x = 0; x < 10; x++){
		printf("%i\n", rnd_name_idx[x]);
	}
	return 0;
}
