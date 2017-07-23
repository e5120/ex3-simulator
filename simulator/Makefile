PROGRAM := ../asm/ex3_asm
SOURCES := $(wildcard *.cpp)
OBJS := $(SOURCES:.cpp=.o)
DEPENDS := $(SOURCES:.cpp=.depend)
CXX := clang++
CPPFLAGS := -O2 -Wall -g

.PHONY: all
all: $(PROGRAM)
$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $(CPPFLAGS) $^


%.depend: %.cpp
	@echo generating $@
	@$(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $< | sed "s/\($*\)\.o[ :]*/\1.o $@ : /g" > $@; [ -s $@ ] || rm -f $@'


ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPENDS)
endif

.PHONY : clean
clean:
	rm -f $(PROGRAM)
	rm -fr $(OBJS)
	rm -fr $(DEPENDS)
