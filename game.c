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

struct ROOMS{
	char name[9];
	char links[7][9];
	int type;
	int count;
};

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
	char file_name[16];
	memset(file_name, '\0', sizeof(file_name));
	char file_io[256];
	memset(file_io, '\0', sizeof(file_io));
	
	char dmp[48];
	char dmp2[48];
	char rm_name[9];
	//memset( rm_name, '\0', sizeof(rm_name));
	
	struct ROOMS layout[7];
	int y;
	for( y = 0; y < 7; y++){
		layout[y].type = 1;
		layout[y].count = 0;
	}
	int room_process_count = 0;
	char *variant = "CONNECTION";
	char *variant2 = "TYPE";
	roomsDIR = opendir(sourceDIR);
	if(roomsDIR > 0){
		while((subFiles = readdir(roomsDIR)) != NULL){
			//skip over the . and .. directories //
			while((strcmp(subFiles->d_name ,".") == 0) || (strcmp(subFiles->d_name ,"..") == 0)){
				subFiles = readdir(roomsDIR);
			}
				//clear out our char array and copy in the room file names //
				memset(file_name, '\0', sizeof(file_name));
				strcpy(file_name, subFiles->d_name);
				
				//format the directory/file_name to open it properly
				sprintf(file_io, "./%s/%s", sourceDIR, file_name);
				rm_FILE = fopen(file_io, "r");
				int x=0;
				
				char txt[24];
				memset( txt, '\0', sizeof(txt));
				while(fgets(txt, 24, rm_FILE)){
					sscanf(txt, "%s %s %s", dmp, dmp2, rm_name);
					if(x == 0){
						strcpy(layout[room_process_count].name, rm_name);
						x++;
					}
					if(strstr(dmp , variant)){
						strcpy(layout[room_process_count].links[layout[room_process_count].count],rm_name);
						layout[room_process_count].count++;
					}
					if(strstr(dmp2, variant2)){
						if( strstr(rm_name, "START")){
							printf("hi\n");
							layout[room_process_count].type = 0;
						}
						else if( strstr(rm_name, "END")){
							layout[room_process_count].type = 2;
						}
					}
					memset( txt, '\0', sizeof(txt));
				}
				room_process_count++;
		}
	}
	closedir(roomsDIR);
	
	int a,b;
	for(a = 0; a < 7; a++){
		printf("Name: %s\n" , layout[a].name);
		for(b = 0 ; b < layout[a].count; b++){
			printf("Con: %s\n" , layout[a].links[b]);
		}
		printf("Type: %i\n" , layout[a].type);
	}
	
	
	return 0;
	
}