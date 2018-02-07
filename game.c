/*Chido Nguyen 931506965
*Program 2: Adventures
*Game File: Program will load files created by rooms.c and link them as the document suggests. Game will prompt users via text to see "where" they want to go.
*The program will also utilize mutex/threads 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

int main(){
	
	/*2.4 Lecture by Brewster
	https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm
	http://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html
	http://pubs.opengroup.org/onlinepubs/7908799/xsh/readdir.html
	http://pubs.opengroup.org/onlinepubs/009695399/functions/stat.html
	https://linux.die.net/man/2/stat
	*/
	int newestDirTime = -1; // set to -1 so we can check/replace anything created after
	char targetPre[32] = "nguychid.rooms."; // dir prefix we hard coded 
	char sourceDIR[256]; 	// storage for our "newest" searched dir
	memset(sourceDIR, '\0', sizeof(sourceDIR)); //init array with null terms
	
	
	DIR* searchDIR; 		// source directory 
	struct dirent *subDIR; 	//current subdir in our source dir
	struct stat dirINFO; 	// information about our target directory
	
	
	searchDIR=opendir(".");	//directory program was ran in
	
	if( searchDIR > 0){		// checks for success dir open
		while ((subDIR = readdir(searchDIR)) != NULL){		//readdir gives us a pointer to the next subdir in our source director 
			if(strstr(subDIR->d_name, targetPre) !=NULL){	 //strstr checks if our prefix is in the current subDIR target pointer
				stat(subDIR->d_name, &dirINFO); // grab our sub directory information and send it to dirINFO structure 
				if( (int)dirINFO.st_mtime > newestDirTime ){	//cast time of last modification to an int and check to see if its greater than current newest directory time. greater = newer 
					newestDirTime = (int)dirINFO.st_mtime; // set newest time
					memset(sourceDIR, '\0', sizeof(sourceDIR));
					strcpy(sourceDIR, subDIR->d_name); // copy sub dir to our source
				}
				
			}
		}
	}
	closedir(searchDIR);
	/*
	 Similar to finding our newest director. We use same functions to open that saved directory inside sourceDIR. readdir through for each of the room files.
	*/
	DIR* roomsDIR;
	struct dirent* subFiles;
	FILE* rm_FILE;
	roomsDIR = opendir(sourceDIR);
	if(roomsDIR > 0){
		while((subFiles = readdir(roomsDIR)) != NULL){
			if( strcmp(subFiles->d_name,"LOCKERS") == 0){
			rm_FILE = fopen(subFiles->d_name, "r");
			char ch;
			ch = fgetc(rm_FILE);
			while (ch != EOF){
				printf("%c", ch);
				ch = fgetc(rm_FILE);
			}
			fclose(rm_FILE);
			}
		}
	}
	closedir(roomsDIR);
	
	return 0;
	
}