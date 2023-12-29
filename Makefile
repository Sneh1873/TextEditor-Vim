CXX = g++
CXXFLAGS = -std=c++14 #-Wall -Wextra -pedantic -g
SRCS = ECTextViewImp.cpp ECEditorTest.cpp ECConcreteObserver.cpp ECMVC.cpp ECCommand.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = edit

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	rm -f $@
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
