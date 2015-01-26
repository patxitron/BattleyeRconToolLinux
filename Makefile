CXXFLAGS = -O2 -std=c++11 -Wall -pedantic -g -fmessage-length=0

OBJS = a3s.o main.o

LIBS =

TARGET = rconTool

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
