STD = -std=c2x
CC ?= gcc
DESTDIR ?= /bin
RELEASE ?= 1
# debug_flags = -Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wsign-conversion -Wformat=2 -Wshadow -Wvla -Wwrite-strings -fstack-protector-all -fsanitize=address,undefined,leak -g
debug_flags = -Wall -Wextra -Werror -Wsign-conversion -Wformat=2 -Wshadow -Wvla -fstack-protector-all -fsanitize=address,undefined,leak -g
# -DNCSH_DEBUG
# release_flags = -Wall -Wextra -Werror -pedantic-errors -Wsign-conversion -Wformat=2 -Wshadow -Wvla -Wwrite-strings -O3 -DNDEBUG
release_flags = -Wall -Wextra -Werror -pedantic-errors -Wsign-conversion -Wformat=2 -Wshadow -Wvla -flto -O3 -ffast-math -march=native -DNDEBUG
# fuzz_flags = -Wall -Wextra -Werror -pedantic-errors -Wformat=2 -Wwrite-strings -fsanitize=address,leak,fuzzer -DNDEBUG -g
fuzz_flags = -Wall -Wextra -Werror -pedantic-errors -Wformat=2 -fsanitize=address,leak,fuzzer -DNDEBUG -g
objects = obj/main.o obj/arena.o obj/help.o obj/fzf.o obj/z.o
target = ./bin/z

ifeq ($(CC), gcc)
	release_flags += -s
endif

ifeq ($(RELEASE), 1)
	CFLAGS ?= $(release_flags)
	cc_with_flags = $(CC) $(STD) $(CFLAGS)
else
	CFLAGS ?= $(debug_flags)
	cc_with_flags = $(CC) $(STD) $(CFLAGS)
endif

$(target) : $(objects)
	$(cc_with_flags) -o $(target) $(objects)

obj/%.o: src/%.c
	$(cc_with_flags) -c $< -o $@

# Normal release build
release:
	make RELEASE=1

# Normal debug build
debug :
	make -B RELEASE=0

# Unity/jumbo release build
unity :
	$(CC) $(STD) $(release_flags) src/unity.c -o $(target)

u :
	make unity

# Unity/jumbo debug build
unity_debug :
	$(CC) $(STD) $(debug_flags) src/unity.c -o $(target)
ud:
	make unity_debug

# Install locally to DESTDIR (default /usr/bin/)
.PHONY: install
install : $(target)
	strip $(target)
	install -C $(target) $(DESTDIR)

# Run tests
check:
	set -e
	make test_fzf
	make test_arena
	make test_str
	make test_z
c :
	make check_local

# Run z tests
test_z :
	gcc -std=c2x -Wall -Wextra -Werror -pedantic-errors -Wformat=2 -fsanitize=address,undefined,leak -DZ_TEST ./src/arena.c ./src/z/fzf.c ./src/z/z.c ./tests/z/z_tests.c -o ./bin/z_tests
	./bin/z_tests
tz :
	make test_z

# Run z fuzzer
fuzz_z :
	chmod +x ./create_corpus_dirs.sh
	./create_corpus_dirs.sh
	clang-19 -std=c2x -Wall -Wextra -Werror -pedantic-errors -Wformat=2 -fsanitize=address,undefined,fuzzer -O3 -DNDEBUG -DZ_TEST ./src/arena.c ./tests/fuzz/z_fuzzing.c ./src/z/fzf.c ./src/z/z.c -o ./bin/z_fuzz
	./bin/z_fuzz Z_CORPUS/ -detect_leaks=0 -rss_limit_mb=8192
fz :
	make fuzz_z

# Run z add fuzzer
fuzz_z_add :
	chmod +x ./create_corpus_dirs.sh
	./create_corpus_dirs.sh
	clang-19 -std=c2x -Wall -Wextra -Werror -pedantic-errors -Wformat=2 -fsanitize=address,undefined,fuzzer -O3 -DNDEBUG -DZ_TEST ./src/arena.c ./tests/fuzz/z_add_fuzzing.c ./src/z/fzf.c ./src/z/z.c -o ./bin/z_add_fuzz
	./bin/z_add_fuzz Z_ADD_CORPUS/ -detect_leaks=0 -rss_limit_mb=8192
fza :
	make fuzz_z_add

# Run fzf tests
test_fzf :
	$(CC) $(STD) -fsanitize=address,undefined,leak -g ./src/arena.c ./src/z/fzf.c ./tests/lib/examiner.c ./tests/z/fzf_tests.c -o ./bin/fzf_tests
	@LD_LIBRARY_PATH=/usr/local/lib:./bin/:${LD_LIBRARY_PATH} ./bin/fzf_tests
tf :
	make test_fzf

# Run arena tests
test_arena :
	$(CC) $(STD) $(debug_flags) -DNCSH_HISTORY_TEST ./src/arena.c ./tests/arena_tests.c -o ./bin/arena_tests
	./bin/arena_tests
ta :
	make test_arena

# Run str tests
test_str :
	$(CC) $(STD) $(debug_flags) ./src/arena.c ./tests/eskilib/str_tests.c -o ./bin/str_tests
	./bin/str_tests
ts :
	make test_str

# Format the project
clang_format :
	find . -regex '.*\.\(c\|h\)' -exec clang-format -style=file -i {} \;
cf :
	make clang_format

# Perform static analysis on the project
.PHONY: scan_build
scan_build:
	scan-build-19 -analyze-headers make

# Clean-up
.PHONY: clean
clean :
	rm $(target) $(objects)
