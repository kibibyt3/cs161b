/******************************************************************************
# Author:      Liv Callister
# Assignment:  Assignment 2
# Date:        29 January 2026
# Description: A miniature text dungeon.
# Input:       Player controls
# Output:      Game UI and prompts
# Sources:     https://tldp.org/HOWTO/NCURSES-Programming-HOWTO,
               en.cppreference.org,
               https://www.gnu.org/software/make/manual/make.html,
               Assignment 2 Specifications
#*****************************************************************************/
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <ncurses.h>
#include <time.h>

typedef enum {
   TILE_FLOOR,
   TILE_WALL,
   TILE_PLAYER,
   TILE_TREASURE,
   TILE_DEFAULT
} Tile;

// Outcomes from user input
typedef enum {
   OUTCOME_EMPTY,
   OUTCOME_IMMOVABLE,
   OUTCOME_VICTORY,
   OUTCOME_DEFAULT,
} ActionOutcome;

// User controls
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
void map_tile_swap(Map map, size_t from, size_t to);
char tile_to_char(Tile tile);
ActionOutcome handle_user_input(Map map, InputCommand input);
size_t coords_to_index(Coords coords);
Coords index_to_coords(size_t index);
void tests();

// Name:    main()
// Desc:    Run the main input-output loop of the program.
// Input:   None
// Output:  Name prompt, game proper, and stats page
// Return:  EXIT_SUCCESS on success
int main() {
   ActionOutcome outcome = OUTCOME_DEFAULT;
   char player_name[79];
   int steps_taken = 0;

   Map map;
   InputCommand input = INPUT_DEFAULT;
   
   // tests();

   // Seed std::rand();
   std::srand(time(0));

   // Get player name.
   std::cout << "Welcome to mini_text_dungeon, by liv." << std::endl;
   std::cout << "Before we begin, please enter your name: " << std::endl;
   std::cin >> player_name;

   // Initialize ncurses.
   initscr();
   noecho();
   raw();
   curs_set(0);

   // Initialize map.
   map_populate(map);
   
   do {
      map_print(map);
      refresh();
      // Get user input.
      input = static_cast<InputCommand>(getch());
      // Operate on user input.
      outcome = handle_user_input(map, input);
      // Right now, only these two outcomes correspond to a successful step.
      if (outcome == OUTCOME_EMPTY || outcome == OUTCOME_VICTORY) {
         steps_taken++;
      }
   } while (input != INPUT_EXIT && outcome != OUTCOME_VICTORY);

   // Clean up ncurses.
   endwin();

   std::cout << "\n\n\nAll done, " << player_name << "!" << std::endl << std::endl;
   std::cout << "S T A T S" << std::endl;
   std::cout << "---------" << std::endl;
   if (outcome == OUTCOME_VICTORY) {
      std::cout << "You won!" << std::endl;
   }
   else {
      std::cout << "You didn't win..." << std::endl;
   }
   std::cout << "STEPS TAKEN: " << steps_taken << std::endl;

   return EXIT_SUCCESS;
}

// Name:    map_populate(Map map)
// Desc:    Procedurally generates a map.
// Input:   Map (map to populate)
// Output:  None
// Return:  None
void map_populate(Map map) {
   bool wall_row = false;
   unsigned short wall_gap = 0;
   Coords coords;

   size_t index = 0;
   unsigned short y = 0;
   unsigned short x = 0;

   unsigned short player_start_x = 0;
   unsigned short player_start_y = 0;
   unsigned short treasure_start_x = 0;
   unsigned short treasure_start_y = 0;

   do {
      // Set up player and treasure coordinates, ensuring they don't begin
      // at the edge of the map.
      player_start_x = (std::rand() % (MAP_WIDTH - 2)) + 1;
      // Put player roughly in the first fourth of the map.
      player_start_y = (std::rand() % (MAP_HEIGHT / 4)) + 1;
      
      treasure_start_x = (std::rand() % (MAP_WIDTH - 2)) + 1;
      // Put treasure in the final fourth of the map.
      treasure_start_y = 
         (std::rand() % (MAP_HEIGHT / 4) + ((MAP_HEIGHT / 4) * 3)) - 1;
   } while (
         // Player and treasure can't be in the same spot!
         // This is a failsafe for maps with height < 4.
         player_start_x == treasure_start_x
         && player_start_y == treasure_start_y
   );
   
   for (y = 0; y < MAP_HEIGHT; y++) {
      // Rows can occasionally be filled with a wall containing a gap,
      // but never adjacent to another such row.
      if (wall_row) {
         wall_row = false;
      }
      else {
         if ((wall_row = std::rand() % 6 == 0)) {
            wall_gap = (std::rand() % (MAP_WIDTH - 2)) + 1;
         }
      }
      for (x = 0; x < MAP_WIDTH; x++) {
         coords.x = x;
         coords.y = y;
         index = coords_to_index(coords);
         if (x == player_start_x && y == player_start_y) {
            map[index] = TILE_PLAYER;
         }
         else if (x == treasure_start_x && y == treasure_start_y) {
            map[index] = TILE_TREASURE;
         }
         // The exterior of the map must be bordered by walls.
         else if (x == 0 || y == 0 
               || x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1) {
            map[index] = TILE_WALL;
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

// Name:    map_print(Map map)
// Desc:    Displays a map to the screen.
// Input:   Map (map to print)
// Output:  None
// Return:  None; this function will abort if the screen is too small.
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

// Name:    map_tile_swap(Map map, size_t from, size_t to)
// Desc:    Exchange the places of two tiles.
// Input:   Map map, size_t (first tile's index), size_t (second tile's index)
// Output:  None
// Return:  None
void map_tile_swap(Map map, size_t from, size_t to) {
   Tile temp_tile = TILE_DEFAULT;

   // Movement is implemented as displacement.
   temp_tile = map[to];
   map[to] = map[from];
   map[from] = temp_tile;
}

// Name:    map_tile_index(Map map, Tile tile)
// Desc:    Find the final instance on a map of a given tile.
// Input:   Map map, Tile (tile type to match)
// Output:  None
// Return:  size_t (tile index), or 0 if no match is found
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

// Name:    handle_user_input(Map map, InputCommand input)
// Desc:    Handler for user inputs.
// Input:   Map map, InputCommand input
// Output:  None
// Return:  ActionOutcome outcome; this function will abort when given 
//          INPUT_DEFAULT.
ActionOutcome handle_user_input(Map map, InputCommand input) {
   size_t player_index = 0;
   player_index = map_tile_index(map, TILE_PLAYER);
   bool move = false;
   size_t target_index = 0;
   ActionOutcome outcome = OUTCOME_DEFAULT;
   switch (input) {
      case INPUT_MOVE_NORTH:
         move = true;
         target_index = player_index - MAP_WIDTH;
         break;
      case INPUT_MOVE_SOUTH:
         move = true;
         target_index = player_index + MAP_WIDTH;
         break;
      case INPUT_MOVE_EAST:
         move = true;
         target_index = player_index + 1;
         break;
      case INPUT_MOVE_WEST:
         move = true;
         target_index = player_index - 1;
         break;
      case INPUT_EXIT:
         break; // This is handled in the main loop.
      case INPUT_DEFAULT:
         abort();
         break;
   }
   if (move) {
      if (map[target_index] == TILE_WALL) {
         outcome = OUTCOME_IMMOVABLE;
      }
      else if (map[target_index] == TILE_TREASURE) {
         outcome = OUTCOME_VICTORY;
      }
      else {
         map_tile_swap(map, player_index, target_index);
         outcome = OUTCOME_EMPTY;
      }
   } else {
      outcome = OUTCOME_EMPTY;
   }

   return outcome;
}

// Name:    coords_to_index(Coords coords)
// Desc:    Convert coordinates to a map index.
// Input:   Coords coords
// Output:  None
// Return:  size_t index
size_t coords_to_index(Coords coords) {
   return (coords.y * MAP_WIDTH) + coords.x;
}

// Name:    index_to_coords(size_t index)
// Desc:    Convert a map index to coordinates.
// Input:   size_t index
// Output:  None
// Return:  Coords coords
Coords index_to_coords(size_t index) {
   Coords coords;
   coords.x = index % MAP_WIDTH;
   coords.y = index / MAP_WIDTH;
   return coords;
}

// Name:    tile_to_char(Tile tile)
// Desc:    Convert a tile to a character for display purposes.
// Input:   Tile tile
// Output:  None
// Return:  char character; this function will abort given TILE_DEFAULT.
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

// Name:    tests()
// Desc:    Run the unit test suite.
// Input:   None
// Output:  None
// Return:  None; this function will abort if a test fails.
void tests() {
   size_t index = 0;
   Coords coords;
   Map map;
   size_t i = 0;

   coords.x = 0;
   coords.y = 1;
   index = MAP_WIDTH;
   assert(
         index_to_coords(index).x == coords.x
         && index_to_coords(index).y == coords.y
   );
   assert(coords_to_index(coords) == index);

   map_populate(map);
   for (i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
      assert(
            map[i] == TILE_FLOOR
            || map[i] == TILE_TREASURE
            || map[i] == TILE_WALL
            || map[i] == TILE_PLAYER
      );
   }
}
