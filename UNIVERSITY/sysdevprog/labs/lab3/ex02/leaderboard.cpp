/*Exercise 2
Leaderboard using standard sets
-------------------------------

A "leaderboard" displays the names and current scores of the leading competitors
in a tournament.
Implement a leaderboard using standard sets.



A name identifies each player and has a score associated with them.
It should be possible to add or remove players from the leaderboard and update
existing players' scores.
It should also be possible to display the top n players on the leaderboard, where
n is a positive integer value.

More specifically, the program should:

* Define a structure player (named Player) to hold a player's name and score.

* Define a custom comparison function or operator to order the player objects in
  the standard set based on their score.

* Implement a class to manage the leaderboard (Leaderboard).
  The class should have the following public methods:

  - void addPlayer(const std::string& name, int score)
    which adds a new player with the given name and score to the leaderboard.
    Suppose that names are unique.

  - void removePlayer(const std::string& name)
    which removes the player with the given name from the leaderboard.

  - void updateScore(const std::string& name, int newScore)
    which updates the player's score with the given name to the new score

  - void printTopPlayers(int n)
    which displays the names and scores of the top n players on the leaderboard (e.g,
    the standard output or a file).
    The players must be ordered by the score in descending order.
    In case ex aequo among players, define a possible order among them (e.g., based on
    the name or the insertion order in the laderboard).
    Display all players if there are fewer than n players on the leaderboard.

Suggestion
----------
Use a standard set to store the Player objects and keep them ordered by score.
When you define the custom comparison function or operator, consider
how ties in scores are solved.
For example, players with the same score can be ordered by the lexicographical
order of their names.

*/

#include "leaderboard.h"
#include <set>       // for std::set
#include <string>    // for std::string
#include <iostream>  // for std::cout and std::endl

using namespace std;

Player::Player(const std::string& n, int s): name(n), score(s) {} 

bool Player::operator<(const Player& other) const {
    if (score != other.score) {
        return score > other.score; // if score not the same order by score 
    }
    return name < other.name; // if the same order by name 
}


void Leaderboard::addPlayer(const std::string& name, int score) {
  players.insert({name,score}) ; 
}

void Leaderboard::removePlayer(const std::string& name) {
    set<Player>::iterator it; 
    for (it = players.begin(); it != players.end(); ++it) {   
        if (it->name == name) {
            break;
        }
    }
    if (it == players.end())
        cout << name << " not found" << endl;
    else
        players.erase(it); // using this with only the name does not work, it appears an it is needed 
}

void Leaderboard::updateScore(const std::string& name, int score) {
   set<Player>::iterator it ;

   for (it = players.begin(); it!= players.end(); ++it) {
    if(it->name == name) {
        break ; 
    }
   }
   if(it == players.end())
    cout << "not found" << endl ; 
   else 
    players.erase(it) ; 
    players.insert({name,score}) ; 
}

void Leaderboard::printTopPlayers( int n ) {
    cout << "Leaderboard:" << endl;
    int count = 0;
    for (const auto& player : players) {
        cout << player.name << " - " << player.score << endl;
        count++;
        if (count >= n) {
            break;
        }
    }
}

int main() {
    Leaderboard leaderboard;

    // Add some players with scores
    leaderboard.addPlayer("Luca", 150);
    leaderboard.addPlayer("Mario", 40);
    leaderboard.addPlayer("Bho", 105);


    // Print leaderboard
    leaderboard.printTopPlayers(2);

    // Update a player's score
    leaderboard.updateScore("Mario", 60);

    // Remove a player from the leaderboard
    leaderboard.removePlayer("Mario");

    // Print updated leaderboard
    leaderboard.printTopPlayers(1);

    return 0;
}


