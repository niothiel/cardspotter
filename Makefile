CXX = g++
# Might want to include -Wall eventually.
CXXFLAGS = -std=c++11 -g $(shell pkg-config --cflags openssl)
TARGET = main
SRCS = Code/CSTest.cpp Code/QueryThread.cpp Code/CardData.cpp Code/CardDatabase.cpp
OBJS = $(SRCS:.cpp=.o)


Code/%.o: Code/%.cpp
	$(CXX) $(CXXFLAGS) $(shell pkg-config --cflags opencv4) -c $< -o $@

cstest: $(OBJS)
	echo "Objects: $(OBJS)"
	$(CXX) $(CXXFLAGS) -fno-eliminate-unused-debug-symbols -o cstest $(OBJS) $(shell pkg-config --libs --cflags opencv4 openssl)