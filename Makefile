CXX = clang++
# Might want to include -Wall eventually.
CPPFLAGS = -g -std=c++11 $(shell pkg-config --cflags opencv4 openssl)
LDFLAGS = $(shell pkg-config --libs opencv4 openssl)
SRCS = Code/CSTest.cpp Code/QueryThread.cpp Code/CardData.cpp Code/CardDatabase.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: cstest

cstest: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) cstest .depend

# Terrible include hack to make sure that if the headers change, the objects are rebuilt.
$(OBJS): $(wildcard Code/*.h)
