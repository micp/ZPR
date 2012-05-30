#ifndef __GAMELISTREFRESHERH__
#define __GAMELISTREFRESHERH__

class GameListRefresher
{
	friend class GamesConnector;
	protected:
	//This metod may not use iterationBegin and iterationEnd
	//it may as the only function iterates the list witgout it
	virtual	void refreshGameList() = 0;
};

#endif
