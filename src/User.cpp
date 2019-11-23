#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
using namespace std;

//---------------------Class User----------------------
//Constructor
User::User(const std::string& name) : name(name) {   std::vector<Watchable*> history;  }
//Destructor
User::~User() {
    for(int i=0; i<history.size(); i++)
        delete history[i];
    history.clear();
}
//Copy Constructor
User::User(const User &other, std::string& NewName): name(NewName), history(other.history) {  /// $$$  NewName suppose to be valid
///copy of vector by default
}
//Move Constructor
///default
//Copy Assignment
User& User::operator=(const User &other){    /// NOT NEEDED - the name of the assignment stay the same
    history=other.history;
    //name does not change!!!
    return *this;
}
//Move Assignment
///default
//getName
std::string User::getName() const{    return name;   }
//getHistory
std::vector<Watchable*> User::get_history() const {     return history;    }
//getHistory_i
Watchable* User::get_history_i(int i) const {
    if(i>=0 & i<this->get_history().size())
        return history[i];
    std::cout << "error: no such index exists.";
}

//---------------------Class LengthRecUser----------------------
//getRecommendation
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {}      /// <<<<<<<<<<< build it

//---------------------Class RerunRecUser----------------------
//getRecommendation
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {}       /// <<<<<<<<<<< build it

//---------------------Class GenreRecUser----------------------
//getRecommendation
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {}       /// <<<<<<<<<<< build it