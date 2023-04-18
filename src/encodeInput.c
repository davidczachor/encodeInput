#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>
#include "../inc/utils.h"
#include "../inc/convert.h"

int main(int argc, char *argv[])
{
  program_options options;
  parse_arguments(argc, argv, &options);

  // Show help if requested
  if (options.show_help)
  {
    show_help(argv[0]);
    return EXIT_FAILURE;
  }

  // Generate output filename if not specified
  if (options.input_filename && !options.output_filename)
  {
    options.output_filename = generate_output_filename(options.input_filename, options.srec_format);

    // Check if output filename was generated successfully
    if (!options.output_filename)
    {
      return EXIT_FAILURE;
    }
  }

  // Buffer for input data
  uint8_t buffer[BUFFER_SIZE];
  size_t bytes_read = 0;

  // Read input data
  if (options.input_filename)
  {
    FILE *input_file = fopen(options.input_filename, "rb");

    // Check if input file was opened successfully
    if (!input_file)
    {
      printf("Error: Unable to open the input file: %s\n", options.input_filename);
      return EXIT_FAILURE;
    }

    bytes_read = fread(buffer, 1, BUFFER_SIZE, input_file);
    fclose(input_file);
  }
  else
  {
    // Read input from stdin into buffer
    int c;
    while ((c = getchar()) != EOF && bytes_read < BUFFER_SIZE)
    {
      buffer[bytes_read++] = (uint8_t)c;
    }
  }

  // Convert input data to assembly or S-Record format
  char *output_data;
  if (options.srec_format)
  {
    output_data = convert_to_srec(buffer, bytes_read);
  }
  else
  {
    output_data = convert_to_assembly(buffer, bytes_read);
  }

  if (options.output_filename)
  {
    // Write to file
    FILE *output_file = fopen(options.output_filename, "w");
    // Check if output file was opened successfully
    if (!output_file)
    {
      printf("Error: Unable to open the output file: %s\n", options.output_filename);
      free(output_data);
      return EXIT_FAILURE;
    }
    // Write output data to file
    fprintf(output_file, "%s", output_data);
    fclose(output_file);
  }
  else
  {
    // Write to stdout
    printf("%s", output_data);
  }

  // Free memory
  free(output_data);
  return EXIT_SUCCESS;
}
