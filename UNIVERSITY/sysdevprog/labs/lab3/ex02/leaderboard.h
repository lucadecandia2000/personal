#ifndef LEADERBOARD
#define LEADERBOARD

#include <string>
#include <set>

struct Player {
    std::string name ;
    int score ;

    Player(const std::string& n, int s);

    bool operator<(const Player&other) const ;

};
class Leaderboard {
    public : 

    void addPlayer(const std::string& name , int score) ; 
    void removePlayer(const std::string& name) ;   // here the passing of the name like this (const reference) is because the name it
    void updateScore(const std::string& name, int newS); // is not supposed to be modified (even uninentionally) by the function
    void printTopPlayers(int n ) ; 

    private :
    std::set<Player> players ;
};



#endif