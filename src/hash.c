#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/functions.h" // global functions declarations

// ------------------------------------------------------------------------
// inner functions declarations
// ------------------------------------------------------------------------

word_entry *create_word_entry(int fileIndex, char word[]);
int search_word_in_table(word_list *word_list_to_search_in, listfile_entry *filelist, char word[]);
void update_handler(word_list *word_list_to_add_in, int file_index, char word[]);
void print_table_index(hash_table *htable_ptr, listfile_entry *filelist, int i);
char *clean_word(char word[]);

//------------------------------------------------------------------------
// global functions definitions
//------------------------------------------------------------------------

/**
   create and initialize hash table

   returns : pointer to table or NULL if creation error
*/
hash_table *create_table()
{
   hash_table *hash_t = (hash_table *)malloc(sizeof(hash_table));
   if (hash_t == NULL)
   {
      return NULL;
   }
   hash_t->hsize = MAX_ENTRIES;
   hash_t->htable = (word_list *)malloc(sizeof(word_list) * hash_t->hsize);
   if (hash_t->htable == NULL)
   {
      return NULL;
   }
   for (int i = 0; i < hash_t->hsize; i++)
   {
      hash_t->htable[i].first_word = NULL;
      hash_t->htable[i].last_word = NULL;
   }
   return hash_t;
}

/**
   search a word in table ; print word if found, with number of occurrences
   and file where word is found

   parameters :
   word : the word to look for
   filelist   : pointer to table of files
   htable_ptr : pointer to hash table

   returns : true if found, false otherwise
*/
int search_word(char word[],
                listfile_entry *filelist,
                hash_table *htable_ptr)
{
   int hCode = hashcode(word, strlen(word));
   word_list *word_list_to_search_in = &htable_ptr->htable[hCode];
   return search_word_in_table(word_list_to_search_in, filelist, word);
}

/**
   lookup for word in table and update table accordingly

   parameters :
   htable_ptr : pointer to hash table
   word : word to be added into the hash table
   filename   : filename from where the word was read
   file_index:  the position where the filename has been stored
*/
void update_table(hash_table *htable_ptr,
                  char word[],
                  char filename[],
                  int file_index)
{
   if (htable_ptr == NULL)
   {
      htable_ptr = create_table();
   }
   word = clean_word(word);
   int hCode = hashcode(word, strlen(word));
   word_list *word_list_to_add_in = &htable_ptr->htable[hCode];
   update_handler(word_list_to_add_in, file_index, word);
}

/**
   print table contents

   parameters :
   htable_ptr : pointer to hash table
   filelist   : pointer to table of files
*/
void print_table(hash_table *htable_ptr,
                 listfile_entry *filelist)
{
   printf("Words in the table: \n");
   for (int i = 0; i < htable_ptr->hsize; i++)
   {
      print_table_index(htable_ptr, filelist, i);
   }
}

/**
   free hash table

   parameters :
   htable_ptr : pointer to hash table
*/
void free_table(hash_table *htable_ptr)
{
   for (int i = 0; i < htable_ptr->hsize; i++)
   {
      word_entry *to_free = htable_ptr->htable[i].first_word;
      while (to_free != NULL)
      {
         word_entry *aux = to_free;
         to_free = to_free->next;
         free(aux);
      }
   }
   free(htable_ptr->htable);
   free(htable_ptr);
}

// ------------------------------------------------------------------------
// inner functions definitions
// ------------------------------------------------------------------------

/**
 * Create a new word entry
 *
 * parameters:
 *  fileIndex : index of the file where the word is
 *  word : to add to the word entry
 *
 * returns:
 *  NULL if there is allocation error
 *  to_add if the word entry is well created
 */
word_entry *create_word_entry(int fileIndex, char word[])
{
   word_entry *to_add = (word_entry *)malloc(sizeof(word_entry));
   if (to_add == NULL)
   {
      return NULL;
   }
   strcpy(to_add->word, word);
   to_add->in_file = fileIndex;
   to_add->times = 1;
   to_add->next = NULL;
   return to_add;
}

/**
 * Search for a word in the table
 *
 * parameters:
 *  word_list_to_search_in : word list needed to find the word in
 *  filelist : pointer to table of file
 *  word : word to search
 *
 * returns:
 *  0 if the word does not exist in the table
 *  1 if the word was found
 */
int search_word_in_table(word_list *word_list_to_search_in, listfile_entry *filelist, char word[])
{
   while (word_list_to_search_in->first_word != NULL)
   {
      word_entry *to_search = word_list_to_search_in->first_word;
      if (strcmp(word_list_to_search_in->first_word->word, word) == 0)
      {
         printf("The word exist %d times.\nHe was found in file %s at index %d.\n", to_search->times, filelist[to_search->in_file].filename, to_search->in_file);
         return 1;
      }
      word_list_to_search_in->first_word = word_list_to_search_in->first_word->next;
   }
   fprintf(stderr, "The word does not exist in table.\n");
   return 0;
}

/**
 * Update the table
 *
 * parameters:
 *  word_list_to_search_in : word list needed to add the word in
 *  file_index : index of the file
 *  word : word to add
 */
void update_handler(word_list *word_list_to_add_in, int file_index, char word[])
{
   if (word_list_to_add_in->first_word == NULL)
   {
      word_list_to_add_in->first_word = create_word_entry(file_index, word);
   }
   else
   {
      word_entry *to_add = word_list_to_add_in->first_word;
      word_entry *prev = to_add;
      while (to_add != NULL)
      {
         if (strcmp(to_add->word, word) == 0 && to_add->in_file == file_index)
         {
            to_add->times++;
            return;
         }
         prev = to_add;
         to_add = to_add->next;
      }
      word_entry *new_word = create_word_entry(file_index, word);
      prev->next = new_word;
   }
}

/**
 * print the table index passed in paremeters (i)
 *
 * parameters:
 *  htable_ptr : pointer to hash table
 *  filelist : pointer to table of file
 *  i : index of the table to print
 */
void print_table_index(hash_table *htable_ptr, listfile_entry *filelist, int i)
{
   if (htable_ptr->htable[i].first_word != NULL)
   {
      word_list *word_list_to_print = &htable_ptr->htable[i];
      word_entry *to_print = word_list_to_print->first_word;
      while (to_print != NULL)
      {
         char *filename_to_print = filelist[to_print->in_file].filename;
         char *word_to_print = to_print->word;
         int times = to_print->times;
         if (filelist[to_print->in_file].loaded == 1)
         {
            printf("Filename: %s || Times in file: %d || Word: %s\n", filename_to_print, times, word_to_print);
         }
         to_print = to_print->next;
      }
   }
}

/**
 * Clean a word to make it to lower case and avoid having to deal with punctuation, space, or digit
 *
 * parameters:
 *  word : word to clean
 *
 * returns:
 *  word without punctuation
 */
char *clean_word(char word[])
{
   for (int i = 0; word[i]; i++)
   {
      if (word[i] == ',' || word[i] == '.' || isspace(word[i]) || isdigit(word[i]) || word[i] == '!' || word[i] == ';')
      {
         word[i] = ' ';
      }
      else
      {
         word[i] = tolower(word[i]);
      }
   }
   return word;
}
