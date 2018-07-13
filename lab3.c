/*
@author: Hani Nguyen
Class: 3320 - 001
Lab: 3
*/



#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#define BLOCKSIZE    256
#define FILENAMESIZE 56
#define TIMELENGTH   100
#define LINK_MAX     10

typedef struct BPTable{
	//size of file
	int fileSize;

	//index to dataBlock
 	int dataBlockPointer;

 	//When last modified
	char modifiedDT[TIMELENGTH];
 
} BPTable;

typedef struct FNTable{

	//name of file being stored
	char fileName[FILENAMESIZE];

	//iNode Pointer
  	BPTable iNode;
} FNTable;

struct Link{
	char linkName[FILENAMESIZE];
  	BPTable *linkedFile;
} Link;


struct FileSystem{

	//Name of File System
	char fileSystemName[FILENAMESIZE];
	
	//Number of Data blocks in System
 	int number_of_blocks;
 	
 	//Keep track of ABPT blocks avail                   
 	int numABPT;
 	
 	//Keeping track of FNT blocks that are available                            
 	int numFNT;

 	//Maps available data slot for FNT
 	int *FNTMap;
 	
 	//Maps available data slot for ABPT
 	int *ABPTMap;

 	//Maps available data slot for datablock CPM style                            
 	int *dataBlockMap;

 	 //Structure for holding initial files
 	struct FNTable *files;
 	
 	struct BPTable *pointerTables;
 	
 	char **dataBlocks;
 	
 	struct Link link[LINK_MAX];
} FileSystem;


struct FileSystem FS;


void formatFS(char name[FILENAMESIZE],int numPointerEntries,int numFNT){


  	printf("Creating FS with..\n name:%s\n #pointers: %d\n#files:%d \n",name,numPointerEntries,numFNT);

  	//storing the value in the filesystem

  	//storing the name of the filesystem
  	strcpy(FS.fileSystemName,name);

  	//storing the number of files.
  	FS.numFNT=numFNT;

  	//storing the number of pointer enteries.
  	FS.numABPT=numPointerEntries;
  	
  	//initializing space for the files, pointer tables, FNTMAP and the ABPTMAP
  	FS.files=malloc(numFNT*sizeof(struct FNTable));
  	FS.pointerTables=malloc(numPointerEntries*sizeof(struct BPTable));  
  	FS.FNTMap= malloc(numFNT*sizeof(int*));
  	FS.ABPTMap= malloc(numPointerEntries*sizeof(int*));
}
  
void createFS(int number_of_blocks){
  	
  	int i;
  	int j;
  	char **data_holder;

  	printf("[+] %d datablocks selected\n",number_of_blocks);
  	FS.number_of_blocks=number_of_blocks;

  	data_holder=(char**) malloc(BLOCKSIZE*sizeof(char*));
  	for(i=0;i<number_of_blocks;i++){
	   	data_holder[i]=(char*)malloc(number_of_blocks*sizeof(char));    
  	} 

  	FS.dataBlockMap= malloc(number_of_blocks*sizeof(int*));
  	FS.dataBlocks=data_holder;
  
}

//Main function where the menu and the functions are called.
void run() {
  	int option=-1;

  	char fileSystemName[FILENAMESIZE];
  
  
  while(1){

  	printf("\n---------MENU---------\n");
    printf("1) Create File System\n");
    printf("2) Format File System\n");
    printf("3) Exit\n");
    printf("------------------------\n");
    printf("Enter an option: ");
    scanf("%d",&option);
    printf("\n");
    switch(option){

	    case 1:
  			printf("Enter the number of data blocks in the system: \n");
  			int numberOfBlocks;
  			scanf("%d",&numberOfBlocks);	
  			createFS(numberOfBlocks);
  			printf("[+] Disk has been created\n\n");
			break;
	    case 2:
  			printf("[+] formatting the file system\n");	
  			printf("Enter the name of file system: \n");
  			scanf("%s",fileSystemName);
  			printf("Enter the number of block pointers: \n");
  			int numBlockPointers;
  			int numFiles;
  			scanf("%d",&numBlockPointers);
  			printf("Enter the number of files: \n");
  			scanf("%d",&numFiles);
  			formatFS(fileSystemName,numBlockPointers,numFiles);
  			printf("[+] Disk has been formatted\n\n");
  			break;
      case 3:
        exit(0);
    }
  }
}

int main(){
	run();
 	return 0; 
}
