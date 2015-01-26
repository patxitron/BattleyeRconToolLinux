/**
 * @file main.cpp
 *
 *
 * @date 24/11/2014
 * @uthor patxi
 */
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "a3s.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;
	utum::a3s server(
		 io_service
		,2302
		,"juegos.utum.es"/*"37.59.46.63"*/
		,[](char const* data, std::size_t length)
		{
			string msg(data, length);
			cout << msg << endl;
		}
		,[&server](bool logged)
		{
			if(logged) {
				cout << "Successfully logged in." << endl;
				server.send("players");
			} else {
				cout << "Failed to log in" << endl;
			}
		}
	);
	server.login("U8WX4ytD3xB3");
	io_service.run();
	return 0;
}
