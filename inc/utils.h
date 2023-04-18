#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>
#include "../inc/common.h"

#define MAX_FILENAME_LEN 255
#define ASSEMBLY_FORMAT ".asm"
#define SREC_FORMAT ".srec"

void parse_arguments(int argc, char *argv[], program_options *options);
void show_help(char *program_name);
char *generate_output_filename(char *input_filename, int srec_format);

#endif /* UTILS_H */
