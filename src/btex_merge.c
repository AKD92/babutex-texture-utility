

#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int btex_merge(void) {
    
    int optval, retval;
    List read_dirs, read_textures;
    ListElem *dir_elem, *texture_elem;
    char src_root_dir[2047], merge_root_dir[2047];
    char logfilename[1024], outputdir[2047];
    
    // Take input from the user
    printf("Source texture root:\n");
    gets(&src_root_dir[0]);
    printf("Merge destination root:\n");
    gets(&merge_root_dir[0]);
    printf("OPTIONAL Log file name:");
    gets(&logfilename[0]);
    printf("OPTIONAL Texture output dir:\n");
    gets(&outputdir[0]);
    
    // Initialize directory and texture lists
    list_init(&read_dirs, free);
    list_init(&read_textures, free);
    
    // Get all the subdirectories (texture dirs) from the source root dir
    optval = btex_list_directories((const char *) &src_root_dir[0], &read_dirs);
    if (optval != 0) {
        printf("Could not read directories from source texture root.\n");
        retval = -1;
        goto CLEAN_AND_RETURN;
    }
    
    printf("\n");
    printf("%u directories read from the source root.", list_size(&read_dirs));
    
    dir_elem = list_head(&read_dirs);
    while (dir_elem != NULL) {
        
        
    }
    
    CLEAN_AND_RETURN:
    list_destroy(&read_dirs);
    list_destroy(&read_textures);
    return retval;
}
