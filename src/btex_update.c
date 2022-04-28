

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



int btex_update(const char *texture_root_dir, const char *src_dir) {
    int opval, key_pressed;
    int retval, isempty, isnull;
    List directories, textures;
    ListElem *dir_elem, *tex_elem;
    char *directory;
    struct BTEX_TEXTURE *texture;
    
    isnull = texture_root_dir == NULL || src_dir == NULL;
    isempty = strlen(texture_root_dir) == 0 || strlen(src_dir) == 0;
    
    if (isnull || isempty) {
        return -1;
    }
    
    // Initialize linked lists which will hold subdirectories
    // and textures inside them
    (void) list_init(&directories, NULL);
    
    // Get the list of subdirectories of the texture root directory
    opval = btex_list_directories(texture_root_path, &directories);
    if (opval == -2) {
        printf("Could not find root directory.\n");
        return -2;
    }
    
    // Iterate over each subdirectory on the root folder
    dir_elem = list_head(&directories);
    while (dir_elem != NULL) {
        
        directory = (char *) list_data(dir_elem);
        
    }
}
