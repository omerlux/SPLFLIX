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
Session::Session(const std::string &configFilePath) {
    firstRun= true;
    running = false;
    line="";
    std::ifstream i("../config2.json");
    json j;
    i >>j;
    //switch() /// import json parameters for watchables
    for(int k=0; k<j["movies"].size(); k++){
        this->content.push_back(new Movie(this->content.size()+1,j["movies"][k]["name"],
                j["movies"][k]["length"],j["movies"][k]["tags"]));  ///WATCH FOR ERRORS WITH USING JSON
    }
    for(int k=0; k<j["tv_series"].size(); k++){
        for(int s=1; s<=j["tv_series"][k]["seasons"].size(); s++)
            for(int e=1; e<=j["tv_series"][k]["seasons"][s-1]; e++)
                this->content.push_back(new Episode(this->content.size()+1,j["tv_series"][k]["name"],
                        j["tv_series"][k]["episode_length"],s,e,j["tv_series"][k]["tags"] ));
    }
    /// Add config 1 ?????
}

//Start
void Session::start() {
    std::cout <<"SPFLIX is now on!";
    Session::setRunStat(true); // program is running
    std::string cmd;

    if(firstRun){
        cmd = "createuser default len";
        Session::nextCommand(cmd);
        cmd = "changeuser default";
        Session::nextCommand(cmd);
        this->firstRun=false;                   //only first start will create default.
    }
    else{
        cmd = "changeuser default";
        Session::nextCommand(cmd);
    }

    ///MAIN LOOP
    while (this->getRunStat()){






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
        ///ACT -> go to watchable get reccomendation (Watchable) -> than from the user (User)???
    }
    else if(word.compare("log")==0){
        BaseAction* p = new PrintActionsLog();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("exit")==0){
        BaseAction* p = new Exit();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else{   cout<<"Error - no such command!";    }
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