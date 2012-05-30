src_main = "server.cpp "

src_ipaddress = "ipaddress.cpp "

src_server = "oxgameserver.cpp "

src_authapp = "authapp.cpp session.cpp user.cpp "

src_connector = "gamesconnector.cpp "

src_game = "fields.cpp OXGame.cpp"

liby = Split('boost_regex boost_thread wthttp wt wtdbo wtdbosqlite3 boost_unit_test_framework ');

if ARGUMENTS.get('test', '0') == '1':
	print "* * *BUILDING TESTS* * *"
	Program( 'connectorTest' , Split( src_connector ) , LIBS = liby , CCFLAGS=' -D__GAMESCONNECTOR_TEST__')
	Program('OXGameTest', Split(src_game), CCFLAGS=' -D__OXGAME_TEST__' )

else:
	print "* * *BUILDING RELEASE* * *"
	Program('server' ,Split(src_main + src_server + src_ipaddress + src_authapp) , LIBS = liby)
