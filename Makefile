CC := musl-gcc
CFLAGS := -std=c2x -O2 -static

YEARS := 2022
DAYS_2022 := 14

#

MAKEFLAGS += -rR

_COMMON = $(wildcard lib/main.c)
_YEAR_DAYS := $(foreach year,$(YEARS),$(foreach year_day,$(shell seq -w 01 $(DAYS_$(year))),$(year)/day-$(year_day)))
_EXECUTABLES := $(sort $(_YEAR_DAYS:%=bin/%-1) $(_YEAR_DAYS:%=bin/%-2))


.PHONY: all
all: bin

.PHONY: bin
bin: $(_EXECUTABLES)

bin/%: %.c
	@mkdir -p $(@D)
	@echo -e "CC\t$(filter %.c,$(^))"
	@$(CC) -o $(@) $(CFLAGS) $(filter %.c,$(^))

$(_EXECUTABLES): Makefile

.PHONY: run $(_EXECUTABLES:%=run/%)
run: $(_EXECUTABLES:bin/%=run/%) | $(_EXECUTABLES)

run/%: bin/%
	@echo "================ $(subst run/,,$(@))"
	@$(^) <$(patsubst bin/%-2,%,$(patsubst bin/%-1,%,$(^))).txt
	@echo

.PHONY: clean
clean:
	@rm -rvf bin/
