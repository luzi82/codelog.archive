// c003: test unztell

#include <stdio.h>
#include <string.h>
#include "unzip.h"

unzFile zfPtr = NULL;
char filePathA[260];
unz_file_info fileInfo;
int uocfDone = 0;
unsigned char * buffer = NULL;
unsigned char * bufferOri = NULL;

FILE* ofp=NULL;

void clean();

int main()
{
	int ret;

	zfPtr = unzOpen("zip000.zip");
	if(zfPtr==NULL){clean();return 1;}

	ret = unzLocateFile(zfPtr,"HelloWorld.png",1);
	if(ret!=UNZ_OK){clean();return 1;}
	
	ret = unzGetCurrentFileInfo(zfPtr, &fileInfo, filePathA, sizeof(filePathA), NULL, 0, NULL, 0);
	if(ret!=UNZ_OK){clean();return 1;}
	
	ret = unzOpenCurrentFile(zfPtr);
	if(ret!=UNZ_OK){clean();return 1;}
	uocfDone = 1;
	
	ret = unztell(zfPtr);
	if(ret!=0){
		printf("DJZVXODI %d\n",ret);
		clean();return 1;
	}

	buffer = new unsigned char[300];
	ret = unzReadCurrentFile(zfPtr, buffer, 200);
	if(ret != 200){clean();return 1;}

	ret = unztell(zfPtr);
	if(ret!=200){
		printf("DJZVXODI %d\n",ret);
		clean();return 1;
	}
	
	clean();
	return 0;
}

void clean(){
	if(bufferOri){
		delete [] bufferOri;
		bufferOri=NULL;
	}
	if(ofp){
		fclose(ofp);
		ofp=NULL;
	}
	if(buffer){
		delete [] buffer;
		buffer=NULL;
	}
	if(uocfDone){
		unzCloseCurrentFile(zfPtr);
	}
	if(zfPtr!=NULL){
		unzClose(zfPtr);
		zfPtr=NULL;
	}
}
