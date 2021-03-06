NAME = libiset

INCLUDES =
DEPDIR = .dep
OBJDIR = ../../obj/$(NAME)
LIBDIR = ../../lib

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$<.Td

CXX = g++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall -Wextra $(DEPFLAGS)

LD = ar
LDLIBS =
LDFLAGS = -rcs $(LDLIBS)

POSTCOMPILE = @mv -f $(DEPDIR)/$<.Td $(DEPDIR)/$<.d && touch $@

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g

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
	@mkdir -p $(DEPDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBDIR)

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(TARGET) $(OBJ)

$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.cpp.d
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

# rule for dependency files (this is here to keep make happy)
$(DEPDIR)/%.d: ;

# prevent make from deleting dependency files
.PRECIOUS: $(patsubst %,$(DEPDIR)/%.d,$(SRC))

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(TARGET) $(DEPDIR)/*.d

# include dependency makefiles
include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(SRC)))
