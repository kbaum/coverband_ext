/************************************************

  coverband_ext.c -

  $Author: $

  Copyright (c) 2014 Dan Mayer

 ************************************************/

#include <ruby.h>

static int
any_ignore_patterns( VALUE rb_ignore_patterns, const char *srcfile ) {
  int ignore_patterns_len = RARRAY_LEN(rb_ignore_patterns);
  VALUE * ignore_patterns = RARRAY_PTR(rb_ignore_patterns);
  char * ignore_pattern;
  int i;
  for(i = 0; i < ignore_patterns_len; i++) {
    ignore_pattern  = StringValueCStr(ignore_patterns[i]);
    if (strstr(srcfile, ignore_pattern)) {
      return 1;
    }
  }
  return 0;
}




  static VALUE
cb_track_file(VALUE self, VALUE original_file)
{
  VALUE file = StringValue(original_file);
  const char * srcfile = RSTRING_PTR(original_file);
  VALUE currentCoverbandBase = rb_funcall(rb_path2class("Coverband::Base"), rb_intern("instance"), 0);
  VALUE proj_dir = rb_iv_get(currentCoverbandBase, "@project_directory");
  const char * c_str_proj_dir = StringValueCStr(proj_dir);

  if((strstr(srcfile, "gems") == NULL) &&
     (strstr(srcfile, "internal:prelude") == NULL) &&
     (strstr(srcfile, c_str_proj_dir) != NULL) &&
     (!any_ignore_patterns( rb_iv_get(currentCoverbandBase, "@ignore_patterns"), srcfile))
    ) {
    return Qtrue;
  } else {
    return Qfalse;
  }

}



static VALUE
cb_extended(VALUE self) {
  return Qtrue;
}

void Init_coverband_ext(void)
{
  VALUE coverbandBase = rb_path2class("Coverband::Base");
  rb_define_method(coverbandBase, "extended?", cb_extended, 0);
  rb_define_method(coverbandBase, "track_file?", cb_track_file, 1);
}
