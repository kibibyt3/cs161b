/******************************************************************************
# Author:
# Assignment:
# Date:
# Description:
# Input:
# Output:
# Sources:     https://tldp.org/HOWTO/NCURSES-Programming-HOWTO,
               en.cppreference.org,
               https://www.gnu.org/software/make/manual/make.html
#*****************************************************************************/
#include <cstdlib>
#include <ncurses.h>
#include <time.h>

typedef enum {
   TILE_FLOOR,
   TILE_WALL,
   TILE_PLAYER,
   TILE_TREASURE,
   TILE_DEFAULT
} Tile;

typedef enum {
   INPUT_MOVE_NORTH = 'w',
   INPUT_MOVE_SOUTH = 's',
   INPUT_MOVE_EAST = 'd',
   INPUT_MOVE_WEST = 'a',
   INPUT_EXIT = 'q',
   INPUT_DEFAULT = '\0'
} InputCommand;

const unsigned short MAP_HEIGHT = 20;
const unsigned short MAP_WIDTH = 10;

typedef Tile Map[MAP_HEIGHT * MAP_WIDTH];

typedef struct {
   unsigned short x;
   unsigned short y;
} Coords;

void map_populate(Map map);
void map_print(Map map);
void map_switch(Map map, size_t from, size_t to);
char tile_to_char(Tile tile);
void handle_user_input(Map map, InputCommand input);
size_t coords_to_index(Coords coords);
Coords index_to_coords(size_t index);

// Name:
// Desc:
// Input:
// Output:
// Return:
int main() {

   Map map;
   InputCommand input = INPUT_DEFAULT;

   // Seed std::rand();
   std::srand(time(0));

   // Initialize ncurses.
   initscr();
   noecho();
   raw();

   // Initialize map.
   map_populate(map);
   
   do {
      map_print(map);
      refresh();
      // Get user input.
      input = static_cast<InputCommand>(getch());
      // Operate on user input.
      handle_user_input(map, input);
   } while (input != INPUT_EXIT);

   // Clean up ncurses.
   endwin();

   return EXIT_SUCCESS;
}

void map_populate(Map map) {
   bool wall_row = false;
   unsigned short wall_gap = 0;

   size_t index = 0;
   unsigned short y = 0;
   unsigned short x = 0;

   unsigned short player_start_x = 0;
   unsigned short player_start_y = 0;
   unsigned short treasure_start_x = 0;
   unsigned short treasure_start_y = 0;

   do {
      // Set up player and treasure coordinates.
      player_start_x = std::rand() % MAP_WIDTH;
      // Put player in the first fourth of the map.
      player_start_y = std::rand() % (MAP_WIDTH / 4);
      
      treasure_start_x = std::rand() % MAP_WIDTH;
      // Put treasure in the final fourth of the map.
      treasure_start_y = std::rand() % MAP_WIDTH + ((MAP_WIDTH / 4) * 3);
   } while (
         // Player and treasure can't be in the same spot!
         // This is a failsafe for maps with height < 4.
         player_start_x == treasure_start_x
         && player_start_y == treasure_start_y
   );
   
   for (y = 0; y < MAP_HEIGHT; y++) {
      // Rows can occasionally be filled with a wall containing a gap.
      if ((wall_row = std::rand() % 6 == 0)) {
         wall_gap = std::rand() % MAP_WIDTH;
      }
      for (x = 0; x < MAP_WIDTH; x++) {
         index = coords_to_index(Coords { x = x, y = y });
         if (x == player_start_x && y == player_start_y) {
            map[index] = TILE_PLAYER;
         }
         else if (x == treasure_start_x && y == treasure_start_y) {
            map[index] = TILE_TREASURE;
         }
         else if (wall_row) {
            // Ensure our wall has a gap, but this can also happen randomly.
            if (x == wall_gap || std::rand() % 6 == 0) {
               map[index] = TILE_FLOOR;
            }
            else {
               map[index] = TILE_WALL;
            }
         }
         else {
            map[index] = TILE_FLOOR;
         }
      }
   }
}

void map_print(Map map) {
   size_t i = 0;
   Coords coords;
   coords.x = 0;
   coords.y = 0;
   for (i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
      coords = index_to_coords(i);
      if (mvaddch(coords.y, coords.x, tile_to_char(map[i])) == ERR) {
         abort();
      }
   }
}

void map_switch(Map map, size_t from, size_t to) {
   Tile temp_tile = map[to];
   map[to] = map[from];
   map[from] = temp_tile;
}

size_t map_tile_index(Map map, Tile tile) {
   size_t tile_index = 0;
   size_t i = 0;
   size_t max_index = 0;
   Coords max_coords;
   
   max_coords.x = MAP_WIDTH;
   max_coords.y = MAP_HEIGHT;
   max_index = coords_to_index(max_coords);
   for (i = 0; i < max_index; i++) {
      if (map[i] == tile) {
         tile_index = i;
      }
   }

   return tile_index;
}

void handle_user_input(Map map, InputCommand input) {
   size_t player_index = 0;
   player_index = map_tile_index(map, TILE_PLAYER);
   switch (input) {
      case INPUT_MOVE_NORTH:
         map_switch(map, player_index, player_index - MAP_WIDTH);
         break;
      case INPUT_MOVE_SOUTH:
         map_switch(map, player_index, player_index + MAP_WIDTH);
         break;
      case INPUT_MOVE_EAST:
         map_switch(map, player_index, player_index + 1);
         break;
      case INPUT_MOVE_WEST:
         map_switch(map, player_index, player_index - 1);
         break;
      case INPUT_EXIT:
         break; // This is handled in the main loop.
      case INPUT_DEFAULT:
         abort();
         break;
   }
}

size_t coords_to_index(Coords coords) {
   return (coords.y * MAP_HEIGHT) + coords.x;
}

Coords index_to_coords(size_t index) {
   Coords coords;
   coords.x = index % MAP_WIDTH;
   coords.y = index / MAP_HEIGHT;
   return coords;
}


char tile_to_char(Tile tile) {
   char ch = '\0';
   switch (tile) {
      case TILE_FLOOR:
         ch = '.';
         break;
      case TILE_WALL:
         ch = '#';
         break;
      case TILE_PLAYER:
         ch = 'S';
         break;
      case TILE_TREASURE:
         ch = 'T';
         break;
      case TILE_DEFAULT:
         abort(); // TILE_DEFAULT is an initialization value.
         break;
   }
   return ch;
}
