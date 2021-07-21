
#ifndef BTEX_H_INCLUDED
#define BTEX_H_INCLUDED


#define BTEX_TYPE_UNDEFINED         0
#define BTEX_TYPE_PNG               2
#define BTEX_TYPE_RW                4
#define BTEX_COMPRESS_UNKNOWN       0
#define BTEX_COMPRESS_UNCOMPRESSED  0
#define BTEX_COMPRESS_DXT1          1
#define BTEX_COMPRESS_DXT2          2
#define BTEX_COMPRESS_DXT3          3
#define BTEX_COMPRESS_DXT4          4
#define BTEX_COMPRESS_DXT5          5


#include <list.h>
#include <avl.h>



struct BTEX_TEXTURE {
    int type;
    unsigned int width, height;
    unsigned int compression_level;
    char *file_path;
    char *file_name;
};


int btex_texture_init(struct BTEX_TEXTURE *texture_data);

void btex_texture_destroy(struct BTEX_TEXTURE *texture_data);



int btex_texture_info(const char *filename, struct BTEX_TEXTURE *texture);


int btex_list_directories(const char *path, List *directories);


int btex_list_textures(const char *path, const char *directory, List *textures);


int btex_copy(const char *source_file, const char *dest_file);

#endif // BTEX_H_INCLUDED
