#ifndef GANJAX_FAT12_H
#define GANJAX_FAT12_H
#pragma off(unreferenced)
#include <kernel/io/io.h>
#include <stdint.h>

#define IO_READ  1
#define IO_WRITE 0

#define ENTRY_SIZE 32

#define WEEDFILE_NOEXISTS -1
#define WEEDFILE_LOADED 0
#define FAT12_EOC 0xFFF
#define FAT12_FREE 0x00
#define FAT12_REUSED 0x5
#define WEEDFILE_ADJUST 32

#define DISK_OP_OK 0
#define DISK_NOT_READY 1
#define DISK_UNABLE_RESET 2
#define DISK_NO_SPACE_FOR_ENTRY 3
#define DISK_BAD_SIZE 4
#define DISK_NO_FREE_CLUSTERS 5
#define DISK_NO_ENOUGH_CLUSTERS 6
#define DISK_FILE_NO_EXISTS 7

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
static unsigned int  _fat_offset;
static unsigned int  _ent_offset;

static __segment    ram_segment = RAM_SEGMENT;
static char __based(ram_segment) *fat_buffer;
static char __based(ram_segment) *file_buffer;

static char _ready = 0x00;
static uint16_t first_free_cluster;
/** ==================================================== **/

int8_t floppy_rw_sectors(uint16_t offset_, int16_t lba, uint16_t count, uint8_t read);
int8_t floppy_reset_drive();

fat12_entry_t* fat12_search(const char* _x);

void fat12_init_filesystem();
void fat12_load_bpb();

void fat12_load_fat();
void fat12_write_fat();

void fat12_load_files();
void fat12_write_files();


char* fat12_to_fat_style(const char* _s);
uint8_t fat12_create_new_entry(fat12_entry_t *ent);
char fat12_write_file(short offset_, fat12_entry_t * ent);
char __based(ram_segment) *fat12_load_file(uint16_t offset, const char* _name);
uint8_t fat12_delete_file(const char *_name);
void fat12_print_file_clusters(uint16_t starring);
void fat12_ls();
void fat12_load_files_table();
void fat12_search_file(const char* _name);

static void fat12_search_first_free_cluster();
static int fat12_cluster_to_lba(int cluster);
static int fat12_compute_fat_lba();
static int fat12_compute_fat_size();

static int fat12_compute_ent_lba();
static int fat12_compute_ent_size();

static uint16_t fat12_get_fat_entry(uint16_t clusternum);
static void fat12_set_fat_entry(uint16_t clusternum, uint16_t value);
#endif

