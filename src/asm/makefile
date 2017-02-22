NAME = asm

INCLUDES = -I../libiset
OBJDIR = ../../obj/$(NAME)
LIBDIR = ../../lib
BINDIR = ../../bin
TESTDIR = ../../test

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall -Wextra

LDLIBS = -L$(LIBDIR) -liset
LDFLAGS = $(LDLIBS)

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g
cov: CXXFLAGS += --coverage
cov: LDFLAGS += --coverage

DEPS = Arguments.h \
       Assembler.h \
       CodeGenerator.h \
       Error.h \
       ImmediateExpressionEvaluator.h \
       IOutputFormatter.h \
       LexicalAnalyzer.h \
       Logger.h \
       Preprocessor.h \
       SymbolMap.h \
       SyntaxAnalyzer.h \
       SyntaxTree.h \
       TextOutputFormatter.h \
       Token.h

SRC = Arguments.cpp \
      Assembler.cpp \
      CodeGenerator.cpp \
      ImmediateExpressionEvaluator.cpp \
      LexicalAnalyzer.cpp \
      Logger.cpp \
      main.cpp \
      Preprocessor.cpp \
      SyntaxAnalyzer.cpp \
      SyntaxTree.cpp \
      TextOutputFormatter.cpp \
      Token.cpp
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
