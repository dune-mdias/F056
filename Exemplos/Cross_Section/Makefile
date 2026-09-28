# Makefile para compilar cross_section_v1.cpp com Pythia8

# Configurações do Pythia8
export PYTHIA8      := $(HOME)/pythia8317/
export PYTHIA8DATA  := $(PYTHIA8)/share/Pythia8/xmldoc
export LD_LIBRARY_PATH := $(PYTHIA8)/lib:$(LD_LIBRARY_PATH)

# Compilador e flags
CXX      := g++
CXXFLAGS := -O2 -std=c++11 -I$(PYTHIA8)/include
LDFLAGS  := -L$(PYTHIA8)/lib -lpythia8 -ldl -Wl,-rpath,$(PYTHIA8)/lib

# Arquivos
SRC      := cross_section_v1.cpp
TARGET   := wz

# Regra padrão
all: $(TARGET)

# Regra de link/compilação
$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Limpeza
clean:
	rm -f $(TARGET)

# Executar
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run