
#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


void btex_to_le_4(char *bytes);


void btex_to_le_4(char *bytes) {
    char first, second;
    first = *(bytes + 3);
    second = *(bytes + 2);
    *(bytes + 3) = *(bytes + 0);
    *(bytes + 2) = *(bytes + 1);
    *(bytes + 1) = second;
    *(bytes + 0) = first;
    return;
}


int btex_png_dimension(const char *filename, unsigned int *width, unsigned int *height) {
    
    FILE *image_png;
    unsigned int uwidth, uheight;
    int retval, opval;
    size_t readcount;
    
    if (filename == NULL || width == NULL || height == NULL)
        return -1;
    
    image_png = NULL;
    uwidth = uheight = 0;
    image_png = fopen(filename, "rb");
    if (image_png == NULL)
        return -2;
    
    // Set file pointer before the 17th byte.
    retval = fseek(image_png, 16L, SEEK_SET);
    if (retval != 0) {
        opval = -3;
        goto CLOSE_AND_RETURN;
    }
    
    // Read 4 bytes in big endian as the width of the image.
    readcount = fread((void *) &uwidth, 1, 4, image_png);
    if (readcount < 4) {
        opval = -3;
        goto CLOSE_AND_RETURN;
    }
    
    // Read 4 bytes in big endian as the height of the image.
    readcount = fread((void *) &uheight, 1, 4, image_png);
    if (readcount < 4) {
        opval = -3;
        goto CLOSE_AND_RETURN;
    }
    
    btex_to_le_4((char *) &uwidth);
    btex_to_le_4((char *) &uheight);
    
    *width = uwidth;
    *height = uheight;
    opval = 0;
    
    CLOSE_AND_RETURN:
    fclose(image_png);
    return opval;
}



int btex_list_directories(const char *path, List *directories) {
    
    HANDLE search_hndl;
    WIN32_FIND_DATA search_info;
    ListElem *last_entry;
    DWORD attribute_info;
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
        attribute_info = search_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        cmp_filename_dot = strcmp((const char *) search_info.cFileName, ".");
        cmp_filename_ddot = strcmp((const char *) search_info.cFileName, "..");
        
        // Proceed only if this is a directory and a legitimate one
        if (cmp_filename_dot != 0 && cmp_filename_ddot != 0 && attribute_info != 0) {
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
    DWORD attribute_info;
    char *filename, fullpath[2048];
    unsigned int name_length;
    int opval_nextfind, cmp_filename_dot, cmp_filename_ddot;
    
    if (path == NULL || textures == NULL)
        return -1;
    
    if (directory == NULL || strlen(directory) == 0) {
        sprintf(&fullpath[0], "%s\\*.png", path);
    }
    else {
        sprintf(&fullpath[0], "%s\\%s\\*.png", path, directory);
    }
    
    search_hndl = FindFirstFile(&fullpath[0], &search_info);
    if (search_hndl == INVALID_HANDLE_VALUE)
        return -2;
    
    FIND_TEXTURES:
        attribute_info = search_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        cmp_filename_dot = strcmp((const char *) search_info.cFileName, ".");
        cmp_filename_ddot = strcmp((const char *) search_info.cFileName, "..");
        
        // Proceed only if this is a legitimate texture file (a png image)
        if (cmp_filename_dot != 0 && cmp_filename_ddot != 0 && attribute_info == 0) {
            name_length = strlen(search_info.cFileName);
            filename = (char *) malloc(name_length + 1);
            strcpy(filename, (const char *) search_info.cFileName);
            last_entry = list_tail(textures);
            list_ins_next(textures, last_entry, (const void *) filename);
        }
        opval_nextfind = FindNextFile(search_hndl, &search_info);
        if (opval_nextfind != 0)
            goto FIND_TEXTURES;
    
    FindClose(search_hndl);
    return 0;
}
