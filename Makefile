SRC_DEMO := $(wildcard src/*.c)
INCLUDE := -Iinclude

PROGRAM_NAME := demo

demo: $(SRC_DEMO)
	$(CC) $(INCLUDE) -DPROGRAM_NAME=$(PROGRAM_NAME) --std=c23 --pedantic -Werror $^ -o $@
