NAME = libiset

INCLUDES =
OBJDIR = ../../obj/$(NAME)
LIBDIR = ../../lib

CXX = clang++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall -Wextra

LD = ar
LDLIBS =
LDFLAGS = -rcs $(LDLIBS)

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g

DEPS = FormatSaver.hpp \
       InstructionSet.h \
       InstructionSetRegister.h \
       Mips32.hpp \
       Token.h \
       utils.h \
       W8.hpp \
       W16.hpp

SRC = InstructionSet.cpp \
      InstructionSetRegister.cpp \
      Token.cpp \
      utils.cpp
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

TARGET = $(LIBDIR)/$(NAME).a

.PHONY: release
release: init $(TARGET)

.PHONY: debug
debug: init $(TARGET)

.PHONY: init
init:
	mkdir -p $(OBJDIR)
	mkdir -p $(LIBDIR)

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(TARGET) $(OBJ)

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)
