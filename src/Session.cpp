#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/json.hpp"
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
using namespace std;
using json = nlohmann::json;

//---------------------Class Session----------------------
//Constructor
Session::Session(const std::string &configFilePath): content (0, nullptr), actionsLog(0, nullptr),
    userMap{}, activeUser(nullptr), running(false), line("") {
    std::ifstream i("../config2.json");
    json j;
    i >>j;
    //switch() /// import json parameters for watchables
    for(int k=0; k<(int)j["movies"].size(); k++){
        this->content.push_back(new Movie((int)this->content.size()+1,j["movies"][k]["name"],
                j["movies"][k]["length"],j["movies"][k]["tags"]));  ///WATCH FOR ERRORS WITH USING JSON
    }
    for(int k=0; k<(int)j["tv_series"].size(); k++) {
        for (int s = 1; s <= (int)j["tv_series"][k]["seasons"].size(); s++) {
            for (int e = 1; e <= j["tv_series"][k]["seasons"][s - 1]; e++) {
                Episode* EP = new Episode((int)this->content.size() + 1, j["tv_series"][k]["name"],
                        j["tv_series"][k]["episode_length"], s, e,j["tv_series"][k]["tags"]);
                this->content.push_back(EP);
                if((s==(int)j["tv_series"][k]["seasons"].size()) &  (e==j["tv_series"][k]["seasons"][s - 1]) )
                    EP->setNextEpisodeId(0);                        //LAST EPISODE AT TV SERIES
                else
                    EP->setNextEpisodeId((int)this->content.size()+2);
            }
        }
    }
    /// Add config 1 ?????
    //Create new user default and it's the active user
    std::string cmd;
    cmd = "createuser default len";
    Session::nextCommand(cmd);
    cmd = "changeuser default";
    Session::nextCommand(cmd);
}
//Destructor
Session::~Session() {
    for(int i=0; i<(int)this->getContent().size(); i++){                 //DELETE
        delete this->getContent()[i];
    }
    for(int i=0; i<(int)this->getActionLog().size(); i++){               //DELETE
        delete this->getActionLog()[i];
    }
    this->getUserMap().erase(this->getUserMap().begin(),this->getUserMap().end());  //DELETE
    //delete this->activeUser;
    this->activeUser=nullptr;
}
//Copy Constructor
Session::Session(Session &other): content(other.content), actionsLog(other.actionsLog), userMap(other.userMap),
    activeUser(other.activeUser), running(other.running), line(other.line) {
    for(int i=0; i<(int)other.getContent().size(); i++){                 //Insert
        this->getContent().push_back( other.getContent()[i]->clone() );

       /*Watchable* cpy = Watchable(other.getContent()[i]);
       this->getContent().push_back(other.getContent()[i]); */
    }
    for(int i=0; i<(int)other.getActionLog().size(); i++){                 //Insert
        this->getActionLog().push_back(  other.getActionLog()[i]->clone() );
    }
    /*for ( unsigned i = 0; i < other.getUserMap().bucket_count(); ++i) {
            for ( auto local_it = other.getUserMap().begin(i); local_it!= other.getUserMap().end(i); ++local_it ) {
                User* old_usr = local_it->second;
                std::string old_name = local_it->first;
                std::string class_type = typeid( old_usr ).name(); //get class #name
                if(class_type.at(2) == 'L'){
                    LengthRecommenderUser* new_usr =
                            dynamic_cast<LengthRecommenderUser*>( old_usr); //Duplicate
                    this->getUserMap().insert({old_name, new_usr});        //adding user to userMap
                    if(other.getActiveUser() == old_usr)
                        this->setActiveUser(new_usr);    // Active user
                }
                else if(class_type.at(2)=='R'){
                    RerunRecommenderUser* new_usr =
                            dynamic_cast<RerunRecommenderUser*>( old_usr); //Duplicate
                    this->getUserMap().insert({old_name, new_usr});        //adding user to userMap
                    if(other.getActiveUser() == old_usr)
                        this->setActiveUser(new_usr);   // Active user
                }
                else if(class_type.at(2)=='G') { // can be else - just to clearify
                    GenreRecommenderUser *new_usr =
                            dynamic_cast<GenreRecommenderUser *>( old_usr); //Duplicate
                    this->getUserMap().insert({old_name, new_usr});        //adding user to userMap
                    if(other.getActiveUser() == old_usr)
                        this->setActiveUser(new_usr);    // Active user
                }
            }
    }
     */
    for ( unsigned i = 0; i < other.getUserMap().bucket_count(); ++i) {
        for (auto local_it = other.getUserMap().begin(i); local_it != other.getUserMap().end(i); ++local_it) {
            User *new_usr = local_it->second->clone();
            std::string cpy_name = local_it->first;
            this->getUserMap().insert({cpy_name, new_usr});        //adding user to userMap
            if(other.getActiveUser() == local_it->second)
                this->setActiveUser(new_usr);    // Active user
        }
    }
}
//Move Constructor
Session::Session(Session &&other): content(other.content), actionsLog(other.actionsLog), userMap(other.userMap),
    activeUser(other.activeUser), running(other.running), line(other.line) {
    if(this!=&other) {
        for (int i = 0; i < (int)other.getContent().size(); i++) {                 //Insert
            this->getContent().push_back(other.getContent()[i]);
            other.getContent()[i] = nullptr;  //delete other one
            /*Watchable* cpy = Watchable(other.getContent()[i]);
            this->getContent().push_back(other.getContent()[i]);*/
        }

        //maybe error
        for (int i = 0; i < (int)other.getContent().size(); i++) {  //delete other one
            delete other.getContent()[i];
        }
        //

        for (int i = 0; i < (int)other.getActionLog().size(); i++) {                 //Insert
            this->getActionLog().push_back(other.getActionLog()[i]);
            other.getActionLog()[i] = nullptr;    //delete other one
        }
        for (int i = 0; i < (int)other.getActionLog().size(); i++) {
            delete other.getActionLog()[i];     //delete other one
        }

        for (unsigned i = 0; i < other.getUserMap().bucket_count(); ++i) {
            for (auto local_it = other.getUserMap().begin(i); local_it != other.getUserMap().end(i); ++local_it) {
                User *new_usr = local_it->second;
                std::string cpy_name = local_it->first;
                this->getUserMap().insert({cpy_name, new_usr});        //adding user to userMap
                if (other.getActiveUser() == local_it->second)
                    this->setActiveUser(new_usr);    // Active user
                local_it->second = nullptr;
            }
        }
        other.getUserMap().erase(other.getUserMap().begin(), other.getUserMap().end());  //DELETE
    }
}
//Copy Assignment
Session &Session::operator=(Session &other){
    if(this!=&other) {
        this->running = other.running;
        this->line = other.line;
        for (int i = 0; i < (int)this->getContent().size(); i++) {                 //delete
            delete this->getContent()[i];
        }
        for (int i = 0; i < (int)other.getContent().size(); i++) {                 //Insert
            this->getContent().push_back(other.getContent()[i]->clone());                       //CLONE
        }

        for (int i = 0; i < (int)other.getActionLog().size(); i++) {               //delete
            delete other.getActionLog()[i];
        }
        for (int i = 0; i < (int)other.getActionLog().size(); i++) {                 //Insert
            this->getActionLog().push_back(other.getActionLog()[i]->clone());              //CLONE
        }

        for (unsigned i = 0; i < this->getUserMap().bucket_count(); ++i) { //delete
            for (auto local_it = this->getUserMap().begin(i); local_it != this->getUserMap().end(i); ++local_it) {
                delete local_it->second;
            }
        }
        this->getUserMap().erase(this->getUserMap().begin(), this->getUserMap().end());  //DELETE THIS MAP
        for (unsigned i = 0; i < other.getUserMap().bucket_count(); ++i) {                       // INSERT THIS MAP
            for (auto local_it = other.getUserMap().begin(i); local_it != other.getUserMap().end(i); ++local_it) {
                User *new_usr = local_it->second->clone();                                  //CLONE
                std::string cpy_name = local_it->first;
                this->getUserMap().insert({cpy_name, new_usr});        //adding user to userMap
                if (other.getActiveUser() == local_it->second)
                    this->setActiveUser(new_usr);    // Active user
            }
        }
    }
        return (*this);
}
//Move Assignment
Session &Session::operator=(Session &&other) {
    this->running=other.running;
    this->line=other.line;
    for(int i=0; i<(int)this->getContent().size(); i++){                 //delete
        delete this->getContent()[i];
    }
    for(int i=0; i<(int)other.getContent().size(); i++) {                 //Insert
        this->getContent().push_back(other.getContent()[i]);
        other.getContent()[i]=nullptr;  //delete other one
    }
    for(int i=0; i<(int)other.getContent().size(); i++){  //delete other one
        delete other.getContent()[i];
    }

    for(int i=0; i<(int)this->getActionLog().size(); i++){               //delete
        delete this->getActionLog()[i];
    }
    for(int i=0; i<(int)other.getActionLog().size(); i++){                 //Insert
        this->getActionLog().push_back(  other.getActionLog()[i]);
        other.getActionLog()[i]=nullptr;    //delete other one
    }
    for(int i=0; i<(int)other.getActionLog().size(); i++){
        delete other.getActionLog()[i];     //delete other one
    }

    for ( unsigned i = 0; i < this->getUserMap().bucket_count(); ++i) { //delete THIS
        for (auto local_it = this->getUserMap().begin(i); local_it != this->getUserMap().end(i); ++local_it) {
            delete local_it->second;
        }
    }
    other.getUserMap().erase(other.getUserMap().begin(),other.getUserMap().end());  //DELETE THIS
    for ( unsigned i = 0; i < other.getUserMap().bucket_count(); ++i) {
        for (auto local_it = other.getUserMap().begin(i); local_it != other.getUserMap().end(i); ++local_it) {
            User *new_usr = local_it->second;
            std::string cpy_name = local_it->first;
            this->getUserMap().insert({cpy_name, new_usr});        //adding user to userMap
            if (other.getActiveUser() == local_it->second)
                this->setActiveUser(new_usr);    // Active user
            local_it->second=nullptr;
        }
    }
    other.getUserMap().erase(other.getUserMap().begin(),other.getUserMap().end());  //DELETE Other
    return (*this);
}

//Start
void Session::start() {
    std::cout <<"SPFLIX is now on!\n";
    Session::setRunStat(true); // program is running

    ///MAIN LOOP
    while (this->getRunStat()){
        if( this->line.compare("")!=0){
            nextCommand(this->line);
        }
        else {
            std::string cmd;
            std::getline(std::cin, cmd);
            nextCommand(cmd);
        }
    }
}

//set_Run_stat
void Session::setRunStat(bool run){                             this->running=run;         }
//get_Run_stat
bool Session::getRunStat(){                                     return (this->running);    }
//addAction
void Session::addAction(BaseAction* currAction) {               this->actionsLog.push_back(currAction);      }
//getContent
std::vector<Watchable*> Session::getContent() {                 return this->content;       }
//getSesLine
std::string Session::getSesLine() {                             return this->line;          }
//setSesLine
void Session::setSesLine(std::string &str) {                    this->line=str;             }
//getUserMap
std::unordered_map<std::string,User*> Session::getUserMap(){    return this->userMap;       }
//setActiveUser
void Session::setActiveUser(User *name)  {                      this->activeUser=name;      }
//getActiveUser
User* Session::getActiveUser() {                                return this->activeUser;    }
//getActionLog
std::vector<BaseAction*> Session::getActionLog() {              return this->actionsLog;    }
//nextCommand
void Session::nextCommand(std::string &currLine) {

    //code for word by word from &currLine
    istringstream editedLine(currLine);
    std::string word;
    editedLine >> word;
    //end of word by word

    //check for valid action -> save currline to line -> call spesific action
    if(word.compare("createuser")==0){
        line =currLine.substr(11,currLine.length()-1);  //2 words
        BaseAction* p = new CreateUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("changeuser")==0){
        editedLine>>line;                                       //1 word
        BaseAction* p = new ChangeActiveUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("deleteuser")==0){
        editedLine>>line;
        BaseAction* p = new DeleteUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("dupuser")==0){
        line =currLine.substr(8,currLine.length()-1);
        BaseAction* p = new DuplicateUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("content")==0){
                                                                //no words
        BaseAction* p = new PrintContentList();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("watchhist")==0){
        editedLine>>line;
        BaseAction* p = new PrintWatchHistory();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("watch")==0){              /// <<<<<<<<<<< build it
        editedLine>>line;
        BaseAction* p = new Watch();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("log")==0){
        BaseAction* p = new PrintActionsLog();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("exit")==0){
        BaseAction* p = new Exit();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else{   cout<<"Error - no such command!\n";    }
}


/*  createuser
*  changeuser
*  deleteuser
*  dupuser
*  content
*  watchhist
 *  watch
 *  log
 *  exit
*/