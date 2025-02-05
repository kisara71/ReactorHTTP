CXX = g++

CXXFLAGS = -g -Wall

SRCS = main.cpp Server/Server.cpp Logger/Logger.cpp Config/Config.cpp
ASSEMBLY = $(SRCS:.cpp=.s)
OBJS = $(ASSEMBLY:.s=.o)

TARGET = server

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.s: %.cpp
	$(CXX) -S $< $(CXXFLAGS)

%.o: %.s
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(ASSEMBLY) $(TARGET)

.PHONY: clean
