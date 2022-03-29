
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



/*
 *  Initializes a texture object "btex_texture"
 *
 *  Parameter:
 *      texture_data        :   Pointer to a btex_texture object which is being initialized
 *
 *  Returns
 *      0 for successful initialization
 *      -1 for error
*/
int btex_texture_init(struct BTEX_TEXTURE *texture_data);



/*
 *  Destroyes a texture object "btex_texture"
 *
 *  Parameter:
 *      texture_data        :   Pointer to a btex_texture object which is being destroyed
 *
 *  Returns
 *      Nothing
*/
void btex_texture_destroy(struct BTEX_TEXTURE *texture_data);



/*
 *  Retrieve useful texture information about the texture from file.
 *	Only the texture type, width, height and compression information is read.
 *	This function does not read or modify the variables "file_name" and "file_path".
 *
 *  Parameter:
        filename            :   Full path to a texture file which must be either a
                                RenderWare (.rw) texture format or a
                                Portable Network Graphics (.png) format
 *      texture_data        :   Pointer to an initialized btex_texture object which
                                will receive the information extracted from the specified file
 *
 *  Returns
 *      0 for successful execution
 *      -1 if the specified file does not exist
 *      -2 if the format is not a valid texture format
*/
int btex_texture_info(const char *filename, struct BTEX_TEXTURE *texture);



/*
 *  Build up a list of all the child directories (folders) for a
 *  specific parent directory. This algorithm is not recursive, it
 *  will not go through inside of each subdirectories.
 *  The output list will not contain special directories like "." and "..".
 *
 *  Parameter:
 *      path                :   Full path to a directory
 *      directories         :   Pointer to an initialized singly linked list which
 *                              will contain each directory as a C string (char *).
 *
 *  Returns
 *      0 for successful execution
 *      -1 if path or directory parameters are NULL
 *      -2 if the specified path could not be found
*/
int btex_list_directories(const char *path, List *directories);



/*
 *  Build up a list of all the child directories (folders) for a
 *  specific parent directory. This algorithm is not recursive, it
 *  will not go through inside of each subdirectories.
 *
 *  Parameter:
 *      path                :   Full path to a directory
 *      directories         :   Pointer to an initialized singly linked list which
 *                              will contain each directory as a C string (char *).
 *
 *  Returns
 *      0 for successful execution
 *      -1 if path or directory parameters are NULL
 *      -2 if the specified path could not be found
*/
int btex_list_textures(const char *path, const char *directory, List *textures);



/*
 *  Copy contents from the specified source file to the destination file.
 *	This function does a byte by byte copy of the source file.
 *	Metadata of source file like file attributes, file dates etc are not copied
 *	to the destination file.
 *
 *  Parameter:
        source_file         :   Full path to the source file from where the bytes are copied
 *      dest_file	        :   Full path to the target file to where the bytes are copied
 *
 *  Returns
 *      0 for successful execution
 *      -1 if source_file or dest_file is NULL or empty
 *      -2 if some error occurs while opening either files
*/
int btex_copy(const char *source_file, const char *dest_file);




#endif // BTEX_H_INCLUDED
