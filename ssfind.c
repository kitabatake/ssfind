#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define MAX_DIR_DEPTH 1

char *searchPattern;

void searchRecursive(char *path, int depth);

int main(int argc, char *argv[])
{
	
	
	if(argc < 2){
		printf("please specify file search pattern.\n");
		return 0;
	}
	
	searchPattern = argv[1];
	
	searchRecursive("./", 0);
	
	return 0;
}

void searchRecursive(char *path, int depth){
	
	if(depth > MAX_DIR_DEPTH){
		return;
	}
	
	DIR *dir;
	
	struct dirent *dp;
	dir=opendir(path);
	
	char childDirName[256];
	
	for(dp = readdir(dir); dp != NULL; dp = readdir(dir)){
		
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
			continue;
		}
			
		if(dp->d_type == DT_REG && strstr(dp->d_name, searchPattern) != NULL){
			//TODO filtering
			
			for(int i = 0; i < depth; i++){
				printf("  ");
			}
			
			printf("%s%s\n",path, dp->d_name);
		}
		else if(dp->d_type == 4){
			
			strcpy(childDirName, path);
			strcat(childDirName, dp->d_name);
			strcat(childDirName, "/");
			searchRecursive(childDirName, depth + 1);
		}
	}
	
	closedir(dir);
	
}