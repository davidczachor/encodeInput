#ifndef COMMON_H
#define COMMON_H

#define BUFFER_SIZE 4096

typedef struct
{
  char *input_filename;
  char *output_filename;
  int srec_format;
  int show_help;
} program_options;

#endif /* COMMON_H */
