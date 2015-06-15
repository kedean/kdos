#pragma once

#ifndef NULL
	#define NULL 0
#endif

typedef enum{
	false=0,
	true=1
} bool;

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned long uint64_t;

typedef uint32_t uint_t;
typedef uint32_t size_t;

typedef char* memloc_t; //a pointer to a location in memory, able to be operated on with math operators