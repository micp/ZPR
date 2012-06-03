#include "gamesconnector.h"

const string& GamesConnector::const_iterator::operator*() const
{
	if( !*valid_ ) throw IteratorNotValid();
	return my_ele_->game_->getServerUserName();
}

const string* GamesConnector::const_iterator::operator->() const
{
	if( !*valid_ ) throw IteratorNotValid();
	return &( my_ele_->game_->getServerUserName() );
}

// pre
GamesConnector::const_iterator& GamesConnector::const_iterator::operator++()
{
	if( my_ele_ != GamesConnector::getInstance().end().my_ele_ )
	{
		++my_ele_;
		valid_ = my_ele_->valid_;
	}
	return *this;
}

// post
GamesConnector::const_iterator GamesConnector::const_iterator::operator++(int)
{
	const_iterator tmp( *this );
	++(*this);
	return tmp;
}

// pre
GamesConnector::const_iterator& GamesConnector::const_iterator::operator--()
{
	--my_ele_;
	valid_ = my_ele_->valid_;
	return *this;
}

// post
GamesConnector::const_iterator GamesConnector::const_iterator::operator--(int)
{
	const_iterator tmp( *this );
	--(*this);
	return tmp;
}

bool GamesConnector::const_iterator::operator==(const const_iterator& a) const
{	return my_ele_ == a.my_ele_; }



bool GamesConnector::const_iterator::operator!=(const const_iterator& a) const
{	return my_ele_ != a.my_ele_; }

bool GamesConnector::const_iterator::valid() const
{ 	return *valid_; }

GamesConnector::const_iterator GamesConnector::begin() 
{ 
	return const_iterator( games_.begin() ); 
}


GamesConnector::const_iterator GamesConnector::end() 
{ 
	return const_iterator( games_.end() );  
}
//init instance
GamesConnector* GamesConnector::instance_ = NULL;

//declartion of static member
interprocess_mutex GamesConnector::mutex_;

bool operator<(const GamesConnector::Node& n1 , const GamesConnector::Node& n2)
{ 	return n1.game_->getServerUserName() < n2.game_->getServerUserName(); }

GamesConnector& GamesConnector::getInstance()
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
	lock_.unlock();
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

	--writters_count_;
	if( writters_count_ == 0 ) may_read_.unlock();

	mutex_writter_.unlock();
}

boost::shared_ptr<Game> GamesConnector::newGame( GameListRefresher &ref )
{
	rw_synch_.writterEnter();

	pair<set<Node>::iterator , bool> p;
	p = games_.insert( Node( new Game( ref ) , new bool(true) ) );

	for( set<GameListRefresher *>::iterator it = refreshers_.begin(); it != refreshers_.end() ; ++it )
	WServer::instance()->post((*it)->getSessionID(), boost::bind( &GameListRefresher::refreshGameList , (*it) ));
	
	rw_synch_.writterLeave();
	return p.first->game_;
}

boost::shared_ptr<Game> GamesConnector::join( const_iterator &game , GameListRefresher &ref )
{
	rw_synch_.writterEnter();

	if( !game.valid() ) { rw_synch_.writterLeave(); throw IteratorNotValid(); }

	*game.valid_ = false;

	boost::shared_ptr<Game> g = game.my_ele_->game_;
	games_.erase( game.my_ele_ );
	refreshers_.erase( &ref );
	g->join( ref );

	for( set<GameListRefresher *>::iterator it = refreshers_.begin(); it != refreshers_.end() ; ++it )
	WServer::instance()->post((*it)->getSessionID(), boost::bind( &GameListRefresher::refreshGameList, (*it) ));

	rw_synch_.writterLeave();
	return g;
}

void GamesConnector::refRegister( GameListRefresher &ref )
{
	rw_synch_.writterEnter();
	refreshers_.insert( &ref );
	rw_synch_.writterLeave();
}

void GamesConnector::unregister( GameListRefresher &ref )
{
	rw_synch_.writterEnter();
	refreshers_.erase( &ref );
	rw_synch_.writterLeave();
}

void GamesConnector::deleteGame( boost::shared_ptr<Game> g )
{
	rw_synch_.writterEnter();
	for( set<Node>::iterator it = games_.begin() ; it != games_.end() ; ++it )
	if( it->game_ == g )
	{
		games_.erase( it );
		for( set<GameListRefresher *>::iterator it = refreshers_.begin(); it != refreshers_.end() ; ++it )
		WServer::instance()->post((*it)->getSessionID(), boost::bind( &GameListRefresher::refreshGameList, (*it) ));
		break;
	}
	rw_synch_.writterLeave();
}

#ifdef __GAMESCONNECTOR_TEST__
class Reader
{
public:
	void operator()()
	{
		while( !*stop )
		{
			synch_->readerEnter();
			gc_->iterationBegin();
			set<string>::iterator itv = ref_->begin();
			for(GamesConnector::const_iterator it = gc_->begin(); it != gc_->end() ; ++it , ++itv )
			{
				BOOST_REQUIRE( itv != ref_->end() );
				BOOST_REQUIRE(  *itv == *it);
			}
			BOOST_REQUIRE( itv == ref_->end() );

			gc_->iterationEnd();
			synch_->readerLeave();
			usleep( 2000 );
		}
		return;
	}
	GamesConnector *gc_;
	set<string> *ref_;
	GamesConnector::Synchronizer *synch_;
	volatile bool *stop;
};

class Writter
{
public:
	void operator()()
	{
		int i = 0;
		char np[] = {'a' , '\0'};
		User u;
		u.setName( string(np) );

		while( !*stop )
		{
			Ref r( u );
			synch_->writterEnter();
			gc_->newGame( r );
			ref_->insert( u.getName() );
			synch_->writterLeave();

			np[0] += 1;
			u.setName( string(np) );
			++i;
			if(np[0] == '{') break;
			usleep( 23000 );
		}
	}
	GamesConnector *gc_;
	set<string> *ref_;
	GamesConnector::Synchronizer *synch_;
	volatile bool *stop;
};

int test_main( int a , char** c )
{
	GamesConnector* gc =& GamesConnector::getInstance();
	GamesConnector::Synchronizer* synch = new GamesConnector::Synchronizer();
	set<string> s;
	Reader r;
	Reader r2;
	Writter w;
	bool f(false);
	r.gc_ = r2.gc_ = w.gc_ = gc;
	r.ref_ = r2.ref_ = w.ref_ = &s;
	r.synch_ = r2.synch_ = w.synch_ = synch;
	r.stop = r2.stop = w.stop = &f;

	cout<<"**** TESTING class GamesConnector with synchronization"<<endl<<endl;
	cout<<"**** readers + writter start"<<endl; cout.flush();
	boost::thread thr( r );
	boost::thread thr2( r2 );
	w();
	BOOST_REQUIRE( thr.joinable() && thr2.joinable() );
	cout<<"**** readers + writter ok"<<endl; cout.flush();
	cout<<"**** readers only start"<<endl; cout.flush();
	sleep( 2 );
	BOOST_REQUIRE( thr.joinable() && thr2.joinable() );
	cout<<"**** readers only ok "<<endl; cout.flush();
	f = true;
	thr.join();
	thr2.join();
	delete gc;
	delete synch;
	return 0;
}
#endif



























