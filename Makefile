RM			= rm -f
MKDIR		= mkdir -p

CPPCHECK	= cppcheck
VALGRIND	= valgrind
CLANG		= clang
SLEEP		= sleep

CXX			= g++
#CXXFLAGS	= -std=c++17 -Wall -Wextra -Wpedantic -Og -DTIPOLOW_DEBUG=1 -g
CXXFLAGS	= -std=c++17 -Wall -Wextra -Wpedantic -O2 -DTIPOLOW_DEBUG=1
LDFLAGS		= -Isrc

# Testing libraries and stuff.
CRAYLIB		= -lraylib
# Testing free-standing environment.
FSCXX		= x86_64-elf-g++
FSCXXFLAGS	= -std=c++17 -Wall -Wextra -Wpedantic -Og
FSLDFLAGS	= -ffreestanding -fno-exceptions -fno-rtti -nostdlib -lgcc -Isrc

clean:
	$(RM) \
		./out/test-freestanding.o \
		./out/test-std.o

check:
	$(CPPCHECK) --language=c++ --std=c++17 ./test/std.c++
	$(CLANG) --analyze -Xclang -analyzer-output=html -Isrc \
		-o ./out/analysis \
		./test/std.c++

test-freestanding:
	$(FSCXX) $(FSCXXFLAGS) \
		-o ./out/test-freestanding.o ./test/freestanding.c++ \
		$(FSLDFLAGS)

test-std:
	$(CXX) $(CXXFLAGS) \
		-o ./out/test-std.o ./test/std.c++ \
		$(LDFLAGS)
	
	$(SLEEP) 1
	
	$(VALGRIND) \
		--leak-check=full \
		--track-origins=yes \
		--show-leak-kinds=all \
		./out/test-std.o
# --verbose
