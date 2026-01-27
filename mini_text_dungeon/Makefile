CXX = g++
CXXFLAGS = -g -Wall -Wextra -Wpedantic -std=c++17
LDLIBS = -lncurses

mini_text_dungeon: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o mini_text_dungeon $(LDLIBS)

clean:
	rm -f mini_text_dungeon
