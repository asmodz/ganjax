#ifndef GANJAX_FAT12_H
#define GANJAX_FAT12_H

#pragma off(unreferenced)

#include <stdint.h>
#include <kernel/io/io.h>

#define FAT12_ENTRY_FREE       0xE5
#define FAT12_ENTRY_LAST       0x00
#define FAT12_CLUSTER_EMPTY    0x00
#define FAT12_END_OF_CLUSTERS  0xFFF
#define FAT12_FREE_CLUSTER     0x00

#define IO_READ                1
#define IO_WRITE               0

#define ENTRY_SIZE             32
#define MAX_KERNEL_FILES       3

#define DISK_OP_OK 			   0x0
#define DISK_UNABLE_RESET      0x1
#define DISK_ROOT_LOAD_FAIL    0x2
#define DISK_FAT_LOAD_FAIL     0x3
#define DISK_ROOT_SAVE_FAIL    0x4
#define DISK_FAT_SAVE_FAIL     0x5
#define DISK_ENTRY_OVERWRITE   0x6
#define DISK_NO_SPC_FOR_ENT    0x7
#define DISK_FILE_NOEXIST      0x8
#define DISK_FSLOT_NON_EMPTY   0x9
#define DISK_NO_ENOUGH_SPACE   0xA
#define DISK_FILE_SIZE_IS_ZERO 0xB

_Packed struct fat12_entry_t{
	int8_t filename[8];
	int8_t extension[3];
	int8_t attributes;
	int16_t reserved;
	int16_t create_time;
	int16_t create_date;
	int16_t last_access_date;
	int16_t ignore_in_fat12;
	int16_t last_write_time;
	int16_t last_write_date;
	int16_t first_cluster;
	int32_t file_size;
};

_Packed struct fat12_bpb_t{
	int8_t oem_name[8];
	int16_t bytes_per_sector;
	int8_t sectors_per_cluster;
	int16_t reserved_sectors;
	int8_t number_of_fats;
	int16_t max_files;
	int16_t total_sectors;
	int8_t media_sign;
	int16_t sectors_per_fat;
	int16_t sectors_per_track;
	int16_t heads_per_cylinder;
	int32_t hidden_sectors;
	int32_t total_sectors_big;
	int8_t drive_number;
	int8_t unused;
	int8_t ext_boot_signature;
	int32_t serial_number;
	int8_t volume_label[11];
	int8_t file_system[8];
};

_Packed struct file_t{
	struct fat12_entry_t entry;
	char __based(ram_area) *data;
};

typedef struct fat12_entry_t fat12_entry_t;
typedef struct fat12_bpb_t   fat12_bpb_t;
typedef struct file_t        file_t;
/** ==================================================== **/

static __segment RAMSEG     = 0x07e0;
static uint16_t FAT_OFFSET  = 0x0000;
static uint16_t ROOT_OFFSET = 0x0000;
static uint16_t DATA_OFFSET = 0x0000;


static fat12_bpb_t _bpb;
static char __based(RAMSEG) *fat;
static fat12_entry_t __based(RAMSEG) *entries;
static file_t  *_file_handle = 0;

/** ======================API=========================== **/

int8_t init_fs();
int8_t load_file(const char *__s, file_t *_fhandle);
int8_t get_entry_by_name(const char* __s, fat12_entry_t *__e);

void   print_bpb();
void   print_lba();
void   print_offsets();
void   print_files();
void   print_entry(fat12_entry_t *__e);
void   print_cluster_list(uint16_t clusternum);
void   free_file();


/** ==================================================== **/

static int8_t floppy_rw_sectors(uint16_t offset_, int16_t lba, uint16_t count, uint8_t read);
static int8_t floppy_reset_drive();

static uint16_t fat_lba();
static uint16_t root_lba();
static uint16_t data_lba();

static uint16_t fat_size();
static uint16_t root_size();
static uint16_t data_size();
static uint16_t cluster_to_lba(uint16_t clusternum);
static uint16_t fat12_get_fat_entry(uint16_t clusternum);
 

static void  fat12_load_bpb();
static void  fat12_set_fat_entry(uint16_t clusternum, uint16_t value);

static int8_t fat12_save_root();
static int8_t fat12_save_fat();
static int8_t fat12_load_root();
static int8_t fat12_load_fat();
static int8_t fat12_load_file_mem(const char *__n, uint16_t offset);
static int8_t fat12_add_new_entry(fat12_entry_t *__e);
int8_t fat12_create_new_file(uint16_t offset, fat12_entry_t *__e);
static int8_t fat12_search_free_clusters(uint16_t *_fcl, uint8_t needed);
/** ==================================================== **/
#endif
