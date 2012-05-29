#ifndef __GAMESCONNECTORH__
#define __GAMESCONNECTORH__

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/shared_ptr>

#include <iterator>

#include "game.h"
#include "gamelistrefresher.h"

using namespace boost::interprocess;
using namespace boost;
using namespace std;

class IteratorNotValid {};

class GamesConnector
{
public:
	class const_iterator : public std::iterator<std::forward_iterator_tag, string >
	{
		friend class GameConnector;
	protected:
		shared_ptr<bool> valid_;
		set<Node>::iterator my_ele_;

      		const_iterator( set<Node>::iterator my_ele ) : my_ele_( my_ele ) , valid_( my_ele->valid_ ) {}
	
	public:
		const_iterator(){};

		const_iterator( const_iterator& other): my_ele_( other.my_ele_ ) , valid( other.valid_ );

		const string& operator*() const
		{
			if( !*valid_ ) throw IteratorNotValid();
		 	return my_ele_->game_.getServerUserName();	
	      	}

	      	const T* operator->() const
	      	{
			if( !*valid_ ) throw IteratorNotValid();
		 	return &( my_ele_->game_.getServerUserName() );	
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
			valid = my_ele_->valid_;
		  	return *this;
	      	}

	      	// post
	     	const_iterator operator--(int)
	      	{
		  	const_iterator tmp( *this );
			--(*this)
		  	return tmp;
	      	}

	      	bool operator==(const const_iterator& a) const
	      	{	return my_ele_ == a.my_ele_; }



	       	bool operator!=(const const_iterator& a) const
	      	{	return my_ele_ != a.my_ele_; }	      
		
		bool valid() const
		{ 	return *valid_; } 
	};

	GamesConnector& getInstance();

	//means that user would like to set a new game
	shared_ptr<Game> newGame( User &user );

	//after succesfull return from this function you will be no longer notified about list changes
	shared_ptr<Game> join( const_iterator &game , GameListRefresher &ref );
	
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
	const_iterator end() { return const_iterator( games_.end(); }

private:
	struct Node
	{
		Node( Game *game , bool *valid ): game_( game ) , valid_( valid ) {}
		shared_ptr<Game> game_;
		shared_ptr<bool> valid_;
	};
	
	class Synchronizer
	{
	public:
		Synchronizer(): readers_count( 0 ) , writters_count( 0 ) {}

		void readerEnter();

		void readerLeave();

		void writterEnter();

		void writterLeave();
	private:
		interprocess_mutex mutex_reader_;
		int readers_count;
		interprocess_mutex mutex_writter_;
		int writters_count;

		interprocess_mutex lock_;
		interprocess_mutex mutex_;
		interprocess_mutex may_read__;
	};	
	
	GamesConnector();
	
	static GamesConnector *instance_;
	static interprocess_mutex mutex_;
	set<shared_ptr<Node> > games_;
	set<GameListRefresher *> refreshers_;
	Synchronizer rw_synch_;
};	
#endif
