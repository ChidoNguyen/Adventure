/*Chido Nguyen 931506965
*Program 2: Adventures
*Rooms: This file will house hard coded room names. It will also handle various rooms related functions. Generation of random room names.
*Links rooms appropriately, functions will be specified prior to function defintion.
*/

#include <stdio.h>
#include <string.h>
/*
 *Room structures to handle data related to our rooms
 *rm_name has size 9 because MAX 8 characters + null term 
 *cnx_cnt = connection count needs to stay below 6
 */
struct ROOMS{
	char* rm_name;
	int cnx_cnt;
};

//Main Function to test various usage Remove or comment out later//
int main(){
	//Names of all room outputs possible; 10 total//
	char NAMES[10][9]= { "WEIGHTS", "COURT", "SPA", "SAUNA", "LOCKERS", "POOL", "STORE", "CLOSET", "FOODS", "GAINZ"};
	
	struct ROOMS one;
	one.rm_name = NAMES[4];

	
	printf("Test Room: NAMES 1 : %s \n", one.rm_name );
	
	return 0;
}
