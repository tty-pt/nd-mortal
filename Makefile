all := libnd-mortal

LDLIBS-libnd-mortal := -lxylem

# Sibling -I for a dev build: nd-mortal's only cross-module header dependency is
# nd-attr's. In CI the siblings do not exist and both headers come from the
# installed packages named in .github/workflows/ci.yml.
CFLAGS += -I$(shell cd .. && pwd)/axil-nd/include
CFLAGS += -I$(shell cd .. && pwd)/axil-nd-attr/include

FOLDER := nd

-include ./../mk/include.mk
