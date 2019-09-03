#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

#include <oniguruma.h>

// static int nerror = 0;

/*static void xx(char *pattern, char* str, int from, int to, int mem, int not) {
  regex_t *reg;
  OnigErrorInfo einfo;

  int status = onig_new(&reg, (UChar *)pattern, (UChar *)(pattern +
strlen(pattern)), ONIG_OPTION_CAPTURE_GROUP, ONIG_ENCODING_UTF8,
ONIG_SYNTAX_DEFAULT, &einfo);

 if (status != ONIG_NORMAL) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str((UChar* )s, r, &einfo);
    printf("ERROR: %s\n", s);
    nerror++;
    return ;
  }

  printf("SUCCESS");
}*/

typedef struct _regexp {
  regex_t *regexp;
} regexp_W;

void reonig_finalize_regexp(value v) {
  regexp_W *p;
  p = (regexp_W *)Data_custom_val(v);
  onig_free(p->regexp);
};

static struct custom_operations regexp_custom_ops = {
  identifier : "regexp handling",
  finalize : reonig_finalize_regexp,
  compare : custom_compare_default,
  hash : custom_hash_default,
  serialize : custom_serialize_default,
  deserialize : custom_deserialize_default
};

static value reonig_val_result_error(char *errorMsg) {
  CAMLparam0();
  CAMLlocal1(error);
  error = caml_alloc(1, 1);
  Store_field(error, 0, caml_copy_string(errorMsg));
  CAMLreturn(error);
}

static value reonig_val_result_ok(value val) {
  CAMLparam0();
  CAMLlocal1(result);
  result = caml_alloc(1, 0);
  Store_field(result, 0, val);
  CAMLreturn(result);
}

#define Val_none Val_int(0)

static value reonig_val_some(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 0);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

CAMLprim value reonig_create(value vPattern) {
  CAMLparam1(vPattern);
  CAMLlocal2(result, v);

  regex_t *reg;
  OnigErrorInfo einfo;

  char *pattern = String_val(vPattern);
  int status =
      onig_new(&reg, (UChar *)pattern, (UChar *)(pattern + strlen(pattern)),
               ONIG_OPTION_CAPTURE_GROUP, ONIG_ENCODING_UTF8,
               ONIG_SYNTAX_DEFAULT, &einfo);

  if (status != ONIG_NORMAL) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str((UChar *)s, status, &einfo);
    result = reonig_val_result_error(s);
    onig_free(reg);
  } else {

    regexp_W regexpWrapper;
    regexpWrapper.regexp = reg;
    v = caml_alloc_custom(&regexp_custom_ops, sizeof(regexp_W), 0, 1);
    memcpy(Data_custom_val(v), &regexpWrapper, sizeof(regexp_W));
    result = reonig_val_result_ok(v);
  }

  CAMLreturn(result);
}

CAMLprim value reonig_search(value vStr, value vPos, value vEnd,
                             value vRegExp) {
  CAMLparam4(vStr, vPos, vEnd, vRegExp);

  UChar *searchData = String_val(vStr);
  size_t position = Int_val(vPos);
  size_t end = Int_val(vEnd);

  regexp_W *p = Data_custom_val(vRegExp);
  regex_t *regex = p->regexp;

  OnigRegion *region = onig_region_new();
  int status =
      onig_search(regex, searchData, searchData + end, searchData + position,
                  searchData + end, region, ONIG_OPTION_NONE);

  CAMLreturn(Val_none);
};
