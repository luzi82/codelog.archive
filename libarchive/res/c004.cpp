#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>

const int BUF_SIZE=4096;
FILE* ifp=NULL;
unsigned char buf[BUF_SIZE];
//int offset=0;

ssize_t rc(struct archive *,void *client_data, const void **buffer);
__LA_INT64_T sc(struct archive *,void *client_data, __LA_INT64_T request);
int oc(struct archive *, void*client_data);
int cc(struct archive *, void*client_data);

int main()
{
	struct archive *a;
	struct archive_entry *entry;
	int r;
	
	a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_all(a);
	r = archive_read_open2(a,0,oc,rc,sc,cc);
	//r = archive_read_open_filename(a, "t.tar.gz", 10240);
	if (r != ARCHIVE_OK)
	  return 1;
	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
	  printf("%s\n",archive_entry_pathname(entry));
	  archive_read_data_skip(a);  // Note 2
	}
	r = archive_read_free(a);  // Note 3
	if (r != ARCHIVE_OK)
	  return 1;
	return 0;
}

// typedef ssize_t archive_read_callback(struct archive *,void *client_data, const void **buffer)
ssize_t rc(struct archive *a,void *client_data, const void **buffer)
{
	//printf("archive_read_callback\n");
	ssize_t ret = fread(buf,1,BUF_SIZE,ifp);
	*buffer=buf;
	//offset+=ret;
	return ret;
}

// typedef int archive_skip_callback(struct archive *,void *client_data, size_t request)
__LA_INT64_T sc(struct archive *a,void *client_data, __LA_INT64_T request)
{
	printf("archive_skip_callback\n");
	fseek(ifp, request, SEEK_CUR);
	//offset+=ret;
	return request;

	//int x=(request>BUF_SIZE)?BUF_SIZE:request;
	//return fread(buf,1,x,ifp);
}

// typedef int archive_open_callback(struct archive *, void*client_data)
int oc(struct archive *a, void*client_data)
{
	printf("archive_open_callback\n");
	ifp = fopen("t.tar.gz","rb");
	//offset=0;
	return ARCHIVE_OK;
}

// typedef int archive_close_callback(struct archive *, void*client_data)
int cc(struct archive *a, void*client_data)
{
	printf("archive_close_callback\n");
	fclose(ifp);
	ifp = NULL;
	//offset=0;
	return ARCHIVE_OK;
}
