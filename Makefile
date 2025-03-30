SRC_DEMO := $(wildcard src/*.c)
INCLUDE := -Iinclude

demo: $(SRC_DEMO)
	$(CC) $(INCLUDE) $^ -o $@
