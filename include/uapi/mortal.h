#ifndef MORTAL_H
#define MORTAL_H

#include <nd/type.h>

/* API */

SIC_DECL(int, mortal_damage, unsigned, killer_ref, unsigned, victim_ref, long, amt);
SIC_DECL(int, mcp_hp, unsigned, player_ref);
SIC_DECL(int, heal, unsigned, ref);
SIC_DECL(int, feed, unsigned, ref, unsigned, food, unsigned, drink);

/* SIC */

SIC_DECL(int, on_mortal_life, unsigned, player_ref, double, dt);
SIC_DECL(int, on_mortal_survival, unsigned, player_ref, double, dt);

SIC_DECL(int, on_birth, unsigned, ref, uint64_t, v);
SIC_DECL(int, on_death, unsigned, ref);

SIC_DECL(int, on_murder, unsigned, killer_ref, unsigned, ref);

#endif
