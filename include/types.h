#ifndef __TYPES_H__
#define __TYPES_H__

#define MAX_LENGTH 50    // maximum word length of an entry
#define MAX_FILES 20     // maximum number of files
#define MAX_ENTRIES 1023 // capacity of hash table

// elements of the word list
typedef struct word_entry
{
  char word[MAX_LENGTH];
  int in_file; // index of file in file table
  int times;   // how many times does the word exist
  struct word_entry *next;
} word_entry;

// simple linked list of word entries
typedef struct
{
  word_entry *first_word;
  word_entry *last_word;
} word_list;

// a hash table is an array of word_list + maximum number of elements in the array
typedef struct
{
  word_list *htable;
  int hsize; // capacity of array
} hash_table;

// names of files loaded in the hash table + loaded status
typedef struct
{
  char filename[MAX_LENGTH];
  int loaded; // true if file loaded
} listfile_entry;

#endif // __TYPES_H__
