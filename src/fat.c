#include "sd.h"
#include "fat.h"
#include <string.h>
#include <ctype.h>

struct boot_sector *bs;
char bootSector[512];
char fat_table[8*SECTOR_SIZE];
unsigned int root_sector;

int fatInit() {

    sd_readblock(0, bootSector, 1);
    bs = (struct boot_sector *)bootSector;
    
    if (bs->boot_signature != 0xaa55) {

        return -1;

    }
    
    if (strcmp(bs->fs_type, "FAT12   ") != 0) {

        return -2;

    }

    unsigned int fat_start_sector = bs->num_reserved_sectors;
    unsigned int fat_size_sectors = bs->num_sectors_per_fat;
    sd_readblock(fat_start_sector, fat_table, fat_size_sectors);
    
    root_sector = bs->num_fat_tables * bs->num_sectors_per_fat + 
                  bs->num_reserved_sectors + bs->num_hidden_sectors;
    
    return 0;

}

struct file *fatOpen(const char *filename) {

    struct file *file = malloc(sizeof(struct file));
    if (!file) return NULL;

    char upper_filename[13];
    int i;
    for (i = 0; i < 12 && filename[i]; i++) {

        upper_filename[i] = toupper(filename[i]);

    }

    upper_filename[i] = '\0';

    char root_dir[SECTOR_SIZE];
    sd_readblock(root_sector, root_dir, 1);

    struct root_directory_entry *rde = (struct root_directory_entry *)root_dir;
    for (int i = 0; i < bs->num_root_dir_entries; i++) {

        char entry_name[13];
        strncpy(entry_name, rde[i].file_name, 8);
        entry_name[8] = '.';
        strncpy(entry_name + 9, rde[i].file_extension, 3);
        entry_name[12] = '\0';

        if (strcmp(upper_filename, entry_name) == 0) {

            file->rde = rde[i];
            file->start_cluster = rde[i].cluster;
            return file;

        }

    }

    free(file);
    return NULL;

}

int fatRead(struct file *file, void *buffer, unsigned int size) {

    if (!file || !buffer) return -1;

    unsigned int bytes_read = 0;
    unsigned int current_cluster = file->start_cluster;
    char cluster_data[CLUSTER_SIZE];

    while (bytes_read < size && current_cluster < 0xFF8) {

        unsigned int cluster_sector = root_sector + (current_cluster - 2) * bs->num_sectors_per_cluster;
        sd_readblock(cluster_sector, cluster_data, bs->num_sectors_per_cluster);

        unsigned int bytes_to_copy = (size - bytes_read < CLUSTER_SIZE) ? size - bytes_read : CLUSTER_SIZE;
        memcpy(buffer + bytes_read, cluster_data, bytes_to_copy);
        bytes_read += bytes_to_copy;

        unsigned int fat_offset = current_cluster * 3 / 2;
        unsigned int fat_entry = *(unsigned short*)&fat_table[fat_offset];

        if (current_cluster & 1) {

            current_cluster = fat_entry >> 4;

        } else {

            current_cluster = fat_entry & 0x0FFF;

        }

    }

    return bytes_read;

}

