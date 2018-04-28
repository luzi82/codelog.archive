#include <stdio.h>
#include "unzip.h"

unzFile zfPtr = NULL;
char filePathA[260];
unz_file_info fileInfo;
int uocfDone = 0;
unsigned char * buffer = NULL;

FILE* ofp=NULL;

int zfd_seek_file_func_count=0;

void clean();

voidpf   zfd_open_file_func      (voidpf opaque, const char* filename, int mode);
uLong    zfd_read_file_func      (voidpf opaque, voidpf stream, void* buf, uLong size);
uLong    zfd_write_file_func     (voidpf opaque, voidpf stream, const void* buf, uLong size);
int      zfd_close_file_func     (voidpf opaque, voidpf stream);
int      zfd_testerror_file_func (voidpf opaque, voidpf stream);
long     zfd_tell_file_func      (voidpf opaque, voidpf stream);
long     zfd_seek_file_func      (voidpf opaque, voidpf stream, uLong offset, int origin);

zlib_filefunc_def zfd={
	zfd_open_file_func,
	zfd_read_file_func,
	zfd_write_file_func,
	zfd_tell_file_func,
	zfd_seek_file_func,
	zfd_close_file_func,
};

int main()
{
	int ret;

	zfPtr = unzOpen2("zip000.zip",&zfd);
	if(zfPtr==NULL){clean();return 1;}

	ret = unzLocateFile(zfPtr,"HelloWorld.png",1);
	if(ret!=UNZ_OK){clean();return 1;}
	
	ret = unzGetCurrentFileInfo(zfPtr, &fileInfo, filePathA, sizeof(filePathA), NULL, 0, NULL, 0);
	if(ret!=UNZ_OK){clean();return 1;}
	
	ret = unzOpenCurrentFile(zfPtr);
	if(ret!=UNZ_OK){clean();return 1;}
	uocfDone = 1;
	
	buffer = new unsigned char[fileInfo.uncompressed_size];
	ret = unzReadCurrentFile(zfPtr, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
	if(ret != (int)fileInfo.uncompressed_size){clean();return 1;}
	
	ofp = fopen("c002.out","wb");
	if(ofp==NULL){clean();return 1;}
	
	ret=fwrite(buffer,fileInfo.uncompressed_size,1,ofp);
	if(ret!=1){clean();return 1;}
	
	if(zfd_seek_file_func_count<=0){clean();return 1;}
	
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

// refer to ioapi.c

// fopen_file_func
voidpf   zfd_open_file_func      (voidpf opaque, const char* filename, int mode)
{
	return fopen(filename,"rb");
}

// fread_file_func
uLong    zfd_read_file_func      (voidpf opaque, voidpf stream, void* buf, uLong size)
{
	printf("read %ld\n",size);
    uLong ret;
    ret = (uLong)fread(buf, 1, (size_t)size, (FILE *)stream);
    return ret;
}

// fwrite_file_func
uLong    zfd_write_file_func     (voidpf opaque, voidpf stream, const void* buf, uLong size)
{
    uLong ret;
    ret = (uLong)fwrite(buf, 1, (size_t)size, (FILE *)stream);
    return ret;
}

// fclose_file_func
int      zfd_close_file_func     (voidpf opaque, voidpf stream)
{
    int ret;
    ret = fclose((FILE *)stream);
    return ret;
}

// ferror_file_func
int      zfd_testerror_file_func (voidpf opaque, voidpf stream)
{
    int ret;
    ret = ferror((FILE *)stream);
    return ret;
}

// ftell_file_func
long     zfd_tell_file_func      (voidpf opaque, voidpf stream)
{
	printf("tell\n");
    long ret;
    ret = ftell((FILE *)stream);
    return ret;
}

// fseek_file_func
long     zfd_seek_file_func      (voidpf opaque, voidpf stream, uLong offset, int origin)
{
	printf("seek %ld %d\n",offset,origin);
	//printf("zfd_seek_file_func offset=%d origin=%d\n",(int)offset,origin);
	++zfd_seek_file_func_count;

    int fseek_origin=0;
    long ret;
    switch (origin)
    {
    case ZLIB_FILEFUNC_SEEK_CUR :
        fseek_origin = SEEK_CUR;
        break;
    case ZLIB_FILEFUNC_SEEK_END :
        fseek_origin = SEEK_END;
        break;
    case ZLIB_FILEFUNC_SEEK_SET :
        fseek_origin = SEEK_SET;
        break;
    default: return -1;
    }
    ret = 0;
    if (fseek((FILE *)stream, offset, fseek_origin) != 0)
        ret = -1;
    return ret;
}
