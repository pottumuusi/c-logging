SRC_DEMO := $(wildcard src/*.c)
INCLUDE := -Iinclude

PROGRAM_NAME := demo

# https://en.wikipedia.org/wiki/C23_(C_standard_revision)#Syntax
# > `va_start` macro no longer needs a second argument nor does it evaluate any
#   argument after the first one if present
demo: $(SRC_DEMO)
	$(CC) $(INCLUDE) -DPROGRAM_NAME=$(PROGRAM_NAME) --std=c23 -Wall --pedantic -Werror $^ -o $@
