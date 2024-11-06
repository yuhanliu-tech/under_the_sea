CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Wno-c++11-narrowing
CXXFLAGS += -I../external/json/include

SRCS = POLYNOMIAL_4D.cpp QUATERNION.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = extract

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean