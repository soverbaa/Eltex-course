#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <sys/types.h>
#include <sys/stat.h>

int parse_permissions(const char *input, mode_t *permissions);

void print_permissions(mode_t permissions);

int modify_permissions(mode_t *permissions, const char *command);

#endif
