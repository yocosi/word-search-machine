#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/types.h"
#include "../include/functions.h" // extern functions declarations

// ------------------------------------------------------------------------
// inner functions declarations
// ------------------------------------------------------------------------
void delete_words_from_table(int j, listfile_entry *filelist, char filename[], hash_table *htable_ptr);
int file_handler(char s[], char word[], char filename[], int indexToAdd, hash_table *htable_ptr);
int is_file_on_table(listfile_entry *filelist, char filename[]);
int search_space_in_file_table(listfile_entry *filelist);
int search_index_of_file(listfile_entry *filelist, char filename[]);

//------------------------------------------------------------------------
// global functions definitions
//------------------------------------------------------------------------

/**
   Create and initialize file table of capacity maxfiles

   parameters :
   maxfiles : capacity of file table

   returns : pointer to table or NULL in case of error
 */
listfile_entry *create_filelist(int maxfiles)
{

  listfile_entry *file_list = malloc(sizeof(listfile_entry) * maxfiles);
  if (file_list == NULL)
  {
    return NULL;
  }
  for (int i = 0; i < maxfiles; i++)
  {
    file_list[i].loaded = 0;
    memset(file_list[i].filename, ' ', MAX_LENGTH);
  }
  return file_list;
}

/**
   add words from file to table
   - checks if the file has already been loaded
   - updates the file table (if file not already loaded)
   - reads every word in the file (idem)
   - updates the hash table (idem)

   parameters :
   filename   : name of file :)
   filelist   : pointer to table of files
   htable_ptr : pointer to hash table

   returns :
    1 if file already present in table
    2 if no space left in filelist
   -1 if file doesn't exist or can't be read
   -2 if allocation error
    0 if everything ok
*/
int add_file(char filename[],
             listfile_entry *filelist,
             hash_table *htable_ptr)
{
  if (is_file_on_table(filelist, filename) != -1)
  {
    return 1;
  }

  int indexToAdd = search_space_in_file_table(filelist);
  if (indexToAdd == -1)
  {
    return 2;
  }

  strcpy(filelist[indexToAdd].filename, filename);
  filelist[indexToAdd].loaded = 1;

  char word[MAX_LENGTH];
  char s[MAX_LENGTH] = "test/";
  strcat(s, filename);

  return file_handler(s, word, filename, indexToAdd, htable_ptr);
}

/**
   remove file from file table

   parameters :
   filename   : name of file to remove
   filelist   : pointer to table of files
   htable_ptr : pointer to hash table

   returns :
   -1 if file not in table
    0 if file removed
*/
int remove_file(char filename[],
                listfile_entry *filelist,
                hash_table *htable_ptr)
{
  int file_index = search_index_of_file(filelist, filename);
  if (file_index == -1)
  {
    fprintf(stderr, "File is not in the table.\n");
    return -1;
  }

  delete_words_from_table(file_index, filelist, filename, htable_ptr);
  return 0;
}

/*
  print file table (only loaded files)

  parameters :
   filelist : pointer to table of files
*/
void print_list(listfile_entry *filelist)
{
  printf("Files loaded: \n");
  for (int i = 0; i < sizeof(filelist); i++)
  {
    if (filelist[i].loaded == 1)
    {
      printf("\t- %s [%d]\n", filelist[i].filename, i);
    }
  }
}

/**
   free file table

parameters :
   filelist   : pointer to table of files
*/
void free_filelist(listfile_entry *filelist)
{
  free(filelist);
}

// ************************************************************************
// inner functions
// ************************************************************************

/**
 * Delete words from the table for a specific file
 *
 * parameters:
 *  file_index : index of the file to remove
 *  filelist : pointer to table of file to remove
 *  filename : name of the file to remove
 *  htable_ptr : pointer to hash table
 */
void delete_words_from_table(int file_index, listfile_entry *filelist, char filename[], hash_table *htable_ptr)
{
  for (int i = 0; i < htable_ptr->hsize; i++)
  {
    if (htable_ptr->htable[i].first_word != NULL)
    {
      word_list *word_list_to_delete = &htable_ptr->htable[i];
      word_entry *current = word_list_to_delete->first_word;

      while (current != NULL)
      {
        if (current->in_file == file_index)
        {
          word_list_to_delete->first_word = current->next;
          free(current);
          current = word_list_to_delete->first_word;
        }
        else
        {
          current = current->next;
        }
      }
    }
  }
  strcpy(filelist[file_index].filename, "");
  filelist[file_index].loaded = 0;
  printf("File %s got removed.\n", filename);
}

/**
 * Handle the file manipulation to add the content of this file to the hash table
 *
 * parameters:
 *  s : Relative path of the file
 *  word : word to read in the file (loop through all words)
 *  filename : name of the file to add
 *  indexToAdd : gives the index where we should load the file
 *  htable_ptr : pointer to hash table
 *
 * returns:
 *  -1 if the files doesn't exist or can't be read
 *  -2 if allocation error
 *  0 if all fine
 */
int file_handler(char s[], char word[], char filename[], int indexToAdd, hash_table *htable_ptr)
{
  FILE *fp;
  fp = fopen(s, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "File doesn't exist or can't be read.\n");
    return -1;
  }

  int i = 0;
  while (fscanf(fp, "%s", word) == 1)
  {
    if (i > htable_ptr->hsize)
    {
      fprintf(stderr, "\n[ALLOCATION ERROR] - There are too many words in %s\nThe file has been loaded with just %d words.\n", filename, i);
      return -2;
    }
    update_table(htable_ptr, word, filename, indexToAdd);
    i++;
  }
  fclose(fp);
  return 0;
}

/**
 * Check if the file already exist
 *
 * parameters:
 *  filelist : pointer to table of file
 *  filename : name of the file
 *
 * returns:
 *  1 if file already exist
 *  -1 if file doesn't exist
 */
int is_file_on_table(listfile_entry *filelist, char filename[])
{
  for (int i = 0; i < MAX_FILES; i++)
  {
    if (strcmp(filelist[i].filename, filename) == 0)
    {
      fprintf(stderr, "File is already present in table.\n");
      return i;
    }
  }
  return -1;
}

/**
 * Check if there is space to add a file in the filelist and returns the index where it can be added
 *
 * parameters:
 *  filelist : pointer to table of file
 *
 * returns:
 *  i if there is space on filelist
 *  -1 if there is no space left
 */
int search_space_in_file_table(listfile_entry *filelist)
{
  for (int i = 0; i < MAX_FILES; i++)
  {
    if (filelist[i].loaded == 0)
    {
      return i;
    }
  }
  fprintf(stderr, "No space left in filelist.\n");
  return -1;
}

/**
 * Search for a file in filelist and return the index
 *
 * parameters:
 *  filelist : pointer to table of file
 *  filename : name of the file
 *
 * returns:
 *  i if file is found on filelist
 *  -1 if is not found
 */
int search_index_of_file(listfile_entry *filelist, char filename[])
{
  for (int i = 0; i < MAX_FILES; i++)
  {
    if (strcmp(filelist[i].filename, filename) == 0 && filelist[i].loaded == 1)
    {
      return i;
    }
  }
  return -1;
}
