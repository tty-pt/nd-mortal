# axil-nd-mortal

`nd-mortal` for [axil-nd](../axil-nd), ported from SIC to libxylem.

Owns mortality: hit points, death, healing and feeding. It implements the
`mcp_hp` bar, `mortal_damage`, `heal` and `feed`, tracks the `mortal` table,
and fires the `on_death` chain that fight, spell and the corpse path consume.

## Install

```sh
make install
```

Installs:

```
lib/libnd-mortal.so
include/nd/mortal.h
```

There is deliberately no `lib/nd-mortal.so` symlink (see `axil-nd-wts` for
why: `mods.load` names the installed filename, and the OpenBSD packing list
never lists a symlink).

Also packaged for deb, apk, rpm, brew and openbsd from a `v*` tag.

## Build from source

```sh
make
```

Needs [libxylem](https://github.com/tty-pt/libxylem) and the engine's game
API, `<nd/xy.h>`, plus the `<nd/attr.h>` it co-implements against — from
checkouts beside this repo or from installed packages:

```sh
git clone https://github.com/tty-pt/nd-mortal && cd nd-mortal
git clone https://github.com/tty-pt/axil-nd ../axil-nd
git clone https://github.com/tty-pt/nd-attr ../axil-nd-attr
make
```

Both the checkout `-I` flags and the installed-package paths are on the
command line at once (see `Makefile`), and a missing `-I` is ignored, so the
same command works either way. CI names the deps explicitly
(`axil-nd,libxylem,nd-attr`).

## What it does

* `xy_install()` registers the `mortal` table and the HP/mana bar plumbing.
* `mcp_hp`, `mortal_damage`, `heal`, `feed` are the provider bodies;
  `on_death` is the chain killers amend (`XY_DECL`'d in `<nd/mortal.h>` along
  with `on_murder`, `on_birth`, `on_mortal_life`, `on_mortal_survival`).
* `on_add` initializes the row for a new entity; `on_status` and
  `on_examine` report the living state.

## Testing

There is no `test.sh` here. Behaviour is asserted by the engine's own suite:

```sh
cd ../axil-nd
make && ./test.sh
```

## Notes from the port

* `SIC_DEF` → `XY_IMPL`, `mod_install` → `xy_install`, `call_f(...)` →
  `f(...)`. `call_verb`/`call_verb_to` never existed (the engine confirms
  it); death and damage messages go out as `nd_printf` + `nd_rwrite` via
  `OBJ.location`.
* This module both implements and calls `on_death`, and `XY_DECL` cannot
  coexist with `XY_IMPL` in one TU, so it dispatches through its own
  registered adapter via a small `_chain` helper placed after the `XY_IMPL`.
* It `XY_IMPL`s names from `<nd/mortal.h>`, so it defines `MORTAL_IMPL`
  before including it.
* The link line is libxylem alone. `NEEDED` is `libxylem.so` and `libc.so.6`.

## License

BSD 2-Clause, carried over from `tty-pt/nd-mortal`. See `LICENSE`.
