#ifndef parse_questions_h
#define parse_questions_h

#include <stdlib.h>
#include <stdint.h>

struct choices {
  uint8_t    choice_number;
  char     **choices;
};

struct choices split(const char *input, const char *separator);
void free_choices(struct choices *choices);

char *format_choices(const char *questions);
#endif
