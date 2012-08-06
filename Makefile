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

programs = helloworld regulatefps rectangle keyhandling loadimage colorkey blitimage
#binaries = $(addprefix $(bin), $(programs))
#progexec = $(addprefix exec., $(programs))

lflags = -lSDL
glflags = -lGL -lGLU

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

clink = $(cc) $(lflags) -o $@

#.PHONY : $(programs)

t1 = helloworld
$(t1) : $(bin)$(t1)
$(bin)$(t1) : $(bin)$(t1).o
	$(clink) $^

t2 = regulatefps
$(t2) : $(bin)$(t2)
$(bin)$(t2) : $(bin)$(t2).o
	$(clink) $^

t3 = rectangle
$(t3) : $(bin)$(t3)
$(bin)$(t3) : $(bin)$(t3).o
	$(clink) $^

t4 = keyhandling
$(t4) : $(bin)$(t4)
$(bin)$(t4) : $(bin)$(t4).o
	$(clink) $^

t5 = loadimage
$(t5) : $(bin)$(t5)
$(bin)$(t5) : $(bin)$(t5).o
	$(clink) $^

t6 = colorkey
$(t6) : $(bin)$(t6)
$(bin)$(t6) : $(bin)$(t6).o
	$(clink) $^

t7 = blitimage
$(t7) : $(bin)$(t7)
$(bin)$(t7) : $(bin)$(t7).o
	$(clink) $^

$(bin)%.o : $(src)%.cpp
	$(cc) $(cflags) -c -o $@ $<

#.PHONY : $(progexec)
#$(progexec) :
#	./$(bin)$(removeprefix exec., $@)
