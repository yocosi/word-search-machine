#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/functions.h"

//------------------------------------------------------------------------

int main()
{
  // create hash table
  hash_table *htable_ptr;
  htable_ptr = create_table();

  // create filelist array
  listfile_entry *filelist;
  filelist = create_filelist(MAX_FILES);

  // display menu
  while (1)
  {
    int nbchoices = 0;
    fprintf(stderr, "\nChoisir une action\n");
    fprintf(stderr, "%d. Load a file in dictionary\n", ++nbchoices);
    fprintf(stderr, "%d. Search a word in dictionary\n", ++nbchoices);
    fprintf(stderr, "%d. Remove file from dictionary\n", ++nbchoices);
    fprintf(stderr, "\n");
    fprintf(stderr, "%d. Print dictionary\n", ++nbchoices);
    fprintf(stderr, "%d. Print file list\n", ++nbchoices);
    fprintf(stderr, "\n0. Quit\n");
    int choice;
    while (1)
    {
      fprintf(stderr, "Your choice ? ");
      scanf("%d", &choice);
      if (choice >= 0 && choice <= nbchoices)
      {
        break;
      }
      fprintf(stderr, "\nError %d is an incorrect choice\n", choice);
    }
    if (choice == 0)
    {
      break;
    }

    fprintf(stderr, "-------------------------------------------------\n");

    char file_str[30];
    char word[MAX_LENGTH];
    switch (choice)
    {
      // Load a file in dictionary
    case 1:
      printf("Which file do you want to load ?\n");
      scanf("%s", file_str);
      add_file(file_str, filelist, htable_ptr);
      break;

    // Search a word in dictionary
    case 2:

      printf("Which word do you want to search ?\n");
      scanf("%s", word);
      search_word(word, filelist, htable_ptr);
      break;

    // Remove file from dictionary
    case 3:

      printf("Which file do you want to remove ?\n");
      scanf("%s", file_str);
      remove_file(file_str, filelist, htable_ptr);
      break;

      // Print dictionary
    case 4:
      print_table(htable_ptr, filelist);
      break;

    // Print file list
    case 5:
      print_list(filelist);
      break;
    }
    fprintf(stderr, "-------------------------------------------------\n");
  }

  // the end : free allocated memory
  free_filelist(filelist);
  free_table(htable_ptr);

  return 0;
}

// compute hash value for word
// returns : N ; 0 <= N < size
int hashcode(char word[], int size)
{
  int N = 0;
  while (*word != '\0')
  {
    N += *word++;
  }
  return (N % size);
}
