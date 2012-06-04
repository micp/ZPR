#ifndef __EndOfGameListenerh__
#define __EndOfGameListenerh__
class Game;
#ifndef __GAMESCONNECTOR_TEST__
#include "game.h"
#endif
#include "fields.h"

//to ja zrobie pozniej bo to ja bede uzywal Ty tylko powadamiaj
class EndOfGameListener
{
public:
	/** Constructs end of game listinter tied to Game
	 */
	EndOfGameListener( Game *g);
	
	/** Notifies game endend if stalemate.
	 */
	void gameEnded(FieldEmpty f, int startX , int startY , int finishX , int finishY ); //gdy brak mozliwosci ruchu
	
	/** Notifies game ended if X won.
	 */
	void gameEnded(FieldX f, int startX , int startY , int finishX , int finishY ); //gdy wygral X
	
	/** Notifies game neded if O won.
	 */
	void gameEnded(FieldO f, int startX , int startY , int finishX , int finishY ); //gdy wygral Y
private:
	Game *g_;
};

#endif
