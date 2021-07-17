
#include "btex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


int main(void) {
    
    char *filename;
    char directory[2047], fullpath[2048];
    unsigned int width, height;
    List png_files;
    ListElem *list_elem;
    int opval, key_pressed;
    unsigned int counter;
    
    printf("Welcome to PNG Dimension listing program.\n");
    printf("This tool is part of the \"BABUTEX\" utility.\n");
    
    // Start executing the main portion of this program.
    RUN_PROGRAM:
    
    // Initialize the list which will hold the names of the files.
    list_init(&png_files, free);
    printf("\nEnter directory: ");
    
    // Read directory address from user.
    gets(directory);
    
    // Make a list of all the png files stored inside the given directory.
    opval = btex_list_textures((const char *) &directory[0], NULL, &png_files);
    if (opval != 0) {
        
        // Operation return value is non-zero, meaning we have encountered an error situation.
        printf("Could not get any PNG file from the directory, error %d.\n", opval);
    }
    else {
        printf("\n");
        counter = 0;
        
        // Get the first png image file from the file list.
        list_elem = list_head(&png_files);
        while (list_elem != NULL) {
            
            // Get the name of the png file from the list.
            filename = (char *) list_data(list_elem);
            
            // Make a full address of the png file with its name and directory.
            sprintf(&fullpath[0], "%s\\%s", &directory[0], filename);
            
            // Query the dimension (width and height) of the current png image.
            opval = btex_png_dimension((const char *) &fullpath[0], &width, &height);
            if (opval != 0) {
                printf("ERRORN on file: %s\n\n", &fullpath[0]);
            }
            else {
                
                // Print the information into the display.
                printf("%4u %50s (%-4u x %-4u)\n", counter + 1, filename, width, height);
            }
            
            // Go to the next png file in the file list.
            list_elem = list_next(list_elem);
            counter += 1;
        }
    }
    
    // Release system memory occupied by the file list.
    list_destroy(&png_files);
    
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
