/* mortal.h — nd-mortal's cross-module API: hit points, hunger/thirst, death
 * and the mortal lifecycle hooks other modules drive.
 *
 * Caller-facing header. Implementers do NOT include this header.
 */

#ifndef ND_MORTAL_H
#define ND_MORTAL_H

#include <ttypt/xy.h>

/* API */
XY_DECL(int, mortal_damage, unsigned, killer_ref, unsigned, victim_ref, long, amt);
XY_DECL(int, mcp_hp, unsigned, player_ref);
XY_DECL(int, heal, unsigned, ref);
XY_DECL(int, feed, unsigned, ref, unsigned, food, unsigned, drink);

/* SIC — lifecycle hooks nd-mortal fires and other modules implement.
 * on_birth has no implementor in-tree; the dispatch finds nothing and
 * returns 0. */
XY_DECL(int, on_mortal_life, unsigned, player_ref, double, dt);
XY_DECL(int, on_mortal_survival, unsigned, player_ref, double, dt);

XY_DECL(int, on_birth, unsigned, ref, uint64_t, v);
XY_DECL(int, on_death, unsigned, ref);

XY_DECL(int, on_murder, unsigned, killer_ref, unsigned, ref);

#endif /* !ND_MORTAL_H */
