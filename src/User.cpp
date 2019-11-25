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
User::User(const User &other): name(name), history(other.history) {  /// $$$  NewName suppose to be valid
///copy of vector by default
}
//Move Constructor
///default
//Copy Assignment
///default - for action duplicate user
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
    std::cout << "Error - no such index exists.";
}
//setName
void User::setName(std::string &new_name) {             this->name=new_name; }

//---------------------Class LengthRecUser----------------------
//Constructor - this is the default anyway
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {}
//Copy Constructor - this is the default anyway
LengthRecommenderUser::LengthRecommenderUser(const User &other) : User(other) {}
//getRecommendation
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {}      /// <<<<<<<<<<< build it

//---------------------Class RerunRecUser----------------------
//Constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) {}
//getRecommendation
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {}
/// <<<<<<<<<<< build it

//---------------------Class GenreRecUser----------------------
//Constructor
GenreRecommenderUser::GenreRecommenderUser( const std::string &name):User(name) {}
//getRecommendation
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {}       /// <<<<<<<<<<< build it
/// <<<<<<<<<<< build it
