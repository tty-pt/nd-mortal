/* mortal-types.h — nd-mortal's types, enums, and structures */

#ifndef ND_MORTAL_TYPES_H
#define ND_MORTAL_TYPES_H

#include <stdint.h>

enum huth {
	HUTH_THIRST = 0,
	HUTH_HUNGER = 1,
};

typedef struct {
	long hp;
	unsigned short huth[2];
	unsigned char huth_n[2];
	long damage;
} mortal_t;

#endif /* !ND_MORTAL_TYPES_H */
