
#ifndef __QDFILELOADER_API_H__
#define __QDFILELOADER_API_H__
#include "modding.h"

#define YAZMT_QDFILELOADER_MOD_NAME "yazmt_mm_qdfileloader"

typedef enum {
    QDFL_STATUS_OK,
    QDFL_STATUS_ERR_UNKNOWN,
    QDFL_STATUS_ERR_PATH_NO_EXIST,
    QDFL_STATUS_ERR_FILE_NO_EXIST,
    QDFL_STATUS_ERR_DIR_NO_EXIST,
    QDFL_STATUS_EXPECTED_FILE,
    QDFL_STATUS_EXPECTED_DIR,
    QDFL_STATUS_ERR_FILE_TOO_LARGE,
} QDFL_Status;

// Returns 1 if path corresponds to an existing file or directory.
//
// Returns 0 otherwise.
RECOMP_IMPORT(YAZMT_QDFILELOADER_MOD_NAME, int QDFL_isExist(const char *path));

// Returns 1 if path corresponds to an existing file.
//
// Returns 0 otherwise.
RECOMP_IMPORT(YAZMT_QDFILELOADER_MOD_NAME, int QDFL_isFile(const char *path));

// Returns 1 if path corresponds to an existing directory.
//
// Returns 0 otherwise.
RECOMP_IMPORT(YAZMT_QDFILELOADER_MOD_NAME, int QDFL_isDirectory(const char *path));

// Writes the size (in bytes) of the file at the path into out and returns QDFL_STATUS_OK.
//
// If an error occurs, a different QDFL_Status is returned, and out is left unmodified.
RECOMP_IMPORT(YAZMT_QDFILELOADER_MOD_NAME, QDFL_Status QDFL_getFileSize(const char *path, unsigned long *out));

// Reads a file into memory, writes the pointer to out, and returns QDFL_STATUS_OK.
//
// If the file cannot be read, a different QDFL_Status is returned, and out is left unmodified.
//
// Remember to recomp_free this file when you are done with it.
RECOMP_IMPORT(YAZMT_QDFILELOADER_MOD_NAME, QDFL_Status QDFL_loadFile(const char *path, void **out));

// Writes number of files and folders in the directory to out and returns QDFL_STATUS_OK.
//
// If an error occurs, out is left unmodified, and a different QDFL_Status is returned.
RECOMP_IMPORT(YAZMT_QDFILELOADER_MOD_NAME, QDFL_Status QDFL_getNumDirEntries(const char *dirPath, unsigned long *out));

// Writes a pointer to a string representing the file or folder at the index in dirPath to out and
// returns QDFL_STATUS_OK.
//
// The directory is sorted alphabetically, so in a dir containing files 'A.txt', 'B.txt', and 'C.txt',
// passing index 1 in would write a pointer to string 'B.txt'.
//
// Elements are listed recursively with directories followed by their contents.
//
// recomp_free this string when you are done with it to avoid a memory leak.
//
// If an error occurs, a different status is returned, and out is left unmodified.
RECOMP_IMPORT(YAZMT_QDFILELOADER_MOD_NAME, QDFL_Status QDFL_getDirEntryNameByIndex(const char *dirPath, unsigned long index, char **out));

// Combines count number of paths into a single string.
//
// '/' will be used as the directory seperator, which is compatible with Windows, Mac, and Linux
//
// recomp_free this string when you are done with it to avoid a memory leak
RECOMP_IMPORT(YAZMT_QDFILELOADER_MOD_NAME, char *QDFL_getCombinedPath(unsigned long count, ...));

#endif
