#include "utils/file.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* file_rem_path(char* filename){
	char* fpath = strrchr(filename, '/');
	return fpath ? fpath + 1 : filename;
}

char* file_add_pref(char* filename, char* pref){
	const size_t sz_filename = strlen(filename);
	const size_t sz_pref = strlen(pref);
	const size_t sz_dest = sz_filename + sz_pref + 1;
	char* dest = calloc(sz_dest, sizeof(char));
	strcat(dest, pref);
	return strcat(dest, filename);
}

char* file_rem_ext(char* filename){
	char* ext = strrchr(filename, '.');
	if(!ext){
		return NULL;
	}
	ext[0] = '\0';
	return filename;
}

char* file_add_ext(char* filename, char* ext){
	const size_t sz_filename = strlen(filename);
	const size_t sz_ext = strlen(ext);
	const size_t sz_dest = sz_filename + sz_ext + 1;
	char* dest = calloc(sz_dest, sizeof(char));
	strcat(dest, filename);
	return strcat(dest, ext);
}

ssize_t file_get_nbytes(FILE* fp){
	long curpos = ftell(fp);

	fseek(fp, 0, SEEK_SET);				
	long inipos = ftell(fp);

	fseek(fp, 0, SEEK_END);				
	long endpos = ftell(fp);

	fseek(fp, curpos, SEEK_SET);			

	return endpos - inipos;
}

ssize_t file_get_remaining_nbytes(FILE* fp){
	long curpos = ftell(fp);

	fseek(fp, 0, SEEK_END);				
	long endpos = ftell(fp);

	fseek(fp, curpos, SEEK_SET);			

	return endpos - curpos;
}
