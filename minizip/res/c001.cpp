#include <stdio.h>
#include "unzip.h"

unzFile zfPtr = NULL;
char filePathA[260];
unz_file_info fileInfo;
int uocfDone = 0;
unsigned char * buffer = NULL;

FILE* ofp=NULL;

void clean();

int main()
{
	int ret;

	zfPtr = unzOpen("zip000.zip");
	if(zfPtr==NULL){clean();return 1;}

	ret = unzLocateFile(zfPtr,"QIxndqXX/square.png",1);
	if(ret!=UNZ_OK){clean();return 1;}
	
	ret = unzGetCurrentFileInfo(zfPtr, &fileInfo, filePathA, sizeof(filePathA), NULL, 0, NULL, 0);
	if(ret!=UNZ_OK){clean();return 1;}
	
	ret = unzOpenCurrentFile(zfPtr);
	if(ret!=UNZ_OK){clean();return 1;}
	uocfDone = 1;
	
	buffer = new unsigned char[fileInfo.uncompressed_size];
	ret = unzReadCurrentFile(zfPtr, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
	if(ret != (int)fileInfo.uncompressed_size){clean();return 1;}
	
	ofp = fopen("c001.out","wb");
	if(ofp==NULL){clean();return 1;}
	
	ret=fwrite(buffer,fileInfo.uncompressed_size,1,ofp);
	if(ret!=1){clean();return 1;}
	
	clean();
	return 0;
}

void clean(){
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
