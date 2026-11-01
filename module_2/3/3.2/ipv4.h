
#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>
#include <stddef.h>

int parse_ipv4(const char *text, uint32_t *address);
int is_valid_mask(uint32_t mask);
int same_subnet(uint32_t address1, uint32_t address2,
                uint32_t mask);
int format_ipv4(uint32_t address, char *buffer,
                size_t buffer_size);

#endif
