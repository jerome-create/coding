CXXFLAGS = -std=c++17 -Wall -Wextra -O2
OBJS = main.o jeu.o mobile.o tools.o message.o

projet: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) projet
