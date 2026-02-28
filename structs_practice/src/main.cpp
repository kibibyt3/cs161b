/******************************************************************************
# Author:      Liv Callister
# Assignment:  Discussion 6
# Date:        28 February 2026
# Description:	This program takes song metadata from the user and prints it.
# Input:       char title[], char artist[], unsigned int (times played)
# Output:      Song song
# Sources:     en.cppreference.org, Discussion 6 Specifications
#*****************************************************************************/
#include <iostream>
#include <string>

using namespace std;

struct Song {
   char title[50];
   char artist[50];
   unsigned int plays;
};

void build_song(Song &song);
void print_song(Song const &song);

// Name:    int main()
// Desc:    This function controls the high-level control flow of the program.
// Input:   char title[], char artist[], unsigned int (times played)
// Output:  Song song
// Return:  0 on success
int main() {
   Song song;

   cout << "Welcome to Liv's Song Builder!" << endl;
   build_song(song);

   cout << "Your song info:" << endl;
   print_song(song);

   return 0;
}

// Name:    build_song(Song &song)
// Desc:    This function populates the members of a Song.
// Input:   Song &song
// Output:  Prompts for the title, artist, and plays of the Song.
// Return:  None
void build_song(Song &song) {
   long int plays = 0;

   cout << "Please enter the title of your song:" << endl << ">> ";
   cin.getline(song.title, 50);
   cout << "Please enter the artist's name:" << endl << ">> ";
   cin.getline(song.artist, 50);
   
   cout << "Please enter the number of times this song has been played: ";
   cout << endl << ">> ";
   cin >> plays;
   while (plays < 0) {
      cout << "Plays must be a positive number! Try again." << endl;
      cout << "Please enter the number of times this song has been played: ";
      cout << endl << ">> ";
      cin >> plays;
   }
   song.plays = static_cast<unsigned int>(plays);
}

// Name:    print_song(Song const &song)
// Desc:    This function prints the members of a Song.
// Input:   Song const &song
// Output:  Song const &song
// Return:  None
void print_song(Song const &song) {
   cout << "Title: " << song.title << endl;
   cout << "Artist: " << song.artist << endl;
   cout << "Times played: " << song.plays << endl;
}
