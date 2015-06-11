#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define MAX_DIR_DEPTH 3
#define SEARCH_TYPE_NAME 0
#define SEARCH_TYPE_CONTENT 1
#define MAX_FILEPATH_LENGTH 100

char *searchPattern;
int searchType;

void searchRecursive(char *path, int depth);
void checkFileName(char *path, char *fileName, int depth);
void checkFileContent(char *path, char *fileName, int depth);

int main(int argc, char *argv[])
{
	int correctComandFlag = 1;
	
	if(argc != 3){
		correctComandFlag = 0;
	}
	else{
		if(strcmp(argv[1], "name") == 0){
			searchType = SEARCH_TYPE_NAME;
		}
		else if(strcmp(argv[1], "content") == 0){
			searchType = SEARCH_TYPE_CONTENT;
		}
		else{
			correctComandFlag = 0;
		}
	}
	
	if(!correctComandFlag){
		printf("illegal commands specified.\ncorrect comand is following.\nssfind [type name or content] [search word]\n");
		return 0;
	}
	
	searchPattern = argv[2];
	
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
	
	char childDirName[MAX_FILEPATH_LENGTH];
	
	for(dp = readdir(dir); dp != NULL; dp = readdir(dir)){
		
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
			continue;
		}
			
		if(dp->d_type == DT_REG ){
			
			if(searchType == SEARCH_TYPE_NAME){
				checkFileName(path, dp->d_name, depth);
			}
			else{
				checkFileContent(path, dp->d_name, depth);
			}
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

void checkFileName(char *path, char *fileName, int depth){
	
	if(strstr(fileName, searchPattern) != NULL){
		printf("%s%s\n",path, fileName);
	}
}

void checkFileContent(char *path, char *fileName, int depth){
	
	char filePath[MAX_FILEPATH_LENGTH];
	strcpy(filePath, path);
	strcat(filePath, fileName);
	
	FILE *fp;
	char buff[256];
	int lineNum = 1;
	
	if ((fp = fopen(filePath, "r")) == NULL) {
		printf("file open error at: %s\n", filePath);
		return;
	}
	
	while (fgets(buff, 256, fp) != NULL) {
		
		if(strstr(buff, searchPattern) != NULL){
			printf("%s (%d)\n", filePath, lineNum);
			printf(" - %s", buff);
		}
		
		lineNum++;
	}
	fclose(fp);
}











