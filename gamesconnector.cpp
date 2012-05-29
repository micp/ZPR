#include "gamesconnector.h"

//init instance
GamesConnector* GamesConnector::instance_ = NULL;

//declartion of static member 
interprocess_mutex GamesConnector::mutex_;

GamesConnector& GamesConnector::getServer()
{
	if( instance_ == NULL )
	{
		scoped_lock<interprocess_mutex> lock( mutex_ );
		if( instance_ == NULL ) instance_ = new GamesConnector();
	}
	return *instance_;
}

void GamesConnector::Synchronizer::readerEnter()
{
	lock_.lock();
	may_read_.lock();
	mutex_reader_.lock();

	++readers_count_;
	if( readers_count_ == 1 ) mutex_.lock();

	mutex_reader_.unlock();
	may_read_.unlock();
	lock_.lock();
}

void GamesConnector::Synchronizer::readerLeave()
{
	mutex_reader_.lock();
	
	--readers_count_;
	if(readers_count_ == 0 ) mutex_.unlock();

	mutex_reader_.unlock();
}

void GamesConnector::Synchronizer::writterEnter()
{
	mutex_writter_.lock();

	++writters_count_;
	if( writters_count_ == 1 ) may_read_.lock();

	mutex_writter_.unlock();
	
	mutex_.lock();
}

void GamesConnector::Synchronizer::writterLeave()
{
	mutex_.unlock();

	mutex_writter_.lock();

	--writters_count;
	if( writters_count == 0 ) may_read_.unlock();

	mutex_writter_.lock();
}

shared_ptr<Game> GamesConnector::newGame( User &user )
{
	rw_synch_.writterEnter();

	pair<set<Node>::iterator , bool> p;
	p = games_.insert( new Game( user ) , new bool(true) );	
	
	for( set<GameListRefresher *>::iterator it = refreshers_.begin(); it != refreshers_.end() ; ++it )
					(*it)->refreshGameList();
	rw_synch_.writterLeave();
	return p.first()->game_;
}

shared_ptr<Game> GamesConnector::join( const_iterator &game , GameListRefresher &ref )
{
	rw_synch_.writterEnter();
	
	if( !game.valid() ) { rw_synch_.writterLeave(); throw IteratorNotValid(); }

	*game.valid_ = false;

	shared_ptr<Game> g = game.my_ele_->game_;
	games_.erase( game.my_ele_ );
	refreshers_.erase( &ref );
	g->join( ref );  
	
	for( set<GameListRefresher *>::iterator it = refreshers_.begin(); it != refreshers_.end() ; ++it )
					(*it)->refreshGameList();

	rw_synch_.writterLeave();
	return g;
}

void GamesConnector::unregister( GameListRefresher &ref )
{
	rw_synch_.writterEnter();
	refreshers_.erase( &ref );
	rw_synch_.writterLeave();
}































