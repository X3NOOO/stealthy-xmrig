#ifndef PREPARE_CONFIG_H
#define PREPARE_CONFIG_H

#include "prepare_config.cpp"

char **split_string(const char *input, int *count);
void free_split_string(char **arr, int count);

#endif // PREPARE_CONFIG_H