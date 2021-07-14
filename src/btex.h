
#ifndef BTEX_H_INCLUDED
#define BTEX_H_INCLUDED


#include <list.h>
#include <avl.h>




int btex_list_directories(const char *path, List *directories);


int btex_list_textures(const char *path, const char *directory, List *textures);


int btex_png_dimension(const char *filename, unsigned int *width, unsigned int *height);

#endif // BTEX_H_INCLUDED
