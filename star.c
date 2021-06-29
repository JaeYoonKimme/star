#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>
#include<sys/stat.h>
#include<dirent.h>

char dst_file[PATH_MAX];

struct _s_header {
	char file_type;
	unsigned int path_size;
	unsigned int data_size;
	char path_name[PATH_MAX];
} ;

typedef struct _s_header	s_header ;
typedef struct _s_header * 	s_header_ptr ;

char *
path_cat (char * path, char * file)
{
	char * new_path = (char*) calloc(sizeof(char),PATH_MAX);
	strcpy(new_path, path);
	strcat(new_path, "/");
	strcat(new_path, file);
	return new_path;
}

void
write_file (char * target, FILE * dst)
{
	struct stat st;
	if(stat(target,&st) == -1){
		perror("stat error\n");
		exit(7);
	}
	
	s_header tmp;
	tmp.data_size = st.st_size;
	tmp.file_type = 1;
	tmp.path_size = strlen(target);
	strcpy(tmp.path_name,target);


	if(fwrite(&(tmp.file_type),sizeof(tmp.file_type),1,dst) != 1){
		printf("file write error\n");
		exit(5);
	}
	if(fwrite(&(tmp.path_size),sizeof(tmp.path_size),1,dst) != 1){
		printf("file write error\n");
		exit(5);
	}
	if(fwrite(&(tmp.data_size),sizeof(tmp.data_size),1,dst) != 1){
		printf("file write error\n");
		exit(5);
	}
	if(fwrite(tmp.path_name,tmp.path_size,1,dst) != 1){
		printf("file write eror\n");
		exit(5);
	}

	FILE * f = fopen(target,"rb");
	if(f == NULL){
		perror("file open eror\n");
		exit(4);
	}
	char buf[1024];
	size_t len;
	while(feof(f) == 0){
		len = fread(buf,1,sizeof(buf),f);
		fwrite(buf,1,len,dst);
	}
	fclose(f);

		
}

void
write_dir (char * target, FILE * dst)
{
	s_header tmp;
	tmp.file_type = 0;
	tmp.path_size = strlen(target);
	tmp.data_size = 0;
	strcpy(tmp.path_name, target);

	//write header
	if(fwrite(&(tmp.file_type),sizeof(tmp.file_type),1,dst) != 1){
		printf("file write error\n");
		exit(5);
	}
	if(fwrite(&(tmp.path_size),sizeof(tmp.path_size),1,dst) != 1){
		printf("file write error\n");
		exit(5);
	}
	if(fwrite(&(tmp.data_size),sizeof(tmp.data_size),1,dst) != 1){
		printf("file write error\n");
		exit(5);
	}
	if(fwrite(tmp.path_name,tmp.path_size,1,dst) != 1){
		printf("file write erorr\n");
		exit(5);
	}

	DIR *dp;
	struct dirent * ep;
	dp = opendir(target);
	if(dp == NULL){
		perror("fail to open dir\n");
		exit(6);
	}

	for( ;ep = readdir(dp); ){
		if(strcmp(ep->d_name,".") == 0){
			continue;
		}
		if(strcmp(ep->d_name,"..") == 0){
			continue;
		}
		if(ep->d_type == DT_DIR){
			char * newdir;
			newdir = path_cat(target,ep->d_name);
			write_dir(newdir, dst);
			free(newdir);
		}
		if(ep->d_type == DT_REG || ep->d_type == DT_LNK){
			char * newfile;
			newfile = path_cat(target,ep->d_name);
			write_file(newfile,dst);
			free(newfile);
		}
	}
}



void
archive (char * target)
{
	FILE * dst = fopen(dst_file,"w");
	if(dst == NULL){
		printf("%s\n",target);
		perror("file open error\n");
		exit(5);
	}

	write_dir(target,dst);
	fclose(dst);
}
void
list (char * s_file)
{
	FILE * f = fopen(s_file,"rb");
	if(f == NULL){
		perror("In function list(): file open error\n");
		exit(5);
	}
	
	int cursor = 0;
	while(feof(f) == 0){
		s_header tmp;
		cursor = cursor + fread(&tmp,sizeof(s_header),1,f);
		
		char * name = (char *) malloc(tmp.path_size);
		cursor =+ fread(name,sizeof(char),tmp.path_size,f);
		printf("%s\n",name);
		//......
	}
	fclose(f);
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
		list(argv[2]);
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
