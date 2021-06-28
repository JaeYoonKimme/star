#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
char src_path[PATH_MAX] ;
char dst_path[PATH_MAX] ;

char * 
path_cat (char * path, char * dir)
{
	char * new_path = (char *) calloc(sizeof(char), PATH_MAX) ;
	strcpy(new_path, path) ;
	strcat(new_path, "/") ;
	strcat(new_path, dir) ;
	return new_path ;
}

void 
fcopy (char * dir, char * fname) {
	char *src = path_cat(dir,fname);
	char *srcpath = path_cat(src_path,src);
	char *dstpath = path_cat(dst_path,src);

	FILE *f = fopen(srcpath, "rb");
	FILE *nf = fopen(dstpath,"w");

	if(f == NULL){
		printf("%s\n",srcpath);
		fputs("File open error(src_file)\n",stderr);
		exit(3);
	}
	if(nf == NULL){
		printf("%s\n",dstpath);
		fputs("File open error(dst_file)\n",stderr);
		exit(4);
	}

	char buf[512] ;	
	size_t len;
	while (feof(f) == 0)  {
		len = fread(buf,1,sizeof(buf),f);
		fwrite(buf, 1, len, nf);
	}

	fclose (f);
	fclose(nf);
}


void 
dcopy (char * dir) {
	DIR * dp;
	struct dirent * ep;
	char * path = path_cat(src_path, dir) ;
	
	dp = opendir(path) ;
	if (dp == NULL) {
		printf("%s\n",path);
		perror("fail to open directory") ;
		exit(1) ;
	}

	char * new_path = path_cat(dst_path, dir) ;
	if (mkdir(new_path, 0766) == -1) {
		if(errno != EEXIST){
			printf("%s\n",new_path);
			perror("fail to create a directory") ;
			exit(1) ;
		}
	}
	

	for ( ; ep = readdir(dp) ; ) {
		if (strcmp(ep->d_name, ".") == 0){
            		continue;
            	}
		if (strcmp(ep->d_name, "..") == 0) {
			continue ;
		}
		if (ep->d_type == DT_LNK) {
			continue ;
		}
		if (ep->d_type == DT_REG) {
			fcopy(dir, ep->d_name);
		}
		if (ep->d_type == DT_DIR) {
			char * subpath ;
			subpath = path_cat(dir, ep->d_name) ;
			dcopy(subpath) ;
		}
	} //for
	closedir(dp);
	free(path) ;
	free(new_path) ;
}

int
main (int argc, char** argv)
{
	if(argc != 3){
		fputs("Input error\n",stderr);
		exit(1);
	}

	if(mkdir(argv[2], 0766) == -1){
		if(errno != EEXIST){
			perror("fail to create a drectory");
			exit(1);
		}
	}

	strcpy(src_path, argv[1]) ;
	strcpy(dst_path, argv[2]) ;

	dcopy(".");
}
