APP      = objview
ARGS     = ../data/cow.obj
#_________________ Directory Structure
SRCEXT   = cpp
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
BAKDIR   = bak


export PATH:=/home/kary/scripts/:$(PATH)
SHELL = bash

SRCS    := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
HFILES  := $(shell find $(SRCDIR) -name '*.h')
SRCDIRS := $(shell find . -name '*.$(SRCEXT)' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.$(SRCEXT),$(OBJDIR)/%.o,$(SRCS))

DEBUG    = -g
INCLUDES = -I./external
CFLAGS   = -Wall -pedantic -ansi -c $(DEBUG) $(INCLUDES)
LDFLAGS  = -lGLEW -lglut -lGLU


#_________________________DO NOT EDIT ANYTHING BELOW THIS POINT RE
ifeq ($(SRCEXT), cpp)
CC       = $(CXX)
else
CFLAGS  += -std=gnu99
endif

.PHONY: run clean distclean



all: $(BINDIR)/$(APP)
	@echo "Executing..."
	@cd $(BINDIR); ./$(APP) $(ARGS) 

$(BINDIR)/$(APP): buildrepo $(OBJS)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.$(SRCEXT) $(HFILES)
	@echo "Generating dependencies for $<..."
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@echo "Removing object files..."
	@$(RM) -rfv $(OBJDIR)

distclean: clean
	@echo "Removing bin directory..."
	$(RM) -rfv $(BINDIR)

buildrepo:
	@$(call make-repo)

cleanautosaved:
	@echo "Cleaning up auto saved files..."
	@$(RM) -fv $(SRCDIR)/*~

backup: cleanautosaved
	@mkdir -p $(BAKDIR)
	$(shell date +%Y.%m.%d_%H.%M.%S > .tmp)
	@echo "Compressing source files folder..."
	@cd $(BAKDIR);\
	tar -czf $(APP)_$(shell cat .tmp).tar.gz ../$(SRCDIR) ../Makefile;\
	echo "Emailing archive...";\
	email $(APP)_$(shell cat .tmp).tar.gz
	@$(RM) -fv .tmp

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef


# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFLAGS) \
        $1
endef