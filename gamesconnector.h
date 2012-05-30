#ifndef __GAMESCONNECTORH__
#define __GAMESCONNECTORH__

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/shared_ptr.hpp>

#include <iterator>
#include <set>
#include <string>

#include "gamelistrefresher.h"
#include "user.h"

#ifdef __GAMESCONNECTOR_TEST__
/**************************************************************************
			Just for testing
*************************************************************************/
#include <boost/thread/thread.hpp>
#include <boost/test/minimal.hpp>

class Game
{
	public:
	Game(User &user): name_(user.getName() ) {};
	void join( GameListRefresher &ref) {}
	const std::string& getServerUserName(){ return name_; }	
	std::string name_; 
};

class Ref: public GameListRefresher
{
protected:
	virtual	void refreshGameList() {}
};
 
#else
//when normal compilation
#include "game.h"
#endif

using namespace boost::interprocess;
using namespace boost;
using namespace std;

//Exception class
class IteratorNotValid {};

//Class for connecting palyers into one game
class GamesConnector
{
public:
	struct Node
	{
		Node( Game *game , bool *valid ): game_( game ) , valid_( valid ) {}
		boost::shared_ptr<Game> game_;
		boost::shared_ptr<bool> valid_;
	};

	class const_iterator : public std::iterator<std::forward_iterator_tag, string >
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

		const string& operator*() const
		{
			if( !*valid_ ) throw IteratorNotValid();
		 	return my_ele_->game_->getServerUserName();	
	      	}

	      	const string* operator->() const
	      	{
			if( !*valid_ ) throw IteratorNotValid();
		 	return &( my_ele_->game_->getServerUserName() );	
	      	}

	      	// pre
	      	const_iterator& operator++()
	      	{
		  	if( my_ele_ != GamesConnector::getInstance().end().my_ele_ )
			{
				++my_ele_;
				valid_ = my_ele_->valid_;
			}		
		  	return *this;
	      	}

	      	// post
	       	const_iterator operator++(int)
	      	{
		  	const_iterator tmp( *this );
			++(*this);
		  	return tmp;
	      	}

	      	// pre
	      	const_iterator& operator--()
	      	{
		 	--my_ele_;
			valid_ = my_ele_->valid_;
		  	return *this;
	      	}

	      	// post
	     	const_iterator operator--(int)
	      	{
		  	const_iterator tmp( *this );
			--(*this);
		  	return tmp;
	      	}

	      	bool operator==(const const_iterator& a) const
	      	{	return my_ele_ == a.my_ele_; }



	       	bool operator!=(const const_iterator& a) const
	      	{	return my_ele_ != a.my_ele_; }	      
		
		bool valid() const
		{ 	return *valid_; } 
	};

	friend class const_iterator;

	static GamesConnector& getInstance();

	//means that user would like to set a new game
	boost::shared_ptr<Game> newGame( User &user );

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
	const_iterator begin() { return const_iterator( games_.begin() ); }

	//DONOT use this unless between iterationBegin() and iterationEnd()
	const_iterator end() { return const_iterator( games_.end() );  }
	
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
#endif
