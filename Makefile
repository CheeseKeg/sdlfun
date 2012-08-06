# GNUmakefile for sdlfun
# github.com/CheeseKeg/sdlfun

ifneq (,)
This makefile requires GNU Make.
endif

# debug or release
build = release

bin = bin/
src = src/
cc = g++

programs = helloworld
#programs = $(addprefix $(bin), $(PROGRAMS))
#progexec = $(addprefix exec., $(PROGRAMS))

lflags = -lSDL -lGL -lGLU

ifeq ($(build), debug)
cflags = -Wall -O0 -g
endif

ifeq ($(build), release)
cflags = -Wall -O2
endif

.PHONY : main
main : help

.PHONY : help
help :
	@echo GNUmakefile for sdlfun
	@echo ------------------------------
	@echo github.com/CheeseKeg/sdlfun
	@echo
	@echo \* Available build options:
	@echo \*---\> all $(programs)
#	@echo \* Available execute options \(must build first\):
#	@echo \*---\> $(progexec)
	@echo \* Clean junk files and binaries:
	@echo \*---\> clean
	@echo \* View this help:
	@echo \*---\> help
	@echo
	@echo Example: make all

.PHONY : all
all : $(programs)

.PHONY : clean
clean :
	rm -f $(bin)*
	find ./ -name *~* -exec rm {} +

clink = $(cc) $(lflags) -o $@ $^

.PHONY : $(programs)

helloworld : $(bin)helloworld
$(bin)helloworld : $(bin)helloworld.o
	$(clink)

$(bin)%.o : $(src)%.cpp
	$(cc) $(cflags) -c -o $@ $<

#.PHONY : $(progexec)
#$(progexec) :
#	./$(bin)$(removeprefix exec., $@)
