#ifndef MORTAL_H
#define MORTAL_H

#include <nd/type.h>
#include <math.h>
#define G(x) xsqrtx(x)
#define HP_G(v) 10 * G(v)
#define HP_MAX(mortal) ((unsigned short) HP_G((mortal)->attr[ATTR_CON]))

/* DATA */

enum huth {
	HUTH_THIRST = 0,
	HUTH_HUNGER = 1,
};

typedef struct {
	unsigned short hp;
	unsigned short huth[2];
        unsigned char huth_n[2], damage;
	unsigned attr[ATTR_MAX];
} mortal_t;

/* API */

SIC_DECL(int, mortal_damage, unsigned, killer_ref, unsigned, victim_ref, short, amt)
SIC_DECL(int, mcp_hp, unsigned, player_ref)

/* SIC */

SIC_DECL(int, on_mortal_life, unsigned, player_ref, double, dt)
SIC_DECL(int, on_mortal_survival, unsigned, player_ref, double, dt)

SIC_DECL(int, on_birth, unsigned, ref, uint64_t, v)
SIC_DECL(int, on_death, unsigned, ref)

SIC_DECL(int, on_murder, unsigned, killer_ref, unsigned, ref)

/* OTHER THINGS */

static inline unsigned
xsqrtx(unsigned x)
{
	return x * sqrt(x);
}

#endif
