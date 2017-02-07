INCLUDES =
OBJDIR = ../obj
BINDIR = ../bin

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall -Wextra

LDLIBS =
LDFLAGS = $(LDLIBS)

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g

DEPS = Arguments.h \
       Assembler.h \
       CodeGenerator.h \
       Error.h \
       InstructionSet.h \
       IOutputFormatter.h \
       LexicalAnalyzer.h \
       Logger.h \
       Preprocessor.h \
       SyntaxAnalyzer.h \
       SyntaxTree.h \
       TextOutputFormatter.h \
       Token.h \
       utils.h

_OBJ = Arguments.cpp.o \
       Assembler.cpp.o \
       CodeGenerator.cpp.o \
       InstructionSet.cpp.o \
       LexicalAnalyzer.cpp.o \
       Logger.cpp.o \
       main.cpp.o \
       Preprocessor.cpp.o \
       SyntaxAnalyzer.cpp.o \
       SyntaxTree.cpp.o \
       TextOutputFormatter.cpp.o \
       Token.cpp.o \
       utils.cpp.o \
       W8.cpp.o \
       W16.cpp.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

TARGET = $(BINDIR)/asm

.PHONY: release
release: init $(TARGET)

.PHONY: debug
debug: init $(TARGET)

.PHONY: init
init:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

.PHONY: test
test: release
	../test/test.py

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.cpp.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)
