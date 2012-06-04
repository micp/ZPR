#ifndef __GAMESCONNECTORH__
#define __GAMESCONNECTORH__
#include <Wt/WServer>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include <Wt/Dbo/Types>
#include <Wt/Dbo/Dbo>
#include <Wt/WGlobal>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/Impl>
#include <Wt/Auth/Dbo/AuthInfo>

#include <iterator>
#include <set>
#include <string>

#include "user.h"

class Game;

using namespace boost::interprocess;
using namespace boost;
using namespace std;

//Exception class
/** Represents iterator error.
 */
class IteratorNotValid {};
class GameListRefresher;

//Class for connecting palyers into one game
class GamesConnector
{
public:
	struct Node
	{
		Node( ::Game *game , bool *valid ): game_( game ) , valid_( valid ) {}
		boost::shared_ptr<Game> game_;
		boost::shared_ptr<bool> valid_;
	};

	 class const_iterator 
        {
                friend class GamesConnector;
        protected:
                boost::shared_ptr<bool> valid_;
                set<Node>::iterator my_ele_;

                const_iterator( set<Node >::iterator my_ele ) :
                 my_ele_( my_ele ) , valid_( my_ele->valid_ ) {}

        public:
                const_iterator(){};

                const_iterator( const const_iterator& other): my_ele_( other.my_ele_ ) , valid_( other.valid_ ) {}

                const string& operator*() const;

                const string* operator->() const;

                // pre
                const_iterator& operator++();

                // post
                const_iterator operator++(int);

                // pre
                const_iterator& operator--();

                // post
                const_iterator operator--(int);

		bool operator==(const const_iterator& a) const;

		bool operator!=(const const_iterator& a) const;

		bool valid() const;
	};

	friend class const_iterator;

	static GamesConnector& getInstance();

	//means that user would like to set a new game
	boost::shared_ptr<Game> newGame( GameListRefresher &ref );

	//When you created game, there was no other player to play with you
	void deleteGame( boost::shared_ptr<Game> g );
 
	//after succesfull return from this function you will be no longer notified about list changes
	boost::shared_ptr<Game> join( const_iterator &game , GameListRefresher &ref );

	//call this if you are viewing a list and you'd like it to be allways actual
	void refRegister( GameListRefresher &ref );

	//you should allways call this method when actual list game is not neccesary anymore for you
	void unregister( GameListRefresher &ref );

	//DONOT use ++ -- operations on iterators not between this calls
	//This method should be called befor iteration over game list
	void iterationBegin() { rw_synch_.readerEnter(); }

	//This method must be called after iteration
	void iterationEnd() { rw_synch_.readerLeave(); }

	//DONOT use this unless between iterationBegin() and iterationEnd()
	const_iterator begin();

	//DONOT use this unless between iterationBegin() and iterationEnd()
	const_iterator end();

	//class which implements solution for readers and writters (writters prefered)
	//Do not hesitate to use it if you have to
	class Synchronizer
	{
	public:
		Synchronizer(): readers_count_( 0 ) , writters_count_( 0 ) {}

		void readerEnter();

		void readerLeave();

		void writterEnter();

		void writterLeave();
	private:
		interprocess_mutex mutex_reader_;
		int readers_count_;
		interprocess_mutex mutex_writter_;
		int writters_count_;

		interprocess_mutex lock_;
		interprocess_mutex mutex_;
		interprocess_mutex may_read_;
	};

private:


	GamesConnector(){}

	static GamesConnector *instance_;
	static interprocess_mutex mutex_;
	set<Node> games_;
	set<GameListRefresher *> refreshers_;
	Synchronizer rw_synch_;
};

#include "gamelistrefresher.h"
#ifndef __GAMESCONNECTOR_TEST__
#include "game.h"
#else
/**************************************************************************
			Just for testing
*************************************************************************/
#include <boost/thread/thread.hpp>
#include <boost/test/minimal.hpp>

class Game
{
	public:
	Game(GameListRefresher &ref): name_(ref.getUserName() ) {};
	void join( GameListRefresher &ref) {}
	const std::string& getServerUserName(){ return name_; }
	std::string name_;
};

class Ref: public GameListRefresher
{
	std::string name_;
public:
	Ref( User u): name_(u.getName() ) {};
	virtual void playerJoined() {};

	//both users presed start and the game has began
	virtual void gameStarted() {};

	//informs that pointed field has changed
	virtual void fieldChanged( FieldX &f , int x , int y ){};	
	
	virtual void fieldChanged( FieldO &f , int x , int y ) {};	

	//show another player why he has won
	virtual void wonByGivingUp() {};
	
	//notify antoher player that his oponent has ecsaped
	virtual void playerExited() {};

	//NOtify players that revange has been proposed
	//Players wher switched ( X is now O etc)
	//View should be taht same as in the begining
	// Game is waiting for two start pressed
	virtual void revengeProposed() {}; 	
	
	virtual void endedWithDraw(int startX, int startY, int finishX, int finishY ){};

	virtual void endedWithWin(int startX, int startY, int finishX, int finishY ){};

	virtual void endedWithLose(int startX, int startY, int finishX, int finishY ){};

	virtual std::string& getUserName() { return name_; }
	virtual std::string& getSessionID() { static std::string s; return s; }

	virtual Session * getSession() { return NULL; }
protected:
	virtual	void refreshGameList() {}
};

#endif
#endif
