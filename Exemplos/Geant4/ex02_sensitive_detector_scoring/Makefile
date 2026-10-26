CXX          := g++
CXXFLAGS     := -std=c++17 -Wall -Iinclude $(shell geant4-config --cflags 2>/dev/null)
LDFLAGS      := $(shell geant4-config --libs 2>/dev/null)

TARGET       := ex02_sensitive_detector_scoring
SRCS         := main.cc src/DetectorConstruction.cc src/CalorimeterSD.cc src/CalorHit.cc src/PrimaryGeneratorAction.cc src/RunAction.cc src/EventAction.cc src/ActionInitialization.cc
OBJS         := $(SRCS:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@if [ -z "$(shell which geant4-config 2>/dev/null)" ]; then \
		echo "AVISO: geant4-config nao encontrado no PATH. Use CMake para compilar:"; \
		echo "  mkdir build && cd build && cmake .. && make"; \
	else \
		$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS); \
	fi

%.o: %.cc
	@if [ -n "$(shell which geant4-config 2>/dev/null)" ]; then \
		$(CXX) $(CXXFLAGS) -c $< -o $@; \
	else \
		touch $@; \
	fi

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
