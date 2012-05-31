#ifndef __EndOfGameListenerh__
#define __EndOfGameListenerh__

//to ja zrobie pozniej bo to ja bede uzywal Ty tylko powadamiaj
class EndOfGameListener
 {
public:
	void gameEnded(FieldEmpty f, int startX , int startY , int finishX , int finishY ); //gdy brak mozliwosci ruchu
	void gameEnded(FieldX f, int startX , int startY , int finishX , int finishY ); //gdy wygral X
	void gameEnded(FieldO f, int startX , int startY , int finishX , int finishY ); //gdy wygral Y
};

#endif
