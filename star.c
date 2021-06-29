#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int 
main (int argc, char ** argv)
{	
	if(argc < 2 || argc > 4){
		printf("Wrong number of input files\n");
		exit(1);
	}
	

	//Archive mode
	if(strcmp(argv[1],"archive") == 0){
		if(argc != 4){
			printf("archive mode : wrong input\nstar archive <archive-file-name> <target directory path>\n");
			exit(1);
		}
		if(access(argv[2],F_OK) == 0){
			printf("archive mode : %s is already exist\n",argv[2]);
			exit(2);
		}
		if(access(argv[3],F_OK) != 0){
			printf("archive mode : No directory named %s\n",argv[3]);
			exit(2);
		}
		if(access(argv[3],R_OK) != 0){
			printf("archive mode : No permission to access %s\n",argv[3]);
			exit(2);
		}
		printf("archive mode\n");
		//Archive()
	}
	//List mode
	else if(strcmp(argv[1],"list") == 0){
		if(argc != 3){
			printf("list mode : wrong input\nstar list <archive-file-name>\n");
			exit(1);
		}
		if(access(argv[2],F_OK) != 0){
			printf("list mode : No file named %s\n",argv[2]);
			exit(2);
		}
		if(access(argv[2],R_OK) != 0){
			printf("list mode : No permission to access %s\n",argv[2]);
			exit(2);
		}
		printf("list mode\n");
		//List()
	}
	//Extract mode
	else if(strcmp(argv[1],"extract") == 0){
		if(argc != 3){
			printf("extract mode : wrong input\nstar extract <archive-file-name>\n");
			exit(1);
		}
		if(access(argv[2],F_OK) != 0){
			printf("extract mode : No file named %s\n",argv[2]);
			exit(2);
		}
		if(access(argv[2],R_OK) != 0){
			printf("extract mode : No permission to access %s\n",argv[2]);
			exit(2);
		}
		printf("extract mode\n");
		//Extract()
	}
	//Else
	else{
		printf("Unrecognized Option\n");
		exit(1);
	}
}
