#include "../inc/convert.h"

/*
 * FUNCTION      : char *write_srec(uint8_t type, uint32_t address, const uint8_t *data, uint8_t data_len)
 * DESCRIPTION   : This function writes an SREC record to a buffer based on the provided type, address, and data.
 *                 It calculates the count, sum, and checksum values based on the record type and data length.
 *                 The output is stored in a dynamically allocated buffer.
 * PARAMETERS    :
 *   uint8_t type - An integer indicating the type of SREC record to write.
 *   uint32_t address - An unsigned 32-bit integer indicating the address of the record.
 *   const uint8_t *data - A pointer to an array of unsigned 8-bit integers containing the data to write in the record.
 *   uint8_t data_len - An unsigned 8-bit integer indicating the length of the data array.
 * RETURNS       :
 *   On success, the function returns a pointer to the dynamically allocated buffer containing the SREC record.
 *   On error (e.g., invalid type), the function returns NULL.
 */
char *write_srec(uint8_t type, uint32_t address, const uint8_t *data, uint8_t data_len)
{
  uint8_t count;
  uint32_t sum;
  uint8_t checksum;

  // Calculate the count (CC) value based on the record type and data length
  switch (type)
  {
  case 0:
  case 1:
    count = data_len + 3;
    break;
  case 5:
  case 9:
    count = 3;
    break;
  default:
    // Invalid type, so return without writing anything
    return NULL;
  }

  // Initialize the sum variable with the count value
  sum = count;

  // Add the address bytes to the sum, taking into account the record type
  if (type == 1)
  {
    sum += ((address >> 16) & 0xFF) + ((address >> 8) & 0xFF) + (address & 0xFF);
  }
  else
  {
    sum += ((address >> 8) & 0xFF) + (address & 0xFF);
  }

  // Add the data bytes to the sum
  for (int i = 0; i < data_len; i++)
  {
    sum += data[i];
  }

  // Calculate the checksum (MM) by inverting the least significant byte of the sum
  checksum = ~sum & 0xFF;

  // Allocate a buffer to store the output
  size_t buffer_size = 10 + (data_len * 2);
  char *buffer = (char *)malloc(buffer_size);

  // Write the record to the buffer
  int position = sprintf(buffer, "S%1X%02X%04X", type, count, address);

  // Write the data bytes to the buffer, if present
  for (int i = 0; i < data_len; i++)
  {
    position += sprintf(&buffer[position], "%02X", data[i]);
  }

  // Write the checksum to the buffer and finish with a newline
  sprintf(&buffer[position], "%02X\n", checksum);

  return buffer;
}

/*
 * FUNCTION      : char *convert_to_srec(uint8_t *data, size_t data_size)
 * DESCRIPTION   : This function converts the provided binary data to an S-Record format.
 *                 It processes the data in chunks and writes S1 records with a maximum length of 16 bytes.
 *                 It also writes an S5 record with the total count of S1 records, and an S9 record to signal
 *                 the end of the S-Record file. The output is stored in a dynamically allocated buffer.
 * PARAMETERS    :
 *   uint8_t *data - A pointer to an array of unsigned 8-bit integers containing the binary data to convert.
 *   size_t data_size - The size of the data array.
 * RETURNS       :
 *   On success, the function returns a pointer to the dynamically allocated buffer containing the converted
 *   S-Record data.
 *   On error (e.g., invalid SREC record), the function returns NULL.
 */

char *convert_to_srec(uint8_t *data, size_t data_size)
{
  size_t remaining = data_size;
  uint32_t address = 0;
  uint8_t data_len;
  uint32_t s1_count = 0;

  // Allocate a buffer to store the output
  char *result_buffer = (char *)malloc(data_size * 16 + 64);
  result_buffer[0] = '\0';

  char *srec_line = write_srec(0, 0, (const uint8_t *)FIRST_NAME, strlen(FIRST_NAME));
  strcat(result_buffer, srec_line);
  free(srec_line);

  // Process input data in chunks and write S1 records
  for (size_t index = 0; remaining > 0; index += data_len, address += data_len, remaining -= data_len, s1_count++)
  {
    // Calculate the length of data for the current record, limited to 16 bytes
    data_len = remaining > 16 ? 16 : remaining;

    // Write an S1 record with the calculated data length and current address
    srec_line = write_srec(1, address, &data[index], data_len);
    strcat(result_buffer, srec_line);
    free(srec_line);
  }

  // Write an S5 record with the total count of S1 records
  srec_line = write_srec(5, s1_count, NULL, 0);
  strcat(result_buffer, srec_line);
  free(srec_line);

  // Write an S9 record to signal the end of the S-Record file
  srec_line = write_srec(9, 0, NULL, 0);
  strcat(result_buffer, srec_line);
  free(srec_line);

  return result_buffer;
}

/*
 * FUNCTION      : char *convert_to_assembly(uint8_t *data, size_t data_size)
 * DESCRIPTION   : This function converts the provided binary data to an assembly language format.
 *                 It processes the data in chunks of 16 bytes and writes a line for each chunk containing
 *                 a sequence of DC.B statements, each representing a byte of data in hexadecimal format.
 *                 The output is stored in a dynamically allocated buffer.
 * PARAMETERS    :
 *   uint8_t *data - A pointer to an array of unsigned 8-bit integers containing the binary data to convert.
 *   size_t data_size - The size of the data array.
 * RETURNS       :
 *   On success, the function returns a pointer to the dynamically allocated buffer containing the converted
 *   assembly language code.
 *   On error (e.g., unable to allocate memory), the function returns NULL.
 */
char *convert_to_assembly(uint8_t *data, size_t data_size)
{
  size_t i, j;
  char *result_buffer = malloc(data_size * 16);
  // If unable to allocate memory, return NULL
  if (result_buffer == NULL)
  {
    fprintf(stderr, "Error: Unable to allocate memory for result buffer\n");
    return NULL;
  }
  // Initialize the result buffer to an empty string
  result_buffer[0] = '\0';

  // Process input data in chunks of 16 bytes
  for (i = 0; i < data_size; i += 16)
  {
    char line_buffer[LINE_BUFFER_SIZE] = "dc.b\t";
    size_t line_data_len = (i + 16 > data_size) ? (data_size - i) : 16;

    for (j = 0; j < line_data_len; j++)
    {
      // Append each byte in hexadecimal format to the line buffer
      snprintf(line_buffer + strlen(line_buffer), sizeof(line_buffer) - strlen(line_buffer), "$%02X%s", data[i + j], (j == line_data_len - 1) ? "\n" : ", ");
    }

    // Append the line buffer to the result buffer
    strcat(result_buffer, line_buffer);
  }

  // Return the result buffer
  return result_buffer;
}
