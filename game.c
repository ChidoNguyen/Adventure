/*Chido Nguyen 931506965
*Program 2: Adventures
*Game File: Program will load files created by rooms.c and link them as the document suggests. Game will prompt users via text to see "where" they want to go.
*The program will also utilize mutex/threads 
*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

int main(){
	//2.4 Lecture by Brewster//
	int newestDirTime = -1; // set to -1 so we can check/replace anything created after
	char targetPre[32] = "nguychid.rooms."; // dir prefix we hard coded 
	char sourceDIR[256]; 	// storage for our "newest" searched dir
	memset(sourceDIR, '\0', sizeof(sourceDIR)); //init array with null terms
	
	
	DIR* searchDIR; 		// source directory 
	struct dirent *subDIR; 	//current subdir in our source dir
	struct stat dirINFO; 	// information about our target directory
	
	
	searchDIR=opendir(".");	//directory program was ran in
	
	if( searchDIR > 0){		// checks for success dir open
		while ((subDIR = readdir(searchDIR)) != NULL){		//readdir gives us a pointer to the next subdir in our source director http://pubs.opengroup.org/onlinepubs/7908799/xsh/readdir.html
			if(strstr(subDIR->d_name, targetPre) !=NULL){	 //strstr checks if our prefix is in the current subDIR target pointer
				
				
			}
		}
	}
	
	
	
	closedir(searchDIR);
	return 0;
	
}