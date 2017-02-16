NAME = asm-syntax

INCLUDES =
OBJDIR = ../../obj/$(NAME)
BINDIR = ../../bin

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall -Wextra

LDLIBS =
LDFLAGS = $(LDLIBS)

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g

DEPS = SyntaxFileWriter.h \
       SyntaxInfo.h \
       TextMateSyntaxWriter.h

SRC = main.cpp \
      TextMateSyntaxWriter.cpp
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

TARGET = $(BINDIR)/$(NAME)

.PHONY: release
release: init $(TARGET)

.PHONY: debug
debug: init $(TARGET)

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
	rm -f $(OBJDIR)/*.o $(TARGET)
