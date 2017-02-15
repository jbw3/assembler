NAME = asm

INCLUDES =
OBJDIR = ../../obj/$(NAME)
BINDIR = ../../bin
TESTDIR = ../../test

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

SRC = Arguments.cpp \
      Assembler.cpp \
      CodeGenerator.cpp \
      InstructionSet.cpp \
      LexicalAnalyzer.cpp \
      Logger.cpp \
      main.cpp \
      Preprocessor.cpp \
      SyntaxAnalyzer.cpp \
      SyntaxTree.cpp \
      TextOutputFormatter.cpp \
      Token.cpp \
      utils.cpp \
      W8.cpp \
      W16.cpp
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

TARGET = $(BINDIR)/$(NAME)

.PHONY: release
release: init $(TARGET)

.PHONY: debug
debug: init $(TARGET)

.PHONY: cov
cov: debug test
	llvm-cov gcov -o $(OBJDIR) $(SRC)

.PHONY: init
init:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

.PHONY: test
test:
	$(TESTDIR)/test.py

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/*.gcno $(OBJDIR)/*.gcda $(TARGET) *.gcov
