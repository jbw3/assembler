INCLUDES =
OBJDIR = obj

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall

LDLIBS =
LDFLAGS = $(LDLIBS)

DEPS = Assembler.h \
       Error.h \
       ICodeGenerator.h \
       InstructionSet.h \
       InstructionSet8Bit.h \
       LexicalAnalyzer.h \
       Preprocessor.h \
       SyntaxAnalyzer.h \
       TextCodeGenerator.h \
       tokens.h

_OBJ = Assembler.cpp.o \
       InstructionSet.cpp.o \
       LexicalAnalyzer.cpp.o \
       Preprocessor.cpp.o \
       SyntaxAnalyzer.cpp.o \
       TextCodeGenerator.cpp.o \
       main.cpp.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

TARGET = asm

.PHONY: all
all: init $(TARGET)

.PHONY: init
init:
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.cpp.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)
