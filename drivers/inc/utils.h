#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);

void int2string(int num, char* str);
void float2string(float num, int decimals, char* str);
int string_length(const char* str);
int concat_string(char* dest, size_t dest_size, const char* src);
int string_compare(const char* s1, const char* s2);
int vsconcat_strings(char* dest, size_t dest_size, const char* first, va_list args);
int atoi(const char* s);

#endif
