#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    ///-------------Edited Start-----------------
    bool getRunStat();                              //get running value
    void setRunStat();                              //change running value
    void addAction(BaseAction* currAction);         //add last action to actionLog
    std::vector<Watchable*> getContent();           //return vector of content
    std::string getSesLine();                       //get session.line
    void setSesLine(std::string& str);              //set session.line
    std::unordered_map<std::string,User*> getUserMap();     //get user map
    void nextCommand(std::string& currLine);        //command getter from console
    ///-------------Edited End-----------------
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap; //watch the site for more info
    User* activeUser;
    ///-------------Edited Start-----------------
    bool running;                                  //program flow indicator
    std::string line;                              //line of command
    /// Creation of function to delete Line after each use ???????
    bool firstRun;
    ///-------------Edited End-----------------
};

#endif
