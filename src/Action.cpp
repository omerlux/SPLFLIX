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
//createuser act
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
            error("unknown recommendation algorithm!");
        }
    }
    else{    error("user name is taken!");    }

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
        this->complete();
}
//createuser toString()
std::string CreateUser::toString() const {
    if(this->getStatus()==ERROR)
        cout<<"CreateUser "<<this->getStatus()<<": "<<this->getErrorMsg()<<"\n";
    else
        cout<<"CreateUser "<<this->getStatus()<<"\n";
}

//---------------------Class ChangeActiveUser----------------------
//changeactiveuser act
void ChangeActiveUser::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    std::string name = sess.getSesLine();
    if (sess.getUserMap().find(name) != sess.getUserMap().end()) { //check if NOT exists in the usermap
        sess.setActiveUser(  sess.getUserMap().find(name)->second  ); //this is the User*
    }
    else{    error("there is no such user!");   }

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
        this->complete();
}
//changeactiveuser toString()
std::string ChangeActiveUser::toString() const {
    if(this->getStatus()==ERROR)
        cout<<"ChangeActiveUser "<<this->getStatus()<<": "<<this->getErrorMsg()<<"\n";
    else
        cout<<"ChangeActiveUser "<<this->getStatus()<<"\n";
}

//---------------------Class DeleteUser----------------------
//DeleteUser act
void DeleteUser::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    std::string name = sess.getSesLine();
    if (sess.getUserMap().find(name) != sess.getUserMap().end()) { //check if NOT exists in the usermap
        if(sess.getUserMap().find(name)->second == sess.getActiveUser())
            sess.setActiveUser( sess.getUserMap().find("default")->second  );// if is active - set default
        sess.getUserMap().erase(name);           //delete name from the users
    }
    else{    error("there is no such user!");   }

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
        this->complete();
}
//DeleteUser toString()
std::string DeleteUser::toString() const {
    if(this->getStatus()==ERROR)
        cout<<"DeleteUser "<<this->getStatus()<<": "<<this->getErrorMsg()<<"\n";
    else
        cout<<"DeleteUser "<<this->getStatus()<<"\n";
}

//---------------------Class DuplicateUser----------------------
//DuplicateUser act
void DuplicateUser::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    //code for word by word
    istringstream edit(sess.getSesLine());
    std::string orig_name;
    edit >>orig_name;
    std::string new_name;
    edit >> new_name;
    //end word by word
    if(sess.getUserMap().find(orig_name) == sess.getUserMap().end()){  //check if orig EXISTS in the usermap
        if(sess.getUserMap().find(new_name) != sess.getUserMap().end()) {//check if new is NOT in the usermap
            std::string class_type = typeid( sess.getUserMap().find(orig_name)->second ).name(); //get class #name
            if(class_type.at(2) == 'L'){
                User* orig_usr = sess.getUserMap().find(orig_name)->second;         //temp copy usr
                User* new_usr = new LengthRecommenderUser(*orig_usr, new_name); //duplicate
                delete (orig_usr);                                                 //delete this tmp user
                sess.getUserMap().insert({new_name, new_usr});        //adding user to userMap
            }
            else if(class_type.at(2)=='R'){
                User* orig_usr = sess.getUserMap().find(orig_name)->second;         //temp copy usr
                User* new_usr = new RerunRecommenderUser(*orig_usr, new_name); //duplicate
                delete (orig_usr);                                                 //delete this tmp user
                sess.getUserMap().insert({new_name, new_usr});        //adding user to userMap
            }
            else if(class_type.at(2)=='G'){ // can be else - just to clearify
                User* orig_usr = sess.getUserMap().find(orig_name)->second;         //temp copy usr
                User* new_usr = new GenreRecommenderUser(*orig_usr, new_name); //duplicate
                delete (orig_usr);                                                 //delete this tmp user
                sess.getUserMap().insert({new_name, new_usr});        //adding user to userMap
            }
        }
        else{
            error("new name is taken!");            ///new name not available
        }
    }
    else{    error("there is no such user!");    } ///original name not found

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
        this->complete();
}
//DuplicateUser toString()
std::string DuplicateUser::toString() const {
    if(this->getStatus()==ERROR)
        cout<<"DuplicateUser "<<this->getStatus()<<": "<<this->getErrorMsg()<<"\n";
    else
        cout<<"DuplicateUser "<<this->getStatus()<<"\n";
}

//---------------------Class PrintContentList----------------------
//PrintContentList act
void PrintContentList::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    for(int i=0; i<sess.getContent().size(); i++){
        cout<<sess.getContent()[i]->toString()<<"\n";
    }
    /// when will error occure? ...
/*    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
*/        this->complete();
}
//PrintContentList toString()
std::string PrintContentList::toString() const {
/*    if(this->getStatus()==ERROR)
        cout<<"PrintContentList "<<this->getStatus()<<": "<<this->getErrorMsg()<<"\n";
    else
*/        cout<<"PrintContentList "<<this->getStatus()<<"\n";
}

//---------------------Class PrintWatchHistory----------------------
//PrintWatchHistory act
void PrintWatchHistory::act(Session &sess) {
    sess.addAction(this);               //added action to actionLog of the running session
    std::string name = sess.getSesLine();
    if (sess.getUserMap().find(name) != sess.getUserMap().end()) { //check if NOT exists in the usermap
        if(sess.getUserMap().find(name)->second == sess.getActiveUser()) {
            for(int i=0; i<sess.getActiveUser()->get_history().size(); i++)
                cout<< sess.getActiveUser()->get_history_i(i)->toString() <<"\n";
        }
        else{error(name+" is not the active user!")};
    }
    else{    error("there is no such user!");   }

    if(this->getStatus()==ERROR)
        cout<< "Error - "<< this->getErrorMsg()<<"\n";
    else
        this->complete();
}
//PrintWatchHistory toString()
std::string PrintWatchHistory::toString() const {
    if(this->getStatus()==ERROR)
        cout<<"PrintWatchHistory "<<this->getStatus()<<": "<<this->getErrorMsg()<<"\n";
    else
        cout<<"PrintWatchHistory "<<this->getStatus()<<"\n";
}

