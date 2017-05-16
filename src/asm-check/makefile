NAME = asm-check

INCLUDES = -I../libiset
OBJDIR = ../../obj/$(NAME)
LIBDIR = ../../lib
BINDIR = ../../bin

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++14 -Wall -Wextra

LDLIBS = -L$(LIBDIR) -liset
LDFLAGS = $(LDLIBS)

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g

DEPS = Checker.h

SRC = main.cpp \
      Checker.cpp
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

TARGET = $(BINDIR)/$(NAME)

.PHONY: release
release: init libs_r $(TARGET)

.PHONY: debug
debug: init libs_d $(TARGET)

.PHONY: libs_r
libs_r:
	cd ../libiset; make release

.PHONY: libs_d
libs_d:
	cd ../libiset; make debug

.PHONY: init
init:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	cd ../libiset; make clean
	rm -f $(OBJDIR)/*.o $(TARGET)
