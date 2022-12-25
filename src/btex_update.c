

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

static void free_btex_texture(void *data) {
    struct BTEX_TEXTURE *texture;
    texture = (struct BTEX_TEXTURE *) data;
    (void) btex_texture_destroy(texture);
    (void) free((void *) texture);
    return;
}



int btex_update(const char *texture_root_dir, const char *src_dir) {
    
    int opval, key_pressed;
    int retval, isempty, isnull;
    List directories, textures;
    ListElem *dir_elem, *tex_elem;
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
    
    // Get the list of subdirectories of the texture root directory
    opval = btex_list_directories(texture_root_dir, &directories);
    if (opval == -2) {
        printf("Could not find root directory.\n");
        return -2;
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
    }
}
