#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

#include <oniguruma.h>
#include <stdio.h>

static int nerror = 0;

static void xx(char *pattern, char* str, int from, int to, int mem, int not) {
  regex_t *reg;
  OnigErrorInfo einfo;

  FILE* err_file;
  int r = onig_new(&reg, (UChar *)pattern, (UChar *)(pattern + strlen(pattern)),
    ONIG_OPTION_DEFAULT, ONIG_ENCODING_UTF8, ONIG_SYNTAX_DEFAULT, &einfo);

 if (r) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str((UChar* )s, r, &einfo);
    fprintf(err_file, "ERROR: %s\n", s);
    nerror++;
    return ;
  }
}
