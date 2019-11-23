#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/json.hpp"
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
using namespace std;

//---------------------Class BaseAction----------------------
//Constructor
BaseAction::BaseAction(): status(PENDING), errorMsg(nullptr) {}

//getStatus
ActionStatus BaseAction::getStatus() const {            return this->status;        }
//complete
void BaseAction::complete() {                            this->status=COMPLETED;    }
//error
void BaseAction::error(const std::string &errorMsg) {    this->status=ERROR; this->errorMsg=errorMsg;   }
//getErrorMsg
std::string BaseAction::getErrorMsg() const{             return this->errorMsg;     }

//---------------------Class CreateUser----------------------
void CreateUser::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    //code for word by word
    istringstream edit(sess.getSesLine());
    std::string name;
    edit >>name;
    std::string alg;
    edit >> alg;
    //end word by word
    if(sess.getUserMap().find(name) != sess.getUserMap().end()){  //check if NOT exists in the usermap
        if(alg.compare("len")==0){
            User* usr = new LengthRecommenderUser(name);
            sess.getUserMap().insert({name,usr});        //adding user to userMap
        }
        else if(alg.compare("rer")==0){
            User* usr = new RerunRecommenderUser(name);
            sess.getUserMap().insert({name,usr});        //adding user to userMap
        }
        else if(alg.compare("gen")==0){
            User* usr = new GenreRecommenderUser(name);
            sess.getUserMap().insert({name,usr});        //adding user to userMap
        }
        else{
            /// error <<<<<<<<<<< build it
        }
    }
}
//---------------------Class ChangeActiveUser----------------------
