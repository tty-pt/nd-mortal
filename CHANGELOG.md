## 1.0.0

- **nd-mortal is now an installable library rather than a build artifact of
  the engine.** It builds and installs exactly two files,
  `lib/libnd-mortal.so` and `include/nd/mortal.h`, following the same layout
  as `axil-tty` and `axil-auth`, and the same layout `nd-core` was converted
  to first. Previously `make` produced a `mortal.so` named by the engine's
  `mods.load` and installed nothing. There is no `lib/nd-mortal.so` symlink:
  `mods.load` names this module `libnd-mortal`, the installed filename, and
  `module_load_path()` only appends `.so`.

- **The link line is libxylem alone.** `LDLIBS := -lxylem`; the engine is not
  linked. `NEEDED` is `libxylem.so` and `libc.so.6`.

- **Death, healing and feeding are declared in `<nd/mortal.h>`.**
  `on_death`, `on_murder`, `on_birth`, `on_mortal_life`,
  `on_mortal_survival`, `mortal_damage`, `mcp_hp`, `heal`, `feed` are
  `XY_DECL`'d there; this TU defines `MORTAL_IMPL` because it `XY_IMPL`s the
  same names. `on_death` is both implemented and called here, so it goes
  through this module's own registered adapter via a small `_chain` helper —
  `XY_DECL` cannot coexist with `XY_IMPL` in one TU.

- **`call_verb`/`call_verb_to` never existed** and are gone from the port;
  death and damage messages go out as `nd_printf` + `nd_rwrite` via
  `OBJ.location`.

- **Dropped the `nd-mod.mk` dependency.** This module resolves the game's
  headers itself, the way every other house library does. `nd-mod.mk` has
  now been deleted.
