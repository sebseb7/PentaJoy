UNAME := $(shell uname)

SOURCES = $(wildcard ../apps/*.c)
BINARIES = $(SOURCES:../apps/%.c=%)

FLAGS = --std=gnu99 -Wall

ifeq ($(UNAME), Darwin)
	FLAGS += -I/Library/Frameworks/SDL.framework/Headers SDLmain.m -framework SDL -framework Cocoa
endif

ifeq ($(UNAME), Linux)
	FLAGS += -lSDL
endif

all: $(BINARIES)

clean:
	rm -f $(BINARIES)

%: ../apps/%.c main.c main.h Makefile
	@grep $@ .gitignore || echo $@ >> .gitignore # update .gitignore
	gcc $(FLAGS) main.c $< -o $@ -I.
