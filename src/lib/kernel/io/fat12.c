#include <kernel/io/fat12.h>
/** ==================================================== **/
// API
int8_t init_fs(){
    int8_t return_code;
    fat12_load_bpb();
    if( (return_code = fat12_load_fat()) > 0)
        return return_code;
    if( (return_code = fat12_load_root()) > 0)
        return return_code;
    return DISK_OP_OK;
}

int8_t load_file(const char *__s, file_t *_fhandle){
    fat12_entry_t _ent;
    static file_t _fbuff_;
    char name[11];
    int8_t return_code;
    int16_t offset = 0x0000;
    
    if( _file_handle != NULL )
        return DISK_FSLOT_NON_EMPTY;
    
    if((return_code = normal_to_fat12(__s, name)) > 0){
        return return_code;
    }

    
    if(get_entry_by_name(name, &_ent))
    {
        if( (return_code = fat12_load_file_mem(_ent.filename, offset)) > 0)
            return return_code;
        
        else
        {
            _fbuff_.entry = _ent;
            _fbuff_.data  = DATA_OFFSET + offset;
            _file_handle = &_fbuff_;
            (*_fhandle) = _fbuff_;
            return DISK_OP_OK;
        }
    
    }   
    else
        return DISK_FILE_NOEXIST;
}

int8_t create_file(const char *__s, uint16_t offset, uint16_t size){
    fat12_entry_t _ent;
    int8_t rc;
    char name[11]; 
    
    if(size == 0)
        return DISK_FILE_SIZE_IS_ZERO;
    
    if( (rc = normal_to_fat12(__s, name)) > 0)
        return rc;
    
    if(get_entry_by_name(name, &_ent))
        return DISK_ENTRY_OVERWRITE;
    
    memcpy( (char*)&_ent, name, 11);
    _ent.file_size = size;
    
    if(( rc = fat12_create_new_file(offset, &_ent, false)) > 0)
        return rc;
    
    return DISK_OP_OK;
}

int8_t delete_file(const char *__s){
    int8_t rc;
    char name[11];
    
    if( (rc = normal_to_fat12(__s, name)) > 0)
        return rc;
    if( (rc = fat12_delete_file(name)) > 0)
        return rc;
    
    return DISK_OP_OK;
}

int8_t get_entry_by_name(const char* __s, fat12_entry_t *__e){
    uint16_t i=0;
    for(;i<3;++i){
        fat12_entry_t _tmp = entries[i];
        if(strncmp(_tmp.filename, __s, 11) == 0){
            *__e = _tmp;
            return true;
        }
    }
    return false;
}

int8_t normal_to_fat12(const char* _n, char* _b){
    static char bn[8];
    static char be[3];
    int16_t posdot, cntdot, i, i2;
    memset(bn, 0, 8); memset(be, 0, 8); memset(_b, ' ', 11);
    posdot = strpos(_n, '.');
    cntdot = strcnt(_n, '.');
    if(cntdot != 1)
        return DISK_MORE_DOTS;
    if(posdot == -1)
        return DISK_NOT_DOT;
    if(posdot == 0)
        return DISK_DOT_FIRST;
    if(posdot > 7)
        return DISK_DOT_PLUS_EIGHT;
    
    memcpy( bn, _n, posdot);
    memcpy( be, _n + posdot +1, 3);
    
    if(strcnt(bn, ' '))
        return DISK_SPACE_IN_FILENAME;
    if(strcnt(be, ' '))
        return DISK_SPACE_IN_EXT;
    if(strlen(be) != 3)
        return DISK_EXT_NOT_THREE;
        
    i2 = 0;
    for(i=0;i<8;++i) _b[i] = bn[i];
    for(i=0;i<8;++i) if(_b[i] == 0) _b[i] = ' ';
    for(i=8;i<11;++i)_b[i] = be[i2++];
    _b[11] = 0;
    return DISK_OP_OK;
}

void print_bpb(){

    puts_attrib("=================BPB=================", color_entry(COLOR_GREEN, COLOR_BLACK));
    eol();
    /**  **/
    puts_attrib("OEM:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    puts_attrib(_bpb.oem_name, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    /**  **/
    puts_attrib("Bajty na sektor:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    print_int(_bpb.bytes_per_sector,10, color_entry(COLOR_RED, COLOR_BLACK)); eol();    
    /**  **/
    puts_attrib("Sektory na klaster:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    print_int(_bpb.sectors_per_cluster,10, color_entry(COLOR_RED, COLOR_BLACK)); eol(); 
    /**  **/
    puts_attrib("Zarezerwowane:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    print_int(_bpb.reserved_sectors,10, color_entry(COLOR_RED, COLOR_BLACK)); eol();    
    /**  **/
    puts_attrib("Tablice FAT:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    print_int(_bpb.number_of_fats,10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    /**  **/
    puts_attrib("Max plkow:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    print_int(_bpb.max_files,10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    /**  **/
    puts_attrib("MEDIA:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    print_int(_bpb.media_sign,16, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    /**  **/
    puts_attrib("Sektory na track:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    print_int(_bpb.sectors_per_track,10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    /**  **/
    puts_attrib("HPC:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    print_int(_bpb.heads_per_cylinder,10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    /**  **/
    puts_attrib("Label:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    puts_attrib(_bpb.volume_label, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    /**  **/
    puts_attrib("FS Label:", color_entry(COLOR_BROWN, COLOR_BLACK)); 
    puts_attrib(_bpb.file_system, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("=================BPB=================",color_entry(COLOR_GREEN, COLOR_BLACK));eol();
}

void print_lba(){
    puts_attrib("================DDATA================",color_entry(COLOR_MAGENTA, COLOR_BLACK));
    eol();
    puts_attrib("FAT LBA:", color_entry(COLOR_BROWN, COLOR_BLACK));
    print_int(fat_lba(), 10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("Root LBA:", color_entry(COLOR_BROWN, COLOR_BLACK));
    print_int(root_lba(), 10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("Data LBA:", color_entry(COLOR_BROWN, COLOR_BLACK));
    print_int(data_lba(), 10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("FAT Size:", color_entry(COLOR_BROWN, COLOR_BLACK));
    print_int(fat_size(), 10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("Root Size:", color_entry(COLOR_BROWN, COLOR_BLACK));
    print_int(root_size(), 10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("Data Size:", color_entry(COLOR_BROWN, COLOR_BLACK));
    print_int(data_size(), 10, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("================DDATA================",color_entry(COLOR_GREEN, COLOR_BLACK));eol();
}

void print_offsets(){
    puts_attrib("=================OFF=================", color_entry(COLOR_MAGENTA, COLOR_BLACK)); eol();
    
    puts_attrib("FAT Offset:", color_entry(COLOR_GREEN, COLOR_BLACK));
    print_int(FAT_OFFSET, 16, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("Root Offset:", color_entry(COLOR_GREEN, COLOR_BLACK));
    print_int(ROOT_OFFSET , 16, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    puts_attrib("Data Offset:", color_entry(COLOR_GREEN, COLOR_BLACK));
    print_int(DATA_OFFSET, 16, color_entry(COLOR_RED, COLOR_BLACK)); eol();
    
    puts_attrib("=================OFF=================",color_entry(COLOR_MAGENTA, COLOR_BLACK)); eol();
}

void print_files(){
    uint16_t i=0;
    uint8_t total = 0;
    for(;i<_bpb.max_files;++i){
        fat12_entry_t _tmp = entries[i];
        if(_tmp.filename[0] == FAT12_ENTRY_FREE || _tmp.filename[0] == FAT12_ENTRY_LAST)
            continue;
        else{
            print_entry(&_tmp);
            eol();
            total++;
        }
    }
    puts_attrib("Total:", color_entry(COLOR_RED, COLOR_BLACK));
    print_int(total, 10, color_entry(COLOR_MAGENTA, COLOR_BLACK));
    eol();
}

void print_cluster_list(uint16_t clusternum){
    while(clusternum != FAT12_END_OF_CLUSTERS){
        print_int(clusternum, 10, 0); putc(' ');
        clusternum = fat12_get_fat_entry(clusternum);
    }
    eol();
}

void print_entry(fat12_entry_t *__e){
    puts_attrib("N:", color_entry(COLOR_RED, COLOR_BLACK));
    puts_attrib(__e->filename, color_entry(COLOR_MAGENTA, COLOR_BLACK));
    putc(' ');
    puts_attrib("S:", color_entry(COLOR_RED, COLOR_BLACK));
    print_int(__e->file_size, 10, color_entry(COLOR_MAGENTA, COLOR_BLACK));
    putc(' ');
    puts_attrib("F:", color_entry(COLOR_RED, COLOR_BLACK));
    print_int(__e->first_cluster, 10, color_entry(COLOR_MAGENTA, COLOR_BLACK));
}

void free_file(){
    _file_handle = NULL;
}

uint16_t get_data_offset(){
    return DATA_OFFSET;
}

/** ==================================================== **/
// Internalz.
static int8_t floppy_rw_sectors(uint16_t offset_, int16_t lba, uint16_t count, uint8_t read){
    _asm
    {
        mov ax, RAM_SEGMENT
        mov es, ax
    }
    while(count--)
    {
        char c,h,s, rcode;
        c =       lba / ( _bpb.sectors_per_track * _bpb.heads_per_cylinder ); 
        h =     ( lba / _bpb.sectors_per_track ) % _bpb.heads_per_cylinder;
        s =     ( lba % _bpb.sectors_per_track ) + 1;
        if(read)
        {
            _asm
            {
                mov ah, 0x02       
                mov al, 0x01        
                mov ch, c           
                mov dh, h
                mov cl, s
                mov dl, 0           
                mov bx, offset_    
                int 0x13 
                mov rcode, ah
            }
            if(rcode != 0x00)
            {
                puts("[KERNEL] Disk read errcode:"); print_int(rcode, 16, 0); eol();
                return rcode;
            }
        }
        if(!read)
        {
            _asm
            {
                mov ah, 0x03       
                mov al, 0x01       
                mov ch, c          
                mov dh, h
                mov cl, s
                mov dl, 0           
                mov bx, offset_     
                int 0x13 
                mov rcode, ah
            }   
            if(rcode != 0x00)
            {
                puts("[KERNEL] Disk write errcode:"); print_int(rcode, 16, 0); eol();
                return rcode;
            }
        }
        offset_ += _bpb.bytes_per_sector;
        lba++; 
    }
    _asm
    {
        mov ax, cs
        mov es, ax
    }
    return DISK_OP_OK;
}

static int8_t floppy_reset_drive(){
    int16_t flags_reg;
    _asm{
        mov ah, 0x00
        mov al, 0x00 
        int 0x13
        pushf
        pop flags_reg
    }
    if(CHECK_BIT(flags_reg, CARRY_FLAG))
        return DISK_UNABLE_RESET;
    return DISK_OP_OK;
}

static uint16_t fat_lba(){
    return _bpb.reserved_sectors;
}

static uint16_t root_lba(){
    return fat_lba() + (_bpb.number_of_fats * _bpb.sectors_per_fat);
}

static uint16_t data_lba(){
    return root_lba() + ((_bpb.max_files * ENTRY_SIZE) / _bpb.bytes_per_sector);
}

static uint16_t fat_size(){
    return _bpb.number_of_fats * _bpb.sectors_per_fat;
}

static uint16_t root_size(){
    return (_bpb.max_files * ENTRY_SIZE) / _bpb.bytes_per_sector;
}

static uint16_t data_size(){
    return (_bpb.total_sectors - (_bpb.reserved_sectors + fat_size() + root_size())) / _bpb.sectors_per_cluster;
}

static uint16_t cluster_to_lba(uint16_t clusternum){
    return data_lba() + clusternum - 2;
}

static uint16_t fat12_get_fat_entry(uint16_t clusternum){
    uint32_t offset;
    uint16_t value;
    uint8_t b1, b2;
    offset =  (3 * (clusternum/2));
    switch(clusternum % 2) 
    {
        case 0:
            b1 = *(fat + offset);
            b2 = *(fat + offset + 1);
            value = ((0x0f & b2) << 8) | b1;
        break;
        case 1:
            b1 = *(fat + offset + 1);
            b2 = *(fat + offset + 2);
            value = b2 << 4 | ((0xf0 & b1) >> 4);
        break;
    }
    return value;
}

static void fat12_load_bpb(){
    short  c, h, s, i;
    char err;
    char* fat12_bpb_ptr = ((char*)&_bpb);
    __segment fat12_bpb_seg = 0x07e0;
    char __based(fat12_bpb_seg) *bpbptr = 0x03; // Skip JMP opcode
    _asm
    {
        mov ax, RAM_SEGMENT
        mov es, ax
        mov ah, 0x02
        mov al, 0x01
        mov ch, 0
        mov dh, 0
        mov cl, 1
        mov dl, 0
        mov bx, 0x0000
        int 0x13
        mov err, ah
    }
    if(err != 0x0) fatal_error_box(errmesg("FAT12 BPB Read error:", err));
    for(i=0;i<sizeof(fat12_bpb_t);++i) fat12_bpb_ptr[i] = bpbptr[i];
    _asm{
        mov ax, cs
        mov es, ax
    }
    ROOT_OFFSET = (fat_size() * _bpb.bytes_per_sector);
    DATA_OFFSET = ROOT_OFFSET + (root_size() * _bpb.bytes_per_sector);
}

static void fat12_set_fat_entry(uint16_t clusternum, uint16_t value){
    uint32_t offset;
    char __based(RAMSEG) *p1;
    char __based(RAMSEG) *p2;
    offset = (3 * (clusternum/2));
    switch(clusternum % 2) 
    {
        case 0:
            p1 = fat + offset;
            p2 = fat + offset + 1;
            *p1 = (uint8_t)(0xff & value);
            *p2 = (uint8_t)((0xf0 & (*p2)) | (0x0f & (value >> 8)));
        break;
        case 1:
            p1 = fat + offset + 1;
            p2 = fat + offset + 2;
            *p1 = (uint8_t)((0x0f & (*p1)) | ((0x0f & value) << 4));
            *p2 = (uint8_t)(0xff & (value >> 4));
        break;
    }
}

static int8_t  fat12_save_root(){
    if(floppy_rw_sectors(ROOT_OFFSET, root_lba(), root_size(), IO_WRITE) > 0)
        return DISK_ROOT_SAVE_FAIL;
    return DISK_OP_OK;
}

static int8_t  fat12_save_fat(){
    if(floppy_rw_sectors(FAT_OFFSET, fat_lba(), fat_size(), IO_WRITE) > 0)
        return DISK_FAT_SAVE_FAIL;
    return DISK_OP_OK;
}

static int8_t fat12_load_root(){
    if(floppy_rw_sectors(ROOT_OFFSET, root_lba(), root_size(), IO_READ) > 0)
        return DISK_ROOT_LOAD_FAIL;
    entries = ROOT_OFFSET;
    return DISK_OP_OK;
}

static int8_t fat12_load_fat(){
    if(floppy_rw_sectors(FAT_OFFSET, fat_lba(), fat_size(), IO_READ) > 0)
        return DISK_FAT_LOAD_FAIL;
    fat = FAT_OFFSET;
    return DISK_OP_OK;
}

static int8_t fat12_load_file_mem(const char *__n, uint16_t offset){
    fat12_entry_t ent;
    if(get_entry_by_name(__n, &ent)){
        uint16_t cluster_index = ent.first_cluster;
        int8_t rcode;
        while(cluster_index != FAT12_END_OF_CLUSTERS){
            if( (rcode = floppy_rw_sectors(DATA_OFFSET + offset, cluster_to_lba(cluster_index), 1, IO_READ)) > 0)
                return rcode;
            offset += _bpb.bytes_per_sector;
            cluster_index = fat12_get_fat_entry(cluster_index);
        }
        return DISK_OP_OK;
    }
    else 
        return DISK_FILE_NOEXIST;
}

static int8_t fat12_add_new_entry(fat12_entry_t *__e){
    uint8_t i=0;
    for(;i<_bpb.max_files;++i){
        fat12_entry_t _tmp = entries[i];
        if(strcmp(_tmp.filename, __e->filename) == 0)
            return DISK_ENTRY_OVERWRITE;
        if(_tmp.filename[0] == FAT12_ENTRY_FREE || _tmp.filename[0] == FAT12_ENTRY_LAST){
            entries[i] = *__e;
            return DISK_OP_OK;
        }
    }
    return DISK_NO_SPC_FOR_ENT;
}

static int8_t fat12_delete_file(const char *__n){
    uint8_t j = 0, i = 0;
    for(;i<_bpb.max_files;++i){
        fat12_entry_t _tmp = entries[i];
        /** Ok, file exists **/
        if(strncmp(_tmp.filename, __n, 11) == NULL){
            uint16_t f_c = _tmp.first_cluster;
            uint16_t c_c = 0, c_i = 15, c_next;
            /** Filename to 0xE5 **/
            for(j=0;j<32;++j)
                entries[i].filename[j] = FAT12_ENTRY_FREE;
            while(f_c != FAT12_END_OF_CLUSTERS)
            {
                /** Delete clusters from FAT but not zero clusters **/
                c_next = fat12_get_fat_entry(f_c);
                #ifdef TINYDEBUG
                    puts("Dla "); print_int(f_c, 10, 0); 
                    puts(" Ustawiam 0"); eol();
                #endif
                fat12_set_fat_entry(f_c, FAT12_CLUSTER_EMPTY);
                f_c = c_next; 
            }
            fat12_save_fat();
            fat12_save_root();
            return DISK_OP_OK;
        }
    }
    return DISK_FILE_NOEXIST;
}

static int8_t fat12_create_new_file(uint16_t offset, fat12_entry_t *__e, uint8_t forcewrite){
    int8_t rcode, i, list_cnt = 0;
    static uint16_t free_list[32]; // 16kb
    uint16_t clusters_needed;
    
    /** If file size is zero then not **/
    if( (__e->file_size == 0) )
        return DISK_FILE_SIZE_IS_ZERO;
        
    /** Clear clusterz **/
    for(;i<32;++i) free_list[i] = 0x00;
    
    /** Calculate needed clusters for allocation **/
    clusters_needed = (uint16_t) __e->file_size / _bpb.bytes_per_sector;
    if( (int) __e->file_size % _bpb.bytes_per_sector > 0 )
        clusters_needed++;
    
    puts("Need:"); print_int(clusters_needed, 10, 0);eol();
    /** Search clusters in FAT **/
    list_cnt = fat12_search_free_clusters(free_list, clusters_needed);
    if(list_cnt != clusters_needed)
        return DISK_NO_ENOUGH_SPACE;
        
    /** Set last cluster in table to EOC Mark **/
    free_list[list_cnt] = FAT12_END_OF_CLUSTERS;
    
    /** Ok, we have enough clusters to save file **/
    __e->first_cluster = free_list[0];
    if( (rcode = fat12_add_new_entry(__e)) > 0 && forcewrite == false)
            return rcode;
    i=0;
    for(;i<clusters_needed + 1;++i){
        if(free_list[i] == FAT12_END_OF_CLUSTERS) break;
        #ifdef TINYDEBUG
            puts("Dla "); print_int(free_list[i], 10, 0);
            puts(" Ustaw "); print_int(free_list[i+1], 10, 0);
            eol();
        #endif
        fat12_set_fat_entry(free_list[i], free_list[i+1]);
        if( (rcode = floppy_rw_sectors(DATA_OFFSET + offset, cluster_to_lba(free_list[i]), 1, IO_WRITE)) > 0){
            /** If error, clear fucked clusters **/
            for(;i<clusters_needed + 1;++i){
                if(free_list[i] == FAT12_END_OF_CLUSTERS) break;
                fat12_set_fat_entry(free_list[i], FAT12_FREE_CLUSTER);
            }
            /** Und exit with BIOS errcode **/
            return rcode;
        }
        offset += _bpb.bytes_per_sector;
    }
    fat12_save_fat();
    fat12_save_root();
    return DISK_OP_OK;
}

static int8_t fat12_search_free_clusters(uint16_t *_fcl, uint8_t needed){
    uint16_t i=2;
    int8_t free=0;
    for(;i< fat_size() * _bpb.bytes_per_sector;++i){
        if(free == needed) return free;
        if(fat12_get_fat_entry(i) == FAT12_FREE_CLUSTER)
            _fcl[free++] = i;
    }
    return free;
}
