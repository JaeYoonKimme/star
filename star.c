#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>
#include<sys/stat.h>

char dst_file[PATH_MAX];

struct _s_header {
	char f_type;
	unsigned int path_size;
	unsigned int data_size;
} ;

typedef struct _s_header	s_header ;
typedef struct _s_header * 	s_header_ptr ;

void
write_file (char * target)
{
	struct stat st;
 	if(stat(target, &st) == -1){
                perror("stat error");
                exit(3);
        }

	//file
        if(S_ISREG(st.st_mode)){
        	FILE * tar = fopen(target,"rb");
		if(tar == NULL){
			perror("file open error\n");
			exit(4);
		}

		FILE * des = fopen(dst_file,"ab");
		if(des == NULL){
			perror("file open error\n");
			exit(4);
		}

		s_header tmp;
		tmp.f_type = '0';
		tmp.path_size = strlen(target);
			
		fseek(tar, 0, SEEK_END);
		tmp.data_size = ftell(tar);
		rewind(tar);
		
		//Write header
		if(fwrite((void*)(&tmp),sizeof(s_header),1,des) != 1 ){
			printf("file write error\n");
			exit(5);
		}

		//Write name
		if(fwrite(target,sizeof(char),tmp.path_size,des) != tmp.path_size){
			printf("file write error\n");
			exit(5);
		}

		//Write data
		char * buf[1024];
		size_t len;
		while(feof(tar) == 0){
			len =+ fread(buf,1,sizeof(buf),tar);
			fwrite(buf,1,len,des);
		}
		fclose(tar);
		fclose(des);
	}


        //Directory
        if(S_ISDIR(st.st_mode)){
        }
}


void
archive (char * target)
{
	struct stat st;
	if(stat(target, &st) == -1){
		perror("stat error");
		exit(3);
	}
	//One file
	if(S_ISREG(st.st_mode)){
		write_file(target);			
	}
	//Directory
	if(S_ISDIR(st.st_mode)){
		printf("Archive directory\n");	
	}
}

void
list (char * s_file)
{

}

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
			printf("archive mode : No directory or file named %s\n",argv[3]);
			exit(2);
		}
		if(access(argv[3],R_OK) != 0){
			printf("archive mode : No permission to access %s\n",argv[3]);
			exit(2);
		}
		strcpy(dst_file,argv[2]);
		archive(argv[3]);
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
