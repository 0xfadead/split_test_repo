#include "parse_questions.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define debug_all

char *_multi_strtok_last_string = NULL;

char *multi_strtok(const char *string, const char *delim) {
  // the start of the returned string
  char *token_start = NULL;
  // the end of the returned string
  char *token_end   = NULL;

  if (string) {
    // find the delimiter
    token_start = strstr(string, delim);
    // if it's not found, return NULL
    if (!token_start) return NULL;
    
    // add the length of the delimiter to not find the same delimiter again
    token_end   = token_start + strlen(delim);
    // find the next token
    token_end   = strstr(token_end, delim);
    // if it's not found, the end is the string end so just add the length of the remaining bytes
    if (!token_end) token_end = token_start + strlen(token_start); 
    
    // malloc the length of the found string plus 1 for the terminator char
    char *token = (char *)malloc(token_end - token_start + 1);
    // make sure the end char is NULL
    token[token_end - token_start] = '\0';
    // copy the found string into the return string
    memcpy(token, token_start, token_end - token_start);
    
#if defined(debug_strtok) || defined(debug_all)
    fprintf(stderr, "[%lu] [%s] [%lu]\n", token_end - token_start, token, (uint64_t)token);
#endif

    // for the next time make the new the the end of this string
    _multi_strtok_last_string = token_end;

    // return the found string
    return token;
  }
  
  // -----begin same as other code-----
  token_start = strstr(_multi_strtok_last_string, delim);
  if (!token_start) return NULL;

  token_end  = token_start + strlen(delim);
  token_end  = strstr(token_end, delim);
  if (token_end == NULL) token_end = token_start + strlen(token_start);

  char *token = (char *)malloc(token_end - token_start + 1);
  token[token_end - token_start] = '\0';
  memcpy(token, token_start, token_end - token_start);

#if defined(debug_strtok) || defined(debug_all)
  fprintf(stderr, "[%lu] [%s] [%lu]\n", token_end - token_start, token, (uint64_t)token);
#endif

  _multi_strtok_last_string = token_end;

  return token;
  // -----end same as other code-----
}

struct choices split(const char *input, const char *separator) {
  // basic init for the output
  struct choices output;
  output.choice_number  = 0;

  // find first string with specified delim.
  char *token = multi_strtok(input, separator);

  // if it's not found return (choice_number is already set)
  if (!token) return output;
  // else allocate space for a char ptr
  output.choices    = (char **)malloc(sizeof(char *));
  
  // enter the mainloop
  while (1) {
    // set the current array member to the token (so 0 first time, 1 next time etc...)
    output.choices[output.choice_number] = token;
    // find next string
    token = multi_strtok(NULL, separator);
    
    // check if a string was found (if not break and return from function)
    if (!token) break;

    // increment to next array iterator
    output.choice_number++;
    // allocate space for the next char ptr
    output.choices = (char **)realloc(output.choices, output.choice_number * sizeof(char *));
  }
  
  return output;
}

void free_choices(struct choices *input) {
#if defined(debug_free_choices) || defined(debug_all)
  fprintf(stderr, "Freeing choices!\n");
  for (int i = 0; i < input->choice_number; i++) {
    fprintf(stderr, "[%lu] [%s] [%lu]\n", 
            (!input->choices[i] ? 0 : strlen(input->choices[i])), 
            (!input->choices[i] ? "NULL" : input->choices[i]), 
            (!input->choices[i] ? 0 : (uint64_t)input->choices[i])); 
  }
#endif
  // free every member of array
  for (int i = 0; i < input->choice_number; i++) { 
    free(input->choices[i]);
  }
#if defined(debug_free_choices) || defined(debug_all)
  fprintf(stderr, "Freeing choice ptr!\n");
#endif
  
  // free the choice ptrptr (sinced it was malloc'd and realloc'd)
  free(input->choices);

  return;
}
