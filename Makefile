# Root Makefile wrapper for src/makefile

SRC_DIR := src
SRC_MAKEFILE := makefile
DOXY_CONFIG := Doxyfile

.PHONY: all clean rebuild run test

all clean rebuild run test:
	$(MAKE) -C $(SRC_DIR) -f $(SRC_MAKEFILE) $@

# Forward any other target to src/makefile as well.
%:
	$(MAKE) -C $(SRC_DIR) -f $(SRC_MAKEFILE) $@

# Generate documentation using Doxygen
gen-doc:
	doxygen $(DOXY_CONFIG)
