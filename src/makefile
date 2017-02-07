INCLUDES =
OBJDIR = ../obj
BINDIR = ../bin

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall -Wextra

LDLIBS =
LDFLAGS = $(LDLIBS)

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g
cov: CXXFLAGS += --coverage
cov: LDFLAGS += --coverage

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

_OBJ = Arguments.o \
       Assembler.o \
       CodeGenerator.o \
       InstructionSet.o \
       LexicalAnalyzer.o \
       Logger.o \
       main.o \
       Preprocessor.o \
       SyntaxAnalyzer.o \
       SyntaxTree.o \
       TextOutputFormatter.o \
       Token.o \
       utils.o \
       W8.o \
       W16.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

TARGET = $(BINDIR)/asm

.PHONY: release
release: init $(TARGET)

.PHONY: debug
debug: init $(TARGET)

.PHONY: cov
cov: debug test

.PHONY: init
init:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

.PHONY: test
test:
	../test/test.py

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/*.gcno $(OBJDIR)/*.gcda $(TARGET) *.gcov
