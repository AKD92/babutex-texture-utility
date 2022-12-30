

/************************************************************************************
    Implementation of texture update program "BTEX_UPDATE"
    Author:             Ashis Kumar Das
    Email:              akd.bracu@gmail.com
    GitHub:             https://github.com/AKD92
*************************************************************************************/








#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


static void free_btex_texture(void *data);
int compare_texture_by_filename(const void *data1, const void *data2);

static void free_btex_texture(void *data) {
    struct BTEX_TEXTURE *texture;
    texture = (struct BTEX_TEXTURE *) data;
    (void) btex_texture_destroy(texture);
    (void) free((void *) texture);
    return;
}

int compare_texture_by_filename(const void *data1, const void *data2) {
    const struct BTEX_TEXTURE *texture1 = (const struct BTEX_TEXTURE *)data1;
    const struct BTEX_TEXTURE *texture2 = (const struct BTEX_TEXTURE *)data2;
    return strcmpi((const char *)texture1->file_name, (const char *)texture2->file_name);
}



int btex_update(const char *texture_root_dir, const char *src_dir) {
    
    int opval, key_pressed;
    int retval, isempty, isnull;
    List directories, textures, src_textures;
    ListElem *dir_elem, *tex_elem, *src_tex_elem;
    char *directory;
    struct BTEX_TEXTURE *texture;
    char directory_full_path[2048];
    
    isnull = texture_root_dir == NULL || src_dir == NULL;
    isempty = strlen(texture_root_dir) == 0 || strlen(src_dir) == 0;
    
    if (isnull || isempty) {
        return -1;
    }
    
    // Initialize linked lists which will hold subdirectories of texture root dir
    (void) list_init(&directories, free);
    (void) list_init(&src_textures, free_btex_texture);
    
    // Get the list of subdirectories of the texture root directory
    opval = btex_list_directories(texture_root_dir, &directories);
    if (opval == -2) {
        printf("Could not find root directory.\n");
        retval = -2;
        goto CLEANUP;
    }
    
    // Get the texture files which will be copied to the destination (updated)
    opval = btex_list_textures(src_dir, NULL, &src_textures);
    if (opval == -2) {
        printf("Could not find source directory.\n");
        retval = -2;
        goto CLEANUP;
    }
    
    // Iterate over each subdirectory on the root folder
    dir_elem = list_head(&directories);
    while (dir_elem != NULL) {
        
        // Read the current subdirectory
        directory = (char *) list_data(dir_elem);
        
        // Initialize linked list which will hold all the textures
        // which belong to the current subdirectory
        (void) list_init(&textures, free_btex_texture);
        
        // Read all the textures in this subdirectory
        opval = btex_list_textures((const char *) texture_root_dir, (const char *) directory, &textures);
        if (opval != 0) {
            // Error
        }
        else {
                
            // Try each source texture and see if it is on the destination dir by filename
            src_tex_elem = list_head(&src_textures);
            while (src_tex_elem != NULL) {
                opval = list_search(&textures, list_data(src_tex_elem), &tex_elem, compare_texture_by_filename);
                
                // If the source texture matches a destination texture by filename
                // Then we copy it to the destination folder and replace the old one with source
                if (opval == 1) {
                    
                }
            }
        }
    }
    
    CLEANUP:
    list_destroy(&directories);
    list_destroy(&src_textures);
    return retval;
}
