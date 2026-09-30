all := libnd-mortal

LDLIBS-libnd-mortal := -lxylem

# Sibling -I for a dev build: nd-mortal's only cross-module header dependency is
# nd-attr's. In CI the siblings do not exist and both headers come from the
# installed packages named in .github/workflows/ci.yml.
CFLAGS += -I$(shell cd .. && pwd)/axil-nd/include
CFLAGS += -I$(shell cd .. && pwd)/axil-nd-attr/include

FOLDER := nd


# macOS ld rejects undefined symbols in shared libs, but WARN needs
# qsyslog: an engine-provided function pointer resolved at dlopen (Linux
# allows this by default). dynamic_lookup is the Darwin equivalent.
LDFLAGS-libnd-mortal-Darwin += -undefined dynamic_lookup
-include ./../mk/include.mk
