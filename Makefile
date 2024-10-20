# Nombre del ejecutable
TARGET = main

# Compilador
CXX = g++

# Flags del compilador
CXXFLAGS = -std=c++11 -pthread

# Archivos fuente
SRCS = main.cpp

# Archivos objeto
OBJS = $(SRCS:.cpp=.o)

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Regla para compilar los archivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos compilados
clean:
	rm -f $(TARGET) $(OBJS)