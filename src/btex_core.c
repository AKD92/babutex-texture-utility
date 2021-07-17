
#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


int btex_littleendian_4byte(char *bytes);



int btex_texture_init(struct BTEX_TEXTURE *texture_data) {
    
    if (texture_data == NULL)
        return -1;
    
    memset((void *) texture_data, 0, sizeof(struct BTEX_TEXTURE));
    return 0;
}


void btex_texture_destroy(struct BTEX_TEXTURE *texture_data) {

    if (texture_data == NULL)
        return;
    
    free((void *) texture_data->file_name);
    free((void *) texture_data->file_path);
    return;
}



int btex_littleendian_4byte(char *bytes) {
    char first, second;
    
    if (bytes == NULL)
        return -1;
    
    first = *(bytes + 3);
    second = *(bytes + 2);
    *(bytes + 3) = *(bytes + 0);
    *(bytes + 2) = *(bytes + 1);
    *(bytes + 1) = second;
    *(bytes + 0) = first;
    return 0;
}



int btex_texture_info(const char *filename, struct BTEX_TEXTURE *texture) {
    
    int retval, cmpval_png, cmpval_rwtex, opval, cmpval_cmpress;
    FILE *fp_texture;
    char *sig_png, *sig_rwtex, *comp_rwtex;
    char magic_number[4];
    unsigned int readcount;
    unsigned int uwidth, uheight, comp_level;
    
    fp_texture = NULL;
    sig_png = "\x89\x50\x4E\x47";
    sig_rwtex = "\x15\x00\x00\x00";
    comp_rwtex = "\x44\x58\x54";
    
    fp_texture = fopen(filename, "rb");
    if (fp_texture == NULL) {
        retval = -1;
        goto CLOSE_AND_RETURN;
    }
    
    readcount = fread((void *) &magic_number[0], 1, 4, fp_texture);
    if (readcount < 4) {
        retval = -1;
    }
    else {
        uwidth = uheight = 0;
        cmpval_png = memcmp((const void *) &magic_number[0], (const void *) sig_png, 4);
        cmpval_rwtex = memcmp((const void *) &magic_number[0], (const void *) sig_rwtex, 4);
        
        if (cmpval_png == 0) {
            
            // Set file pointer before the 17th byte.
            opval = fseek(fp_texture, 16L, SEEK_SET);
            if (opval != 0) {
                retval = -2;
                goto CLOSE_AND_RETURN;
            }
            
            // Read 4 bytes in big endian as the width of the image.
            readcount = fread((void *) &uwidth, 1, 4, fp_texture);
            if (readcount < 4) {
                retval = -2;
                goto CLOSE_AND_RETURN;
            }
            
            // Read 4 bytes in big endian as the height of the image.
            readcount = fread((void *) &uheight, 1, 4, fp_texture);
            if (readcount < 4) {
                retval = -2;
                goto CLOSE_AND_RETURN;
            }
            
            (void) btex_littleendian_4byte((char *) &uwidth);
            (void) btex_littleendian_4byte((char *) &uheight);
            texture->width = uwidth;
            texture->height = uheight;
            texture->type = BTEX_TYPE_PNG;
            retval = 0;
        }
        else if (cmpval_rwtex == 0) {
            
            // Set the file pointer before 105th byte.
            opval = fseek(fp_texture, 104L, SEEK_SET);
            if (opval != 0) {
                retval = -2;
                goto CLOSE_AND_RETURN;
            }
            
            // Read 2 bytes in little endian as the width of the texture.
            readcount = fread((void *) &uwidth, 1, 2, fp_texture);
            if (readcount < 2) {
                retval = -2;
                goto CLOSE_AND_RETURN;
            }
            
            // Read 2 byte in little endian as the height of the texture.
            readcount = fread((void *) &uheight, 1, 2, fp_texture);
            if (readcount < 2) {
                retval = -2;
                goto CLOSE_AND_RETURN;
            }
            
            texture->width = uwidth;
            texture->height = uheight;
            
            // Read compression level of the renderware texture.
            readcount = fread((void *) &magic_number[0], 1, 4, fp_texture);
            if (readcount < 4) {
                retval = -2;
                goto CLOSE_AND_RETURN;
            }
            
            cmpval_cmpress = memcmp((const void *) &magic_number[0], (const void *) comp_rwtex, 3);
            if (cmpval_cmpress != 0) {
                texture->compression_level = BTEX_COMPRESS_UNKNOWN;
            }
            else {
                comp_level = ((unsigned int )magic_number[3]) - 30;
                texture->compression_level = comp_level;
            }
            texture->type = BTEX_TYPE_RWTEX;
            retval = 0;
        }
        else {
            retval = -1;
        }
    }
    
    CLOSE_AND_RETURN:
    fclose(fp_texture);
    return retval;
}



int btex_list_directories(const char *path, List *directories) {
    
    HANDLE search_hndl;
    WIN32_FIND_DATA search_info;
    ListElem *last_entry;
    DWORD is_directory;
    char *filename, fullpath[2048];
    unsigned int name_length;
    int opval_nextfind, cmp_filename_dot, cmp_filename_ddot;
    
    if (path == NULL || directories == NULL)
        return -1;
    
    sprintf(&fullpath[0], "%s\\*", path);
    search_hndl = FindFirstFile(&fullpath[0], &search_info);
    if (search_hndl == INVALID_HANDLE_VALUE)
        return -2;
    
    FIND_DIRECTORIES:
        is_directory = search_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        cmp_filename_dot = strcmp((const char *) search_info.cFileName, ".");
        cmp_filename_ddot = strcmp((const char *) search_info.cFileName, "..");
        
        // Filter out unnecessary file system objects.
        if (cmp_filename_dot != 0 && cmp_filename_ddot != 0 && is_directory != 0) {
            name_length = strlen(search_info.cFileName);
            filename = (char *) malloc(name_length + 1);
            strcpy(filename, (const char *) search_info.cFileName);
            last_entry = list_tail(directories);
            list_ins_next(directories, last_entry, (const void *) filename);
        }
        opval_nextfind = FindNextFile(search_hndl, &search_info);
        if (opval_nextfind != 0)
            goto FIND_DIRECTORIES;
    
    FindClose(search_hndl);
    return 0;
}



int btex_list_textures(const char *path, const char *directory, List *textures) {
    
    HANDLE search_hndl;
    WIN32_FIND_DATA search_info;
    ListElem *last_entry;
    DWORD is_directory;
    char search_string[2000], fullname[2300];
    int opval_texture, opval_nextfind, cmp_filename_dot, cmp_filename_ddot;
    struct BTEX_TEXTURE *texture, dummy_tex;
    
    if (path == NULL || textures == NULL)
        return -1;
    
    if (directory == NULL || strlen(directory) == 0) {
        sprintf(&search_string[0], "%s\\*", path);
    }
    else {
        sprintf(&search_string[0], "%s\\%s\\*", path, directory);
    }
    
    search_hndl = FindFirstFile(&search_string[0], &search_info);
    search_string[strlen(&search_string[0]) - 1] = '\0';
    
    if (search_hndl == INVALID_HANDLE_VALUE)
        return -2;
    
    FIND_TEXTURES:
        is_directory = search_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        cmp_filename_dot = strcmp((const char *) search_info.cFileName, ".");
        cmp_filename_ddot = strcmp((const char *) search_info.cFileName, "..");
        
        // Filter out unnecessary file system objects such as subdirectories.
        if (cmp_filename_dot != 0 && cmp_filename_ddot != 0 && is_directory == 0) {
            
            // Initialize our dummy texture info object.
            btex_texture_init(&dummy_tex);
            
            // Get information on the current texture file.
            // 'fullname' is the string containing full path & name of the current texture file.
            sprintf(&fullname[0], "%s\\%s", &search_string[0], search_info.cFileName);
            opval_texture = btex_texture_info((const char *) &fullname[0], &dummy_tex);
            
            // Check whether we've found a valid texture file or not.
            if (opval_texture == 0) {
                
                // We have got a valid texture.
                // Now store the data from our dummy texture info object to the actual texture info object.
                texture = (struct BTEX_TEXTURE *) malloc(sizeof(struct BTEX_TEXTURE));
                texture->type = dummy_tex.type;
                texture->width = dummy_tex.width;
                texture->height = dummy_tex.height;
                texture->compression_level = dummy_tex.compression_level;
                texture->file_path = (char *) malloc(strlen((const char *) &fullname[0]) + 1);
                texture->file_name = (char *) malloc(strlen(search_info.cFileName) + 1);
                strcpy(texture->file_name, (const char *) search_info.cFileName);
                strcpy(texture->file_path, (const char *) &fullname[0]);
                
                // Add this texture info object to the output list.
                last_entry = list_tail(textures);
                list_ins_next(textures, last_entry, (const void *) texture);
            }
            
            // Erase all data stored to dummy texture info object.
            btex_texture_destroy(&dummy_tex);
        }
        opval_nextfind = FindNextFile(search_hndl, &search_info);
        if (opval_nextfind != 0)
            goto FIND_TEXTURES;
    
    FindClose(search_hndl);
    return 0;
}



int btex_copy(const char *source_file, const char *dest_file) {

    int retval;
    FILE *fp_source, *fp_destination;
    unsigned char buffer[2048];
    unsigned int readcount;
    
    if (source_file == NULL || dest_file == NULL)
        return -1;
    if (strlen(source_file) == 0 || strlen(dest_file) == 0)
        return -1;
    
    fp_source = fp_destination = NULL;
    fp_source = fopen(source_file, "rb");
    fp_destination = fopen(dest_file, "wb");
    
    if (fp_source == NULL || fp_destination == NULL) {
        retval = -2;
        goto CLOSE_AND_RETURN;
    }
    
    while (feof(fp_source) == 0) {
        readcount = fread((void *) &buffer[0], 1, sizeof(buffer), fp_source);
        fwrite((const void *) &buffer[0], 1, readcount, fp_destination);
        fflush(fp_destination);
    }
    retval = 0;
    
    CLOSE_AND_RETURN:
    fclose(fp_source);
    fclose(fp_destination);
    return retval;
}
