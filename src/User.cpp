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
User::User(const std::string& name) : name(name), avg(0) {       std::vector<Watchable*> history;       }
//Destructor
User::~User() {}
//Copy Constructor
User::User(const User &other): name(name), history(other.history) {  /// $$$  NewName suppose to be valid
///copy of vector by default
}
//Move Constructor
User::User(User &&other) {}
//Copy Assignment
User &User::operator=(const User &other) { return *this; }
//Move Assignment
User &User::operator=(User &&other) { return *this; }
//User Clone
User *User::clone() {
    return nullptr;
}


//getName
std::string User::getName() const{                                 return name;   }
//getHistory
std::vector<Watchable*> User::get_history() const {                return history;    }
//getHistory_i
Watchable* User::get_history_i(int i) const {
    if(i>=0 & i<this->get_history().size())
        return this->history[i];
    std::cout << "Error - no such index exists.";
    return nullptr;
}
//setName
void User::setName(std::string &new_name) {                        this->name=new_name;   }
//create Watched vector
void User::CreateWatched(Session &sess) {
    this->watched = std::vector<bool>(sess.getContent().size(), false);                 //initiate bool aray
}
//getWatched
std::vector<bool> User::getWatched (){                             return this->watched;  }
//setWatched
void User::setWatched_i (int i){                                   this->watched[i]=true; }
//setAvg
void User::setAvg(int length_val) {
    avg = ( avg+ length_val) / this->get_history().size();
}
//getAvg
double User::getAvg() const {                                      return this->avg;       }
//getGenres
std::vector<std::pair <std::string,int> > User::getGenres() {      return this->Genres;     }
//addGenre
void User::addGenre(Watchable *currWatch) {
    for(int i=0; i<currWatch->getTags().size(); i++){
        bool found=false;
        for(int k=0; k<this->getGenres().size() & !found; k++){
            if(  currWatch->getTags()[i].compare(this->getGenres()[k].first) ==0 ) {
                this->getGenres()[k].second++;
                if(mstWatchedGenre->second < this->getGenres()[k].second |  // now this is bigger
                   (mstWatchedGenre->second==this->getGenres()[k].second & (mstWatchedGenre->first).compare(this->getGenres()[k].first)>0))
                                                                            // or the same size but lexicographSHTUT
                    mstWatchedGenre=&this->getGenres()[k];
                found = true;
            }
        if(!found) {                   // no kind of this tag yet...
            std::pair<std::string, int> newPair(currWatch->getTags()[i], 0);
            this->getGenres().push_back(newPair);
            if(mstWatchedGenre==nullptr |                                   // no genre chosen yet
                        (mstWatchedGenre->second==newPair.second & (mstWatchedGenre->first).compare(newPair.first)>0))
                                                                            // or the same size but lexicographSHTUT
                mstWatchedGenre=&newPair;
            }
        }
    }
}
//getMstWatchedGenre
std::string User::getMstWatchedGenre () const{                            return this->mstWatchedGenre->first;  }


//---------------------Class LengthRecUser----------------------
//Constructor - this is the default anyway
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {}
//Copy Constructor - this is the default anyway
LengthRecommenderUser::LengthRecommenderUser(const User &other) : User(other) {}
//getRecommendation
Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    // those 2 will be operate manuely at Watchable.cpp->getNextWatchable
    this->setWatched_i( this->get_history_i(  this->get_history().size()-1  )->getId() -1  );  //marked as watched
    this->setAvg(this->get_history_i( this->get_history().size()-1 )->getLength());     //computed as avg

    int bst_dif = 2147483647;                                                            // max val of integer
    Watchable* best_opt = nullptr;
    for(int i=0; i<s.getContent().size(); i++){
        if( !this->getWatched()[i]) {                                                    // content not been watched yet
            if (abs(s.getContent()[i]->getLength() - this->getAvg()) < (bst_dif)) {
                bst_dif = abs(s.getContent()[i]->getLength() - this->getAvg());
                best_opt = s.getContent()[i];
            }
        }
    }
    return (best_opt);
}
//LengthRecommenderUser Clone
User *LengthRecommenderUser::clone() {
    return (new LengthRecommenderUser(*this));
}

//---------------------Class RerunRecUser----------------------
//Constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name), rerun_next_index(0) {}
//CopyConstructor
RerunRecommenderUser::RerunRecommenderUser(const User &other) : User(other) {}
//getRecommendation
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    // ******* rerun_next_index will always < history.size()
    rerun_next_index++;
    return (s.getActiveUser()->get_history_i(  this->rerun_next_index-1  ));
}
// !!!  if seen no episodes - always will recommend the last watchable

//RerunRecommenderUser Clone
User *RerunRecommenderUser::clone() {
    return (new RerunRecommenderUser(*this));
}


//---------------------Class GenreRecUser----------------------
//Constructor
GenreRecommenderUser::GenreRecommenderUser( const std::string &name):User(name) {}
//Copy Constructor
GenreRecommenderUser::GenreRecommenderUser(const User &other): User(other) {}
//getRecommendation
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    // those 2 will be operate manuely at Watchable.cpp->getNextWatchable
    this->setWatched_i( this->get_history_i(  this->get_history().size()-1  )->getId() -1  );  //marked as watched
    this->addGenre(this->get_history_i(  this->get_history().size()-1));                //counted tags

    Watchable* best_opt = nullptr;
    for(int i=0; i<s.getContent().size(); i++){                         //checks all content
        if( !this->getWatched()[i] ) {                                  // content not been watched yet
            for(int k=0; k<s.getContent()[i]->getTags().size(); k++)    //check all tags of 1 watchable
                if(s.getContent()[i]->getTags()[k].compare(this->getMstWatchedGenre())==0)
                    return s.getContent()[i];
        }
    }
    return (best_opt);
}
//GenreRecommenderUser Clone
User *GenreRecommenderUser::clone() {
    return (new GenreRecommenderUser(*this));
}


