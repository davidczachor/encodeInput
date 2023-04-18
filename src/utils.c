#include "../inc/utils.h"

/*
 * FUNCTION      : void parse_arguments(int argc, char *argv[], program_options *options)
 * DESCRIPTION   : This function parses the command line arguments and populates the program_options struct.
 *                 It validates the length of the input and output filenames, and exits with an error message
 *                 if either of them are too long. It also validates the 's' option and shows the help message
 *                 if the argument is invalid.
 * PARAMETERS    :
 *   int argc - The number of arguments passed to the program.
 *   char *argv[] - An array of strings containing the command line arguments.
 *   program_options *options - A pointer to the program_options struct to be populated with the parsed values.
 * RETURNS       :
 *   None.
 */
void parse_arguments(int argc, char *argv[], program_options *options)
{
  // Initialize the options with default values
  options->input_filename = NULL;
  options->output_filename = NULL;
  options->srec_format = 0;
  options->show_help = 0;

  // Parse command line options
  int opt;
  while ((opt = getopt(argc, argv, "i:o:s:h")) != -1)
  {
    // Check if the option is valid
    switch (opt)
    {
    case 'i':
      // Check if input filename length is valid
      if (strlen(optarg) > MAX_FILENAME_LEN)
      {
        printf("Error: Input filename is too long.\n");
        exit(EXIT_FAILURE);
      }
      options->input_filename = optarg;
      break;
    case 'o':
      // Check if output filename length is valid
      if (strlen(optarg) > MAX_FILENAME_LEN)
      {
        printf("Error: Output filename is too long.\n");
        exit(EXIT_FAILURE);
      }
      options->output_filename = optarg;
      break;
    case 's':
      if (strcmp(optarg, "rec") == 0)
      {
        options->srec_format = 1;
      }
      else
      {
        show_help(argv[0]);
        exit(EXIT_FAILURE);
      }
      break;
    case 'h':
      options->show_help = 1;
      break;
    default:
      // Invalid option
      show_help(argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}

/*
 * FUNCTION      : void show_help(char *program_name)
 * DESCRIPTION   : This function prints the help message for the program, which provides information
 *                 on how to use the command line arguments.
 * PARAMETERS    :
 *   char *program_name - A string containing the name of the program.
 * RETURNS       :
 *   None.
 */
void show_help(char *program_name)
{
  printf("Usage: %s [-iInput_filename] [-oOutput_filename] [-s] [-h]\n", program_name);
  printf("  -iInput_filename    Specifies the input file. If not specified, read from stdin.\n");
  printf("  -oOutput_filename   Specifies the output file. If not specified, write to stdout.\n");
  printf("  -srec               Output in the S-Record format. Default is assembly file format.\n");
  printf("  -h                  Show this help message and exit.\n");
}

/*
 * FUNCTION      : char *generate_output_filename(char *input_filename, int srec_format)
 * DESCRIPTION   : This function generates the output filename based on the input filename and the specified format.
 *                 If the srec_format parameter is true, it appends the SREC file extension to the input filename;
 *                 otherwise, it appends the assembly file extension. The generated filename is stored in a static buffer.
 * PARAMETERS    :
 *   char *input_filename - A string containing the input filename.
 *   int srec_format - An integer flag indicating whether to use the SREC file format (1) or the assembly file format (0).
 * RETURNS       :
 *   On success, the function returns a pointer to the generated output filename buffer.
 *   On error, the function returns NULL.
 */
char *generate_output_filename(char *input_filename, int srec_format)
{
  static char output_filename_buffer[MAX_FILENAME_LEN];

  // Ensure the input filename is not too long
  if (strlen(input_filename) + strlen(".srec") > MAX_FILENAME_LEN)
  {
    printf("Error: input filename is too long to append file extension for output filename.\n");
    return NULL;
  }

  // Append the file extension to the input filename
  char *extension = srec_format ? SREC_FORMAT : ASSEMBLY_FORMAT;
  strcpy(output_filename_buffer, input_filename);
  strcat(output_filename_buffer, extension);

  return output_filename_buffer;
}