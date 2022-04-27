

/************************************************************************************
    Implementation of texture merging program "BTEX_MERGE"
    Author:             Ashis Kumar Das
    Email:              akd.bracu@gmail.com
    GitHub:             https://github.com/AKD92
*************************************************************************************/








#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 *  Destroyes the BTEX_TEXTURE object specified on the data parameter
 *  and releases or de-allocates the memory occupied by the texture
 *  object itself, providing that the texture object was dynamically
 *  allocated.
 *
 *  Parameter:
 *      data                :   Pointer to the BTEX_TEXTURE object whichbeing destroyed
 *                              is being destroyed and de-allocated from memory
 *
 *  Returns
 *      Nothing
*/
static void free_btex_texture(void *data);



static void free_btex_texture(void *data) {

    struct BTEX_TEXTURE *texture_info;
    texture_info = (struct BTEX_TEXTURE *) data;
    (void) btex_texture_destroy(texture_info);
    (void) free(data);
    return;
}



// Merge operation
int btex_merge(void) {

    int optval_directory, optval_texture, optval_target_texture, retval;
    int optval_copy;
    List read_dirs, read_textures;
    ListElem *dir_elem, *texture_elem;
    char *subdirectory;
    struct BTEX_TEXTURE *texture_info, target_texture_info;
    char target_texture_name[2048], source_texture_name[2048];
    char src_root_dir[2047], merge_root_dir[2047];
    char logfile_name[1024], outputdir[2047];
    unsigned int side_source, side_target;
    FILE *fp_log;

    // Take input from the user
    printf("Source texture root:\n");
    gets(&src_root_dir[0]);
    printf("Merge destination root:\n");
    gets(&merge_root_dir[0]);
    printf("OPTIONAL Log file name:");
    gets(&logfile_name[0]);
    printf("OPTIONAL Texture output dir:\n");
    gets(&outputdir[0]);

    // Initialize subdirectory list.
    fp_log = NULL;
    list_init(&read_dirs, free);

    if (strlen(&logfile_name[0]) > 0) {
        fp_log = fopen((const char *) &logfile_name[0], "w");
        if (fp_log != NULL) {
            printf("Log file opened: %s\n", &logfile_name[0]);
        }
        else {
            printf("Error opening log file: %s\n", &logfile_name[0]);
            printf("Using screen to dump logs.\n");
            fp_log = stdout;
        }
    }
    else {
        fp_log = stdout;
    }

    // Get all the subdirectories (texture dirs) from the source root dir
    optval_directory = btex_list_directories((const char *) &src_root_dir[0], &read_dirs);
    if (optval_directory != 0) {
        fprintf(fp_log, "Could not read source texture root directory.\n");
        retval = -1;
        goto CLEAN_AND_RETURN;
    }
    if (list_size(&read_dirs) == 0) {
        fprintf(fp_log, "No subdirectory was found in source texture root directory.\n");
        retval = -1;
        goto CLEAN_AND_RETURN;
    }

    fprintf(fp_log, "\n");
    fprintf(fp_log, "%u directories read from the source root.\n", list_size(&read_dirs));

    dir_elem = list_head(&read_dirs);
    while (dir_elem != NULL) {

        list_init(&read_textures, free_btex_texture);
        subdirectory = (char *) list_data(dir_elem);
        optval_texture = btex_list_textures((const char *) &src_root_dir[0], (const char *) subdirectory, &read_textures);

        if (optval_texture == 0) {
            texture_elem = list_head(&read_textures);

            while (texture_elem != NULL) {
                texture_info = (struct BTEX_TEXTURE *) list_data(texture_elem);
                sprintf
                (
                    (char *) &target_texture_name[0],
                    "%s\\%s\\%s",
                    &merge_root_dir[0],
                    subdirectory,
                    texture_info->file_name
                );
                sprintf
                (
                    (char *) &source_texture_name[0],
                    "%s\\%s\\%s",
                    &src_root_dir[0],
                    subdirectory,
                    texture_info->file_name
                );

                btex_texture_init(&target_texture_info);
                optval_target_texture = btex_texture_info((const char *) &target_texture_name[0], &target_texture_info);

                if (optval_target_texture == 0) {
                    side_target = target_texture_info.width + target_texture_info.height;
                    side_source = texture_info->width + texture_info->height;
                    if (side_source > side_target) {

                        // Copy file from source to target.
                        optval_copy = btex_copy((const char *) &source_texture_name[0], (const char *) &target_texture_name[0]);
                    }
                }
                else if (optval_target_texture == -1) {
                    optval_copy = btex_copy((const char *) &source_texture_name[0], (const char *) &target_texture_name[0]);
                }

                btex_texture_destroy(&target_texture_info);
                texture_elem = list_next(texture_elem);
            }
        }
        else {
            fprintf(fp_log, "Could not read textures from %s\\%s\n", &src_root_dir[0], subdirectory);
        }

        list_destroy(&read_textures);
        dir_elem = list_next(dir_elem);
    } // End while

    // Clean up
    CLEAN_AND_RETURN:
    list_destroy(&read_dirs);

    fflush(fp_log);
    if (fp_log != stdout) {
        fclose(fp_log);
    }

    return retval;
}
