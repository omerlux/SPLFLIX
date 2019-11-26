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
    User(User &&other);
    //Copy Assignment
    User& operator=(const User &other);
    //Move Assignment
    User& operator=(User &&other);
    //getHistory_i
    Watchable* get_history_i(int i) const;
    //setName
    void setName(std::string &new_name);
    //getWatched
    std::vector<bool> getWatched();
    //setWatched
    void setWatched_i(int i);
    //create watch vector
    void CreateWatched(Session &);
    //setAvg
    void setAvg (int length_val);
    //getAvg
    double getAvg () const;
    //getGenres
    std::vector<std::pair<std::string,int>> getGenres ();
    //addGenre
    void addGenre (Watchable*);
    //getMstWatchedGenre
    std::string getMstWatchedGenre() const;
    //User clone
    virtual User* clone() ;
    ///-------------Edited End-------------------
protected:
    std::vector<Watchable*> history;
private:
    std::string name;
    ///-------------Edited Start-----------------
    std::vector<bool> watched;
    std::vector<std::pair<std::string,int>> Genres;
    std::pair<std::string,int>* mstWatchedGenre;
    double avg;

    ///-------------Edited End-------------------
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ///-------------Edited Start-----------------
    LengthRecommenderUser(const User &other);
    //GenreRecommenderUser clone
    virtual User* clone() ;
    ///-------------Edited End-------------------
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ///-------------Edited Start-----------------
    RerunRecommenderUser(const User &other);
    //GenreRecommenderUser clone
    virtual User* clone() ;
    ///-------------Edited End-------------------
private:
    ///-------------Edited Start-----------------
    int rerun_next_index;
    ///-------------Edited End-------------------
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    ///-------------Edited Start-----------------
    GenreRecommenderUser(const User &other);
    //GenreRecommenderUser clone
    virtual User* clone() ;
    ///-------------Edited End-------------------
private:
};

#endif
