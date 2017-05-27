NAME = asm-syntax

INCLUDES = -I../libiset
DEPDIR = .dep
OBJDIR = ../../obj/$(NAME)
LIBDIR = ../../lib
BINDIR = ../../bin

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$<.Td

CXX = g++
CXXFLAGS = $(INCLUDES) -std=c++1z -Wall -Wextra $(DEPFLAGS)

LDLIBS = -L$(LIBDIR) -liset
LDFLAGS = $(LDLIBS)

POSTCOMPILE = @mv -f $(DEPDIR)/$<.Td $(DEPDIR)/$<.d && touch $@

release: CXXFLAGS += -O2
debug: CXXFLAGS += -O0 -g

SRC = main.cpp \
	  SyntaxGenerator.cpp \
      TextMateSyntaxWriter.cpp
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

.PHONY: init
init:
	@mkdir -p $(DEPDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.cpp.d
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

# rule for dependency files (this is here to keep make happy)
$(DEPDIR)/%.d: ;

# prevent make from deleting dependency files
.PRECIOUS: $(patsubst %,$(DEPDIR)/%.d,$(SRC))

.PHONY: clean
clean:
	cd ../libiset; make clean
	rm -f $(OBJDIR)/*.o $(TARGET) $(DEPDIR)/*.d

# include dependency makefiles
include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(SRC)))
