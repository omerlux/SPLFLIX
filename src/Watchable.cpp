#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
using namespace std;

//---------------------Class Watchable----------------------
//Constructor
    //Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id),length(length), tags(tags){}
//Destructor
    //Watchable::~Watchable() {}
//Copy Constructor
    //Watchable::Watchable(const Watchable &other): id(other.id), length(other.length), tags(other.tags) {}
//Move Constructor
    //Watchable::Watchable(Watchable &&other): id(other.id), length(other.length), tags(other.tags){}
//Copy Assignment
    //Watchable& Watchable::operator=(const Watchable &other){}
//Move Assignment
    //Watchable& Watchable::operator=(Watchable &&other){}
//getId
long Watchable::getId() const {    return id;   }
//getLength
int Watchable::getLength() const {    return length;   }
//getTags
const std::vector <std::string>& Watchable::getTags() const{   return tags;  }


//---------------------Class Movie----------------------
//Constructor
Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string> &tags) :
        name(name), Watchable(id,length,tags) {}
//Desctructor
Movie::~Movie() {}
//getName
const std::string& Movie::getName() const {     return name;    }
//toString
std::string Movie::toString()  const {
    std::string&& tmp=this->getId()+" "+this->getName()+" "+
            std::to_string(this->getLength())+" minutes [";
    for(int i=0; i<this->getTags().size()-1; i++)
        tmp = tmp + this->getTags()[i]+", ";
    tmp = tmp + this->getTags().back()+"]";
    return (tmp) ;
}
//get Name for "watch"
std::string Movie::only_name() const{
    return (this->getName());
}
//getNextWathable
Watchable* Movie::getNextWatchable(Session &sess) const {    return sess.getActiveUser()->getRecommendation(sess);  }


//---------------------Class Episode----------------------
//Constructor
Episode::Episode(long id, const std::string& seriesName, int length,
        int season, int episode, const std::vector<std::string> &tags) :
        seriesName(seriesName), season(season), episode(episode),  Watchable(id,length,tags) {}
//Destructor
Episode::~Episode() {}
//getSeriesName
const std::string& Episode::getSeriesName() const {     return seriesName;      }
//getSeason
int Episode::getSeason() const {                        return season;   }
//getEpisode
int Episode::getEpisode() const {                       return episode;   }
//setNextEpisodeId
void Episode::setNextEpisodeId(long nxt_id)  {          this->nextEpisodeId =(nxt_id);            }
//toString
std::string Episode::toString()  const {
    std::string&& tmp=this->getId()+" "+this->getSeriesName()
            +" S"+to_string(this->getSeason())+"E"+to_string(this->getEpisode())+" "+
            std::to_string(this->getLength())+" minutes [";
    for(int i=0; i<this->getTags().size()-1; i++)
        tmp = tmp + this->getTags()[i]+", ";
    tmp = tmp + this->getTags().back()+"]";
    return (tmp);
}
//get Name for "watch"
std::string Episode::only_name() const{
    std::string&& tmp=this->getSeriesName()+" S"+to_string(this->getSeason())+"E"+to_string(this->getEpisode());
    return (tmp);
}
//getNextWatchable
Watchable* Episode::getNextWatchable(Session &sess) const {
    //check by id for fast find
    if(this->nextEpisodeId!=0){
        if(typeid(sess.getActiveUser()).name()[2]  == 'L'){ //
            sess.getActiveUser()->setWatched_i(this->getId()-1 );   //setting watched table
            sess.getActiveUser()->setAvg(this->getLength()); //compute avg
        }
        else if(typeid(sess.getActiveUser()).name()[2]  == 'G'){
            sess.getActiveUser()->setWatched_i(this->getId()-1 );   //setting watch table
            sess.getActiveUser()->addGenre((Watchable *) this);        //adding genres list
        }
        return sess.getContent()[this->nextEpisodeId];               //TV series have more episodes
    }
    else{
        return sess.getActiveUser()->getRecommendation(sess);    //Last episode
    }
}
