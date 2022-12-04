#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "types.h"

// ------------------------------------------------------------------------
// file.c
// ------------------------------------------------------------------------

// create and initialize file table of capacity maxfile
listfile_entry *
create_filelist(int maxfiles);

// add words from file to table
int add_file(char filename[],
						 listfile_entry *filelist,
						 hash_table *htable_ptr);

// remove file from file table
int remove_file(char filename[],
								listfile_entry *filelist,
								hash_table *htable_ptr);

// print file table
void print_list(listfile_entry *filelist);

// free file table
void free_filelist(listfile_entry *filelist);

// ------------------------------------------------------------------------
// hash.c
// ------------------------------------------------------------------------

// create hash table
hash_table *
create_table();

// search a word in table and print it
// returns : true if found, false otherwise
int search_word(char word[],
								listfile_entry *filelist,
								hash_table *htable_ptr);

// add/update a word in table
void update_table(hash_table *htable_ptr,
									char word[],
									char filename[],
									int file_index);

// print table contents
void print_table(hash_table *htable_ptr,
								 listfile_entry *filelist);

// free hash table
void free_table(hash_table *htable_ptr);

// ------------------------------------------------------------------------
// main.c
// ------------------------------------------------------------------------

// compute hash value for word
// returns : N;/ 0 <= N < size
int hashcode(char word[], int size);

#endif // __FUNCTIONS_H__
