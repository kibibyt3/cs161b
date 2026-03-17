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
   TILE_WISP,
   TILE_DEFAULT
} Tile;

// Outcomes from user input
typedef enum {
   OUTCOME_EMPTY,
   OUTCOME_IMMOVABLE,
   OUTCOME_VICTORY,
   OUTCOME_DEATH,
   OUTCOME_DEFAULT,
} ActionOutcome;

typedef enum {
   DIRECTION_NORTH,
   DIRECTION_SOUTH,
   DIRECTION_EAST,
   DIRECTION_WEST,
} Direction;

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

typedef Tile Map[MAP_HEIGHT][MAP_WIDTH];

typedef struct {
   unsigned short x;
   unsigned short y;
} Coords;

void map_populate(Map map);
void map_print(Map map);
void map_tile_swap(Map map, Coords from, Coords to);
char tile_to_char(Tile tile);
ActionOutcome handle_user_input(Map map, InputCommand input);
Tile move_unique_tile(Map map, Tile tile, Direction direction);
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
   } while (
         input != INPUT_EXIT
         && outcome != OUTCOME_VICTORY
         && outcome != OUTCOME_DEATH
         );

   // Clean up ncurses.
   endwin();

   std::cout << "\n\n\nAll done, " << player_name << "!" << std::endl << std::endl;
   std::cout << "S T A T S" << std::endl;
   std::cout << "---------" << std::endl;
   if (outcome == OUTCOME_VICTORY) {
      std::cout << "You won!" << std::endl;
   }
   else if (outcome == OUTCOME_DEATH) {
      std::cout << "You were killed by a will-o'-the-wisp..." << std::endl;
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

   unsigned short y = 0;
   unsigned short x = 0;

   unsigned short player_start_x = 0;
   unsigned short player_start_y = 0;
   unsigned short treasure_start_x = 0;
   unsigned short treasure_start_y = 0;
   unsigned short wisp_start_x = 0;
   unsigned short wisp_start_y = 0;

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

      wisp_start_x = (std::rand() % (MAP_WIDTH - 2)) + 1;
      // Put wisp in the middle third of the map.
      wisp_start_y = 
         (std::rand() % (MAP_HEIGHT / 3) + (MAP_HEIGHT / 3));
   } while (
         // Player/treasure/wisp can't be in the same spot!
         // This is a failsafe for small maps.
         (player_start_x == treasure_start_x
            && player_start_y == treasure_start_y)
         || (treasure_start_x == wisp_start_x
            && treasure_start_y == wisp_start_y)
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
         if (x == player_start_x && y == player_start_y) {
            map[y][x] = TILE_PLAYER;
         }
         else if (x == treasure_start_x && y == treasure_start_y) {
            map[y][x] = TILE_TREASURE;
         }
         else if (x == wisp_start_x && y == wisp_start_y) {
            map[y][x] = TILE_WISP;
         }
         // The exterior of the map must be bordered by walls.
         else if (x == 0 || y == 0 
               || x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1) {
            map[y][x] = TILE_WALL;
         }
         else if (wall_row) {
            // Ensure our wall has a gap, but this can also happen randomly.
            if (x == wall_gap || std::rand() % 6 == 0) {
               map[y][x] = TILE_FLOOR;
            }
            else {
               map[y][x] = TILE_WALL;
            }
         }
         else {
            map[y][x] = TILE_FLOOR;
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
   unsigned short x = 0;
   unsigned short y = 0;

   for (y = 0; y < MAP_HEIGHT; y++) {
      for (x = 0; x < MAP_WIDTH; x++) {
         if (mvaddch(y, x, tile_to_char(map[y][x])) == ERR) {
            abort();
         }
      }
   }
}

// Name:    map_tile_swap(Map map, Coords from, Coords to)
// Desc:    Exchange the places of two tiles.
// Input:   Map map, Coords (first tile's coords),
//          Coords (second tile's coords)
// Output:  None
// Return:  None
void map_tile_swap(Map map, Coords from, Coords to) {
   Tile temp_tile = TILE_DEFAULT;

   // Movement is implemented as displacement.
   temp_tile = map[to.y][to.x];
   map[to.y][to.x] = map[from.y][from.x];
   map[from.y][from.x] = temp_tile;
}

// Name:    map_tile_coords(Map map, Tile tile)
// Desc:    Find coords of the final instance on a map of a given tile.
// Input:   Map map, Tile (tile type to match)
// Output:  None
// Return:  Coords (tile coords), or (0, 0) if no match is found
Coords map_tile_coords(Map map, Tile tile) {
   Coords tile_coords = { 0, 0 };
   unsigned short x = 0;
   unsigned short y = 0;
   
   for (y = 0; y < MAP_HEIGHT; y++) {
      for (x = 0; x < MAP_WIDTH; x++) {
         if (map[y][x] == tile) {
            tile_coords.x = x;
            tile_coords.y = y;
         }
      }
   }

   return tile_coords;
}

// Name:    handle_user_input(Map map, InputCommand input)
// Desc:    Handler for user inputs.
// Input:   Map map, InputCommand input
// Output:  None
// Return:  ActionOutcome outcome; this function will abort when given 
//          INPUT_DEFAULT.
ActionOutcome handle_user_input(Map map, InputCommand input) {
   Direction player_target_direction = DIRECTION_NORTH;
   Direction wisp_target_direction = DIRECTION_NORTH;
   Tile player_target_tile = TILE_DEFAULT;
   Tile wisp_target_tile = TILE_DEFAULT;
   bool move = false;

   ActionOutcome outcome = OUTCOME_DEFAULT;
   
   switch (input) {
      case INPUT_MOVE_NORTH:
         move = true;
         player_target_direction = DIRECTION_NORTH;
         break;
      case INPUT_MOVE_SOUTH:
         move = true;
         player_target_direction = DIRECTION_SOUTH;
         break;
      case INPUT_MOVE_EAST:
         move = true;
         player_target_direction = DIRECTION_EAST;
         break;
      case INPUT_MOVE_WEST:
         move = true;
         player_target_direction = DIRECTION_WEST;
         break;
      case INPUT_EXIT:
         break; // This is handled in the main loop.
      case INPUT_DEFAULT:
         abort();
         break;
   }
   if (move) {
      wisp_target_direction = static_cast<Direction>(std::rand() % 4);
      wisp_target_tile = move_unique_tile(
            map,
            TILE_WISP,
            wisp_target_direction);
      player_target_tile = move_unique_tile(
            map,
            TILE_PLAYER,
            player_target_direction);
      if (player_target_tile == TILE_WISP || wisp_target_tile == TILE_PLAYER) {
         outcome = OUTCOME_DEATH;
      }
      else if (player_target_tile == TILE_WALL) {
         outcome = OUTCOME_IMMOVABLE;
      }
      else if (player_target_tile == TILE_TREASURE) {
         outcome = OUTCOME_VICTORY;
      }
      else {
         outcome = OUTCOME_EMPTY;
      }
   } else {
      outcome = OUTCOME_EMPTY;
   }

   return outcome;
}

// Name:    Tile move_unique_tile(Map map, Tile tile, Direction direction)
// Desc:    This function moves a unique tile on the map, if possible.
// Input:   Map map, Tile (unique tile), Direction direction
// Output:  None
// Return:  Tile at targeted space
Tile move_unique_tile(Map map, Tile tile, Direction direction) {
   Coords tile_coords;
   Coords target_coords;
   Tile target_tile = TILE_DEFAULT;

   tile_coords = map_tile_coords(map, tile);
   target_coords = tile_coords;
   switch (direction) {
      case DIRECTION_NORTH:
         target_coords.y -= 1;
         break;
      case DIRECTION_SOUTH:
         target_coords.y += 1;
         break;
      case DIRECTION_EAST:
         target_coords.x += 1;
         break;
      case DIRECTION_WEST:
         target_coords.x -= 1;
         break;
   }

   target_tile = map[target_coords.y][target_coords.x];
   if (target_tile == TILE_FLOOR) {
      map_tile_swap(map, tile_coords, target_coords);
   }

   return target_tile;
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
      case TILE_WISP:
         ch = '*';
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
   Map map;
   unsigned short y = 0;
   unsigned short x = 0;

   map_populate(map);
   for (y = 0; y < MAP_HEIGHT; y++) {
      for (x = 0; x < MAP_WIDTH; x++) {
         assert(
               map[y][x] == TILE_FLOOR
               || map[y][x] == TILE_TREASURE
               || map[y][x] == TILE_WALL
               || map[y][x] == TILE_PLAYER
         );
      }
   }
}
