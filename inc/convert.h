#ifndef CONVERT_H
#define CONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LINE_BUFFER_SIZE 86
#define FIRST_NAME "DAVID"

char *write_srec(uint8_t type, uint32_t address, const uint8_t *data, uint8_t data_len);
char *convert_to_srec(uint8_t *data, size_t data_size);
char *convert_to_assembly(uint8_t *data, size_t data_size);

#endif /* CONVERT_H */
