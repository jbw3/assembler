INCLUDES =
OBJDIR = obj

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++1z

LDLIBS =
LDFLAGS = $(LDLIBS)

DEPS = Assembler.h \
       CodeGenerator.h \
       LexicalAnalyzer.h \
       SyntaxAnalyzer.h

_OBJ = Assembler.cpp.o \
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
