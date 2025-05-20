#ifndef __SIMPLEFILELOADER_H__
#define __SIMPLEFILELOADER_H__
#include "modding.h"
#include "global.h"

// Represents a file loaded by SFL_LoadFile
//
// data points to the start of the file in memory. Remember to recomp_free it when you are done with the file
//
// size is the size in bytes of the data buffer
typedef struct {
    u8 *data;
    u32 size;
} SFL_File;

RECOMP_IMPORT("mm_recomp_simplefileloader", bool SFL_isExist(const char *path));
RECOMP_IMPORT("mm_recomp_simplefileloader", bool SFL_isFile(const char* path));
RECOMP_IMPORT("mm_recomp_simplefileloader", bool SFL_isDirectory(const char *path));

// Returns the size (in bytes) of the file at the path. If there is no file at the path, then 0 is returned
RECOMP_IMPORT("mm_recomp_simplefileloader", u32 SFL_getFileSize(const char *path));

// Returns a SFL_File struct representing the file just loaded
// If there is no file at the path or it has no data, then the data pointer is NULL, and size is 0.
RECOMP_IMPORT("mm_recomp_simplefileloader", SFL_File SFL_loadFile(const char *path));

// Returns number of files and folders in the directory passed in. Returns 0 if non-dir passed in.
RECOMP_IMPORT("mm_recomp_simplefileloader", u32 SFL_getNumDirEntries(const char *dirPath));

// Returns a string representing the file or folder at the index in dirPath
// The directory is sorted alphabetically
// Elements are listed recursively with directories followed by their contents
// recomp_free this string when you are done with it to avoid a memory leak
RECOMP_IMPORT("mm_recomp_simplefileloader", char* SFL_getDirEntryNameByIndex(const char *dirPath, u32 index));

// Combines the paths passed in into a single string
// count number of char pointers should be passed in
// '/' will be used as the directory seperator
// recomp_free this string when you are done with it to avoid a memory leak
RECOMP_IMPORT("mm_recomp_simplefileloader", char *SFL_getCombinedPath(u32 count, ...));

#endif
