#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>

int main()
{
	struct archive *a;
	struct archive_entry *entry;
	int r;
	
	a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_all(a);
	r = archive_read_open_filename(a, "t.tar", 10240);
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
