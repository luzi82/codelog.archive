#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>

static int	copy_data(struct archive *, struct archive *);

int main()
{
	struct archive *a;
	struct archive *ext;
	struct archive_entry *entry;
	int r;
	
	a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_all(a);
	
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, 0);
	
	r = archive_read_open_filename(a, "t.tar.gz", 10240);
	if (r != ARCHIVE_OK)
	  return 1;
	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
	  printf("%s\n",archive_entry_pathname(entry));
	  //archive_read_data_skip(a);
	  archive_write_header(ext, entry);
	  copy_data(a, ext);
	  archive_write_finish_entry(ext);
	}
	r = archive_read_free(a);
	if (r != ARCHIVE_OK)
	  return 1;
	return 0;
}

static int
copy_data(struct archive *ar, struct archive *aw)
{
	int r;
	const void *buff;
	size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
	int64_t offset;
#else
	off_t offset;
#endif

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r != ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r != ARCHIVE_OK) {
			//warn("archive_write_data_block()",
			    //archive_error_string(aw));
			return (r);
		}
	}
}