#ifndef __GAMELISTREFRESHERH__
#define __GAMELISTREFRESHERH__

class GameListRefresher
{
	friend class GamesConnector;
	
	//user joined game
	virtual void playerJoined() = 0;

	//both users presed start and the game has began
	virtual void gameStarted() = 0;

	//informs that pointed field has changed
	virtual void fieldChanged( FieldX &f , int x , int y ) = 0;	
	
	virtual void fieldChanged( FieldO &f , int x , int y ) = 0;	

	//show when player has given up
	virtual void givedUp() = 0;	
	
	//show another player why he has won
	virtual void wonByGivingUp() = 0;
	
	//notify antoher player that his oponent has ecsaped
	virtual void playerExited() =0;

	//NOtify players that revange has been proposed
	//Players wher switched ( X is now O etc)
	//View should be taht same as in the begining
	// Game is waiting for two start pressed
	virtual void revangeProposed() = 0; 	
	
protected:
	//This metod may not use iterationBegin and iterationEnd
	//it may as the only function iterates the list witgout it
	virtual	void refreshGameList() = 0;
};

#endif
