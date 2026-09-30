/* mortal.h — nd-mortal's cross-module API: hit points, hunger/thirst, death
 * and the mortal lifecycle hooks other modules drive.
 *
 * Include this from a module TU that wants to damage, heal or feed a mortal,
 * or to fire the lifecycle hooks, and NOT from nd-mortal's own
 * src/libnd-mortal.c without MORTAL_IMPL: an XY_IMPL and an XY_DECL of the
 * same name in one TU collide, which is the direct replacement for the old
 * `SIC_DECL` + `SIC_DEF` pairing in a single file.
 *
 * Usage:
 *
 *     #include <ttypt/xy-mod.h>     // must come first: injects the xy context
 *     #include <nd/xy.h>            // engine service hooks (nd_get, ...)
 *     #include <nd/mortal.h>        // this file
 *
 * The consumer does not need to load nd-mortal itself -- the engine loads every
 * module in mods.load into one region and XY dispatches by name -- but the
 * engine's mods.load must list mortal, or these forward to a provider that is
 * not there.
 *
 * NOTE: this is a MODULE-OWNED header, not an engine one. The old location was
 * `include/uapi/mortal.h`; the old `~/nd/module.mk` installed it as
 * `$(PREFIX)/include/nd/mortal.h`, so `nd/` is this header's home and it is
 * installed here with `FOLDER := nd`.
 *
 * The old header included `<nd/type.h>`, a file that no longer exists. Its only
 * live content for this module was SIC_DECL/SIC_DEF/SIC_CALL (all now XY_*);
 * the types a consumer needs come from `<nd/xy.h>`. Nothing here needs to
 * include it.
 */

#ifndef ND_MORTAL_H
#define ND_MORTAL_H

#include <ttypt/xy.h>

#ifndef MORTAL_IMPL

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

#endif /* !MORTAL_IMPL */

#endif /* !ND_MORTAL_H */