CXX = clang++
# Might want to include -Wall eventually.
CPPFLAGS = -g -std=c++11 $(shell pkg-config --cflags opencv4 openssl asio)
LDFLAGS = $(shell pkg-config --libs opencv4 openssl asio libcurl)
SRCS = Code/QueryThread.cpp Code/CardData.cpp Code/CardDatabase.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: cstest

cstest: $(OBJS) Code/CSTest.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

csweb: $(OBJS) Code/CSWeb.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

csdb: $(OBJS) Code/CSDB.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f Code/*.o cstest csweb

# Terrible include hack to make sure that if the headers change, the objects are rebuilt.
$(OBJS): $(wildcard Code/*.h)
