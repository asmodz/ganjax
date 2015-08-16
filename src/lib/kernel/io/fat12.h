#ifndef GANJAX_FAT12_H
#define GANJAX_FAT12_H
#include <kernel/io/io.h>
#define IO_READ  1
#define IO_WRITE 0
#define RAM_SEGMENT 0x07E0


_Packed struct fat12_entry_t{
	char filename[8];
	char extension[3];
	char attributes;
	short reserved;
	short create_time;
	short create_date;
	short last_access_date;
	short ignore_in_fat12;
	short last_write_time;
	short last_write_date;
	short first_cluster;
	long int   file_size;
};

_Packed struct fat12_bpb_t{
	char oem_name[8];
	int bytes_per_sector;
	char sectors_per_cluster;
	int reserved_sectors;
	char number_of_fats;
	int max_files;
	int total_sectors;
	char media_sign;
	int sectors_per_fat;
	int sectors_per_track;
	int heads_per_cylinder;
	long int hidden_sectors;
	long int total_sectors_big;
	char drive_number;
	char unused;
	char ext_boot_signature;
	long int serial_number;
	char volume_label[11];
	char file_system[8];
};


typedef struct fat12_entry_t fat12_entry_t;
typedef struct fat12_bpb_t   fat12_bpb_t;

static struct fat12_bpb_t fat12_bpb;
static char _ready = 0x00;

char floppy_rw_sectors(short offset_, short lba, short count, char read);
char floppy_reset_drive();

void fat12_load_bpb();
void fat12_load_fat();
void fat12_load_files();

void fat12_load_files_table();
void fat12_search_file(const char* _name);

#endif
