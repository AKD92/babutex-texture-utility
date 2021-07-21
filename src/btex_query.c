
#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>



void destroy_btex_texture(void *data);
void get_info_string(char *type, char *compression, const struct BTEX_TEXTURE *texture_info);


void destroy_btex_texture(void *data) {
    
    struct BTEX_TEXTURE *texture_info;
    texture_info = (struct BTEX_TEXTURE *) data;
    (void) btex_texture_destroy(texture_info);
    (void) free(data);
    return;
}


void get_info_string(char *type, char *compression, const struct BTEX_TEXTURE *texture_info) {
    
    const char *t_png, *t_rw;
    const char *c_bgra, *c_dxt, *c_unknown;
    
    c_bgra = "Uncompressed";
    c_dxt = "DXT ";
    c_unknown = "UNKNOWN";
    t_png = "PNG";
    t_rw = "RenderWare";
    
    if (texture_info == NULL)
        return;
    
    if (texture_info->type == BTEX_TYPE_PNG) {
        (void) strcpy(type, t_png );
        (void) strcpy(compression, "");
    }
    else if (texture_info->type == BTEX_TYPE_RW) {
        (void) strcpy(type, t_rw);
        if (texture_info->compression_level == BTEX_COMPRESS_UNCOMPRESSED) {
            (void) strcpy(compression, c_bgra);
        }
        else {
            (void) strcpy(compression, c_dxt);
            compression[strlen(compression) + 1] = '\0';
            compression[strlen(compression)] = (char) (texture_info->compression_level + 0x30);
        }
    }
    else {
        (void) strcpy(type, c_unknown);
    }
    return;
}


int main(void) {
    
    char directory[2047];
    const struct BTEX_TEXTURE *texture_info;
    List textures;
    const ListElem *list_elem;
    int opval, key_pressed;
    unsigned int entry_no;
    char compression_name[20], format_name[20];
    
    printf("Welcome to \"BABUTEX\\QTX\".\n");
    printf("Use this program to view basic texture information.\n");
    
    // Start executing the main portion of this program.
    RUN_PROGRAM:
    
    // Initialize the list which will hold the texture information objects.
    list_init(&textures, destroy_btex_texture);
    printf("Enter directory: ");
    
    // Read directory address from user.
    gets(directory);
    
    // List out all the texture files stored inside the given directory.
    opval = btex_list_textures((const char *) &directory[0], NULL, &textures);
    if (opval != 0) {
        
        // Operation return value is non-zero, meaning we have encountered an error situation.
        printf("Could not read from the directory, error %d.\n", opval);
    }
    else if (list_size(&textures) == 0) {
        
        printf("No texture file was found on the specified directory.\n");
    }
    else {
        
        printf("\n");
        entry_no = 0;
        printf("%4s %50s %13s %10s %12s\n", "No.", "FILE", "DIMENSION", "TYPE", "COMPRESSION");
        
        // Get information object to the first texture file.
        list_elem = list_head(&textures);
        while (list_elem != NULL) {
            
            texture_info = (struct BTEX_TEXTURE *) list_data(list_elem);
            
            // Get string representation of some of the integer values.
            (void) get_info_string((char *) &format_name[0], (char *) &compression_name[0], texture_info);
            
            // Print the information onto the display.
            printf
            (
                 "%4u %50s (%4u x %-4u) %10s %12s\n",
                 entry_no + 1,
                 texture_info->file_name,
                 texture_info->width,
                 texture_info->height,
                 &format_name[0],
                 &compression_name[0]
            );
            
            // Go get the next texture information entry and start over.
            list_elem = list_next(list_elem);
            entry_no += 1;
        }
        printf("%4s %50s %13s %10s %12s\n", "No.", "FILE", "DIMENSION", "TYPE", "COMPRESSION");
    }
    
    // Release system memory occupied by the texture information list.
    list_destroy(&textures);
    
    // Ask user if they want to use this program again. If yes, execute the main portion again.
    ASK_RUN:
        printf("Run this program again? (y/n): ");
        key_pressed = getche();
        printf("\n");
        if (key_pressed == 'y')
            goto RUN_PROGRAM;
        else if (key_pressed != 'n')
            goto ASK_RUN;
    
    return 0;
}
