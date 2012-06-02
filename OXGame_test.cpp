#include "OXGame_test.h"

char getType(const Field& f) {
  Field* tmp = f.clone();
  FieldTypeVisitor v;
  tmp->accept(v);
  delete tmp;
  return v.getResult();
}

BOOST_AUTO_TEST_SUITE( OXGame_model_test )

BOOST_AUTO_TEST_CASE( Constructor_test ) {
  OXGame game;
  BOOST_CHECK_NO_THROW( game.getField(14, 14) );
  //x, y out of bounds?
  //BOOST_CHECK_THROW( game.getField(16, 15) );
  BOOST_CHECK_EQUAL( getType(game.getField(13, 13)), '0' );
  //size < 0 ?
  OXGame game2(20);
  BOOST_CHECK_NO_THROW( game2.getField(19, 19) );
  BOOST_CHECK_EQUAL( getType(game2.getField(19, 19)), '0' );
}

BOOST_AUTO_TEST_CASE( Put_test ) {
  OXGame game;
  FieldX fx;
  //x, y out of bounds?
  BOOST_CHECK_NO_THROW( game.put(fx, 1, 1) );
  BOOST_CHECK_EQUAL( getType(game.getField(1, 1)), 'x');
  BOOST_CHECK_THROW( game.put(fx, 1, 1), FieldTakenException );
}

BOOST_AUTO_TEST_CASE( Reset_test ) {
  OXGame game;
  FieldX fx;
  game.put(fx, 1, 14);
  game.resetGame();
  BOOST_CHECK_EQUAL( getType(game.getField(1, 14)), '0' );
}

BOOST_AUTO_TEST_CASE( Out_of_moves_test ) {
  OXGame game;
  FieldX fx;
  FieldO fo;
  TestEndOfGameListener l;
  game.addEndOfGameListener(l);
  for(int i = 0; i < 15; ++i)
    for(int j = 0; j < 15; ++j) {
      if(i%2) {
	if(j%8 < 4)
	  game.put(fx, i, j);
	else
	  game.put(fo, i, j);
      }
      else {
	if(j%8 < 4)
	  game.put(fo, i, j);
	else
	  game.put(fx, i, j);
      }
    }
  BOOST_CHECK_EQUAL( l.calledBy(), '0' );
}

BOOST_AUTO_TEST_CASE( Game_victory_test ) {
  OXGame game;
  FieldX fx;
  FieldO fo;
  TestEndOfGameListener l[8];

  game.addEndOfGameListener(l[0]);
  game.addEndOfGameListener(l[1]);
  for(int i = 0; i < 5; ++i) {
    BOOST_CHECK_EQUAL( l[0].calledBy(), 'u' );
    BOOST_CHECK_EQUAL( l[1].calledBy(), 'u' );
    game.put(fx, 0, i);
  }
  BOOST_CHECK_EQUAL( l[0].calledBy(), 'x' );
  BOOST_CHECK_EQUAL( l[1].calledBy(), 'x' );
  game.resetGame();

  game.addEndOfGameListener(l[2]);
  for(int i = 0; i < 5; ++i) {
    BOOST_CHECK_EQUAL( l[2].calledBy(), 'u' );
    game.put(fo, i, 0);
  }
  BOOST_CHECK_EQUAL( l[2].calledBy(), 'o' );
  game.resetGame();

  game.addEndOfGameListener(l[3]);
  for(int i = 5; i < 10; ++i) {
    BOOST_CHECK_EQUAL( l[3].calledBy(), 'u' );
    game.put(fx, i, i);
  }
  BOOST_CHECK_EQUAL( l[3].calledBy(), 'x' );
  game.resetGame();

  game.addEndOfGameListener(l[4]);
  for(int i = 5; i < 10; ++i) {
    BOOST_CHECK_EQUAL( l[4].calledBy(), 'u' );
    game.put(fx, 10-i, 10-i);
  }
  BOOST_CHECK_EQUAL( l[4].calledBy(), 'x' );
  game.resetGame();

  game.addEndOfGameListener(l[5]);
  for(int i = 5; i < 10; ++i) {
    BOOST_CHECK_EQUAL( l[5].calledBy(), 'u' );
    game.put(fx, i, 10 - i);
  }
  BOOST_CHECK_EQUAL( l[5].calledBy(), 'x' );
  game.resetGame();

  game.addEndOfGameListener(l[6]);
  int tab[5][2];
  tab[0][0] = 0;
  tab[0][1] = 0;
  tab[1][0] = 2;
  tab[1][1] = 0;
  tab[2][0] = 3;
  tab[2][1] = 0;
  tab[3][0] = 4;
  tab[3][1] = 0;
  tab[4][0] = 1;
  tab[4][1] = 0;
  for(int i = 0; i < 5; ++i) {
    BOOST_CHECK_EQUAL( l[6].calledBy(), 'u' );
    game.put(fx, tab[i][0], tab[i][1]);
  }
  BOOST_CHECK_EQUAL( l[6].calledBy(), 'x' );
  game.resetGame();

  game.addEndOfGameListener(l[7]);
  tab[0][0] = 1;
  tab[0][1] = 1;
  tab[1][0] = 2;
  tab[1][1] = 2;
  tab[2][0] = 3;
  tab[2][1] = 3;
  tab[3][0] = 4;
  tab[3][1] = 4;
  tab[4][0] = 0;
  tab[4][1] = 0;
  for(int i = 0; i < 5; ++i) {
    BOOST_CHECK_EQUAL( l[7].calledBy(), 'u' );
    game.put(fx, tab[i][0], tab[i][1]);
  }
  BOOST_CHECK_EQUAL( l[7].calledBy(), 'x' );
  game.resetGame();
  //maybe more patterns / mixed patterns here?
}

BOOST_AUTO_TEST_CASE( Game_ended_test ) {
  OXGame game;
  FieldX fx;
  FieldO fo;

  for(int i = 0; i < 5; ++i)
    game.put(fx, 0, i);
  game.put(fo, 14, 14);
  BOOST_CHECK_EQUAL( getType(game.getField(14, 14)), '0' );
}

BOOST_AUTO_TEST_SUITE_END()
