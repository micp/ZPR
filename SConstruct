src_server = "server.cpp "
src_ipaddress = "ipaddress.cpp"
liby = Split('boost_regex wthttp wt');
Program('server' ,Split( src_server + src_ipaddress) , LIBS = liby)
