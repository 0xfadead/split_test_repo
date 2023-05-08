#include "parse_questions.h"
#include <stdio.h>

int main(void) {
  char test_text[] = "- Test1- Test2- Test3- Test4";
  struct choices choices = split(test_text, "- ");
  free_choices(&choices);

  return 0;
}
