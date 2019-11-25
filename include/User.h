#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;


class User{
public:
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    ///-------------Edited Start-----------------
    //Destructor
    virtual ~User();
    //Copy Constructor
    User(const User &other);      // copy to ANOTHER name
    //Move Constructor
        //User(User &&other);
    //Copy Assignment
    User& operator=(const User &other);
    //Move Assignment
        //User& operator=(User &&other);
    //getHistory_i
    Watchable* get_history_i(int i) const;
    //setName
    void setName(std::string &new_name);
    ///-------------Edited End-------------------
protected:
    std::vector<Watchable*> history;
private:
    std::string name;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ///-------------Edited Start-----------------
    LengthRecommenderUser(const User &other);
    ///-------------Edited End-------------------
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ///-------------Edited Start-----------------
    RerunRecommenderUser(const User &other);
    ///-------------Edited End-------------------
private:
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ///-------------Edited Start-----------------
    GenreRecommenderUser(const User &other);
    ///-------------Edited End-------------------
private:
};

#endif
