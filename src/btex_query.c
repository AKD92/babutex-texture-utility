
#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>



void destroy_btex_texture(void *data);


void destroy_btex_texture(void *data) {
    
    struct BTEX_TEXTURE *texture_info;
    texture_info = (struct BTEX_TEXTURE *) data;
    btex_texture_destroy(texture_info);
    return;
}


int main(void) {
    
    char directory[2047];
    struct BTEX_TEXTURE *texture_info;
    List textures;
    ListElem *list_elem;
    int opval, key_pressed;
    unsigned int counter;
    char *t_png, *t_rw;
    char *c_bgra, *c_dxt, *c_unknown;
    char compression[20];
    char *ttype;
    
    c_bgra = "BGRA";
    c_dxt = "DXT";
    c_unknown = "UNKNOWN";
    t_png = "PNG";
    t_rw = "RenderWare";
    
    printf("Welcome to PNG Dimension listing program.\n");
    printf("This tool is part of the \"BABUTEX\" utility.\n");
    
    // Start executing the main portion of this program.
    RUN_PROGRAM:
    
    // Initialize the list which will hold the names of the files.
    list_init(&textures, destroy_btex_texture);
    printf("\nEnter directory: ");
    
    // Read directory address from user.
    gets(directory);
    
    // Make a list of all the png files stored inside the given directory.
    opval = btex_list_textures((const char *) &directory[0], NULL, &textures);
    if (opval != 0) {
        
        // Operation return value is non-zero, meaning we have encountered an error situation.
        printf("Could not get any PNG file from the directory, error %d.\n", opval);
    }
    else if (list_size(&textures) == 0) {
        printf("No texture file was found on the specified directory.\n");
    }
    else {
        printf("\n");
        counter = 0;
        
        printf("%4s %50s %-13s %-10s %-7s\n", "No.", "FILE", "DIMENSION", "TYPE", "COMPRESSION");
        
        // Get the first png image file from the file list.
        list_elem = list_head(&textures);
        while (list_elem != NULL) {
            
            // Get the name of the png file from the list.
            texture_info = (struct BTEX_TEXTURE *) list_data(list_elem);
            
            if (texture_info->type == BTEX_TYPE_PNG) {
                ttype = t_png;
                strcpy((char *) &compression[0], "");
            }
            else if (texture_info->type == BTEX_TYPE_RWTEX) {
                ttype = t_rw;
                if (texture_info->compression_level == BTEX_COMPRESS_UNKNOWN) {
                    strcpy((char *) &compression[0], c_bgra);
                }
                else {
                    strcpy((char *) &compression[0], c_dxt);
                    compression[strlen(&compression[0]) + 1] = '\0';
                    compression[strlen(&compression[0])] = (char) (texture_info->compression_level + 0x30);
                }
            }
            else {
                ttype = c_unknown;
            }
            
            // Print the information into the display.
            printf
            (
             "%4u %50s (%-4u x %-4u) %-10s %-7s\n",
             counter + 1,
             texture_info->file_name,
             texture_info->width,
             texture_info->height,
             ttype,
             &compression[0]
            );
            
            // Go to the next png file in the file list.
            list_elem = list_next(list_elem);
            counter += 1;
        }
    }
    
    // Release system memory occupied by the file list.
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
