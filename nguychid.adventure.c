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
#include <pthread.h>
#include <time.h>

//https://www.thegeekstuff.com/2012/05/c-mutex-examples/?refcom
pthread_t tid[2];
pthread_mutex_t lock;
char* sharedTIME = "currentTime.txt";


/*
Structure used to store room info when read in from file
name of file
which room names it is linked with
type refers to mid/end/start
count used to keep track of  how many rooms its linked with
*/
struct ROOMS{
	char name[9];
	char links[6][9];
	int type;
	int count;
};
/*
Player structure:
Stores current room via name the user is in
Status determines if the room user is in end room yet
Steps counts # of steps
hist array stores rooms the user transverses through
*/
struct Player{
	char room[9];		//which room you're in
	int status;		//END room status
	int steps;
	char hist[100][9];
	
};


//https://linux.die.net/man/3/strftime//
//https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm//
//https://www.thegeekstuff.com/2012/05/c-mutex-examples/?refcom//

/*
timefortime: writes formatted timestamp to currentTime.txt
Used with threading, function locks -> writes -> unlock
*/
void* timefortime(){
	pthread_mutex_lock(&lock);
	FILE*stuff = fopen(sharedTIME, "w");
	char timestamp[256];
	memset(timestamp, '\0', sizeof(timestamp));
	time_t t;
	struct tm *info;
	time(&t);
	info = localtime( &t);
	strftime( timestamp, 256,"%X, %A, %B %d, %Y", info);
	fprintf(stuff,"%s\n", timestamp);
	fclose(stuff);
	pthread_mutex_unlock(&lock);
}

/*
telltime - reads the formatted timestamp from timefortime and outputs to console 
Used with threading counterpart timefortime: lock -> read ->print -> unlock
It also runs a file check to make sure the file has been written to before reading. If empty
function will loop and unlock to give timefortime a chance to write before relocking
*/
void telltime(){
	FILE * stuff;
	char timeout[256];
	memset(timeout, '\0', sizeof(timeout));
	stuff = fopen(sharedTIME, "r");
	fgets(timeout, 256,stuff);
	printf("\n\n%s\n", timeout);
	fclose(stuff);
}

/*Returns room name from the ROOMS struct array and a given index
Input: struct ROOMS* array, and integer index
Output: Pointer to the structures name array string
*/
char* GetRoomName(struct ROOMS* arr, int x){
	return arr[x].name;
}

/*
Returns integer index representing the start room in the struct ROOMS* array
*/
int FindStart(struct ROOMS* arr){
	int x;
	for(x = 0; x < 7; x++){
		if(arr[x].type == 0){
			return x;
		}
	}
}

/*
Params: struct for Players and ROOMS
Output: No return, but populates the current room the user starts in
*/
void SetupPlayer( struct Player* p1, struct ROOMS* arr){
	int x = FindStart(arr);
	strcpy(p1->room , arr[x].name);
}
/*
Basic Helper function returns room name from Player struct
*/
char* CurRoom(struct Player* p1){
	return p1->room;
}

/*
Function returns array index integer equivalent to its name
*/
int RoomNumba(struct ROOMS* arr, struct Player* p1){
	char * tmp = CurRoom(p1);
	int x;
	for(x = 0; x < 7; x++){
		if(!strcmp(tmp, arr[x].name))
			return x;
	}
}
/*
PossCons
In: ROOMS struct array
Out: Generates printf command to print possible room connections
*/
void PossCons(struct ROOMS* arr, struct Player* p1){
	int ind = RoomNumba(arr,p1);
	printf("POSSIBLE CONNECTIONS: ");
	int x;
	for( x = 0; x < arr[ind].count; x++){
		if(x+1 == arr[ind].count){
			printf("%s.\n", arr[ind].links[x]);
		}
		else {
			printf("%s, ",arr[ind].links[x]);
		}
	}
	
}
/*
RoomCheck- Verifies if userinput is valid with room connections
Input: struct Rooms and Player, and user input
Output: 1 for true userinput is part of room connections
		0 if not part
*/
int RoomCheck(struct ROOMS* arr, struct Player* p1, char* userIn){
	int x;
	int loop = RoomNumba( arr, p1);
	for(x = 0; x < arr[loop].count; x++){
		if(!(strcmp(userIn, arr[loop].links[x]))){
			return 1;
		}
	}
	return 0;
}

/*
CoachSession
Input: ROOMS* array , Player struct, and userInput for where to?
Output: Nothing returned. Does a string check to see if legal room choice input
		-Call time function instead if time
		-Update user location/ status as needed.
*/


void CoachSession(struct ROOMS* arr, struct Player* p1, char* userIn){
	char *tm = "time";
	int verify;
	int str_time = strcmp(userIn, tm);
	verify = RoomCheck(arr,p1,userIn);
	if(verify){
		strcpy(p1->hist[p1->steps],p1->room);
		p1->steps++;
		memset(p1->room, '\0', sizeof(p1->room));
		strcpy(p1->room, userIn);
	}
	else
		printf("\nHUH? I DONT'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
	
}

/*
EndGame - Checks to see if current room user is in is end game to signal "finished"
Param: structure of our Player and our layout struct arr
Output: Void, alters structure status to show end game has been met
		changes p1->status to 0 if end_game is met
*/
void EndGame(struct ROOMS* arr, struct Player* p1){
	int tmp_rm = RoomNumba(arr, p1);
	if( arr[tmp_rm].type == 2 ){
		p1->status = 0;
	}
}

/*
printWinner()- prints out you've found exit , # of steps , and steps taken
input: both struct ROOMS and Player to grab name and step count
output: winner / step count / route
*/
void printWinner(struct Player* p1){
	printf("\n\nEXIT ROOM FOUND. END OF GAME\n\n");
	printf("YOU TOOK %i STEPS.\nYOUR PATH HISTORY:\n", p1->steps);
	int x;
	for(x = 1; x < p1->steps; x++){
		printf("%s\n", p1->hist[x]);
	}
	printf("%s\n", p1->room);
}


/*
Game function: prompts user where they want to go and handles traversing the rooms + time
*/
void* game(void* layout){
	pthread_mutex_lock(&lock);
	
	FILE* fp;
	fp= fopen(sharedTIME, "w");
	fclose(fp);
	struct Player gymbro;
	gymbro.status = 1;
	gymbro.steps = 0 ;
	SetupPlayer(&gymbro, layout);
	char userInput[64];
	char *pos;
	memset(userInput, '\0', sizeof(userInput));
	
	// Loop the "game" while end game hasn't been achieved
	do{
		printf("CURRENT ROOM: %s\n", CurRoom(&gymbro));
		PossCons(layout, &gymbro);
		printf("WHERE TO? >");
		fgets(userInput, 64, stdin);
		//https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input//
		// Removing trailing new line in user input
		if(( pos = strchr (userInput, '\n')) != NULL)
			*pos = '\0';
		
		while(strcmp(userInput, "time") == 0){
			//pthread_create(&tid[0], NULL, &timefortime, (void*) fp);
			pthread_mutex_unlock(&lock);
			pthread_create(&tid[1], NULL, &timefortime, NULL);
			pthread_join(tid[1], NULL);
			pthread_mutex_lock(&lock);
			telltime();// prints out time after other thread writes to file //
			memset(userInput, '\0', sizeof(userInput));
			printf("WHERE TO? >");
			fgets(userInput, 64, stdin);
			if(( pos = strchr (userInput, '\n')) != NULL)
			*pos = '\0';
		}
		CoachSession(layout, &gymbro, userInput);
		EndGame(layout,&gymbro);
	}while(gymbro.status);
	
	printWinner(&gymbro);
	pthread_mutex_unlock(&lock);
}
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
	
	
	
	//create our 7 room struct array and initialize some base values
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
					sscanf(txt, "%s %s %s", dmp, dmp2, rm_name);		//parse our read in line 
					if(x == 0){
						strcpy(layout[room_process_count].name, rm_name);		// copy room name
						x++;
					}
					if(strstr(dmp , variant)){
						strcpy(layout[room_process_count].links[layout[room_process_count].count],rm_name);		// if connections add to links
						layout[room_process_count].count++;
					}
					if(strstr(dmp2, variant2)){		// if type check to see if start or end room 
						if( strstr(rm_name, "START")){
							layout[room_process_count].type = 0;
						}
						else if( strstr(rm_name, "END")){
							layout[room_process_count].type = 2;
						}
					}
					memset( txt, '\0', sizeof(txt));
				}
				fclose(rm_FILE);
				room_process_count++;
		}
	}
	closedir(roomsDIR);
	//start the game //
	pthread_create(&tid[0], NULL, &game, (void*) layout);
	pthread_join(tid[0],NULL);
	pthread_mutex_destroy(&lock);
	return 0;
	
}