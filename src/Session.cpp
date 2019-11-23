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
                j["movies"][k]["length"],j["movies"][k]["tags"]));
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
    Session::setRunStat(); // program is running
    std::string cmd;

    if(firstRun){
        cmd = "createuser default";
        Session::nextCommand(cmd);
        cmd = "changeuser default";
        Session::nextCommand(cmd);
        this->firstRun=false;                   //only first start will create default.
    }
    else{
        cmd = "changeuser default";
        Session::nextCommand(cmd);
    }

    //User* Default = new LengthRecommenderUser("default"); //------- in CreateUser
    ///this->activeUser=Default;          ///--- will be in change Active user <<<<<   build it
    //this->userMap.insert({"default",Default});              //------- in CreateUser
    //this->actionsLog.push_back(&CreateUser);                //------- in CreateUser
}

//set_Run_stat
void Session::setRunStat(){                                     this->running=~running;    }
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
//nextCommand
void Session::nextCommand(std::string &currLine) {

    //code for word by word from &currLine
    istringstream editedLine(currLine);
    std::string word;
    editedLine >> word;
    //end of word by word

    //check for valid action -> save currline to line -> call spesific action
    if(word.compare("createuser")==0){
        line =currLine.substr(11,currLine.length()-1);
        BaseAction* p = new CreateUser();
        p->act(*this);                      //now p is connected to ActionLog, and more stuff
    }
    else if(word.compare("changeuser")==0){         /// <<<<<<<<<<< build it
        editedLine>>line;

    }
    else if(word.compare("deleteuser")==0){         /// <<<<<<<<<<< build it
        editedLine>>line;

    }
    else if(word.compare("dupuser")==0){            /// <<<<<<<<<<< build it
        line =currLine.substr(8,currLine.length()-1);

    }
    else if(word.compare("content")==0){            /// <<<<<<<<<<< build it

    }
    else if(word.compare("watchhist")==0){          /// <<<<<<<<<<< build it

    }
    else if(word.compare("watch")==0){              /// <<<<<<<<<<< build it

    }
    else if(word.compare("log")==0){                /// <<<<<<<<<<< build it

    }
    else if(word.compare("exit")==0){               /// <<<<<<<<<<< build it

    }
    else{   cout<<"error: no such command!";    }
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