NAME = asm

INCLUDES = -I../libiset
DEPDIR = .d
OBJDIR = ../../obj/$(NAME)
LIBDIR = ../../lib
BINDIR = ../../bin
TESTDIR = ../../test

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall -Wextra $(DEPFLAGS)

LDLIBS = -L$(LIBDIR) -liset
LDFLAGS = $(LDLIBS)

POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g
cov: CXXFLAGS += --coverage
cov: LDFLAGS += --coverage

SRC = Arguments.cpp \
      Assembler.cpp \
      BinaryOutputFormatter.cpp \
      CodeGenerator.cpp \
      ImmediateExpressionEvaluator.cpp \
      LexicalAnalyzer.cpp \
      Logger.cpp \
      main.cpp \
      Preprocessor.cpp \
      SyntaxAnalyzer.cpp \
      SyntaxTree.cpp \
      TextOutputFormatter.cpp
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
	mkdir -p $(DEPDIR)
	mkdir -p $(BINDIR)

.PHONY: test
test:
	$(TESTDIR)/test.py

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRC)))

.PHONY: clean
clean:
	cd ../libiset; make clean
	rm -f $(OBJDIR)/*.o $(OBJDIR)/*.gcno $(OBJDIR)/*.gcda $(TARGET) *.gcov $(DEPDIR)/*.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRC))))
