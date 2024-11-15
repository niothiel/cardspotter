CXX = g++
# Might want to include -Wall eventually.
CXXFLAGS = -g -std=c++11 $(shell pkg-config --cflags opencv4 openssl)
SRCS = Code/CSTest.cpp Code/QueryThread.cpp Code/CardData.cpp Code/CardDatabase.cpp
OBJS = $(SRCS:.cpp=.o)


Code/%.o: Code/%.cpp Code/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

cstest: $(OBJS)
	echo "Objects: $(OBJS)"
	$(CXX) $(CXXFLAGS) -o cstest $(OBJS) $(shell pkg-config --libs --cflags opencv4 openssl)

.PHONY: clean
clean:
	rm -f $(OBJS) cstest

all: cstest
