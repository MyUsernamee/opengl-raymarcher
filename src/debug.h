#pragma once
#include <stdio.h>

#define DEBUG

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) printf("%s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) /* Do nothing */
#endif