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
using std::cin;
using std::endl;
using std::string;
using std::getline;

int main(int argc, char* argv[])
{
	string host;
	uint16_t port = 0;
	string passwd;
	if(argc > 1) {
		host = string(argv[1]);
		if(argc > 2) {
			port = static_cast<uint16_t>(strtoul(argv[2], nullptr, 10));
		}
	}
	while(host.empty()) {
		cout << "Please enter the name or IP addres of the server:" << endl;
		getline(cin, host);
	}
	while(port == 0) {
		cout << "Please enter the port of the server:" << endl;
		string p;
		getline(cin, p);
		port = static_cast<uint16_t>(strtoul(p.c_str(), nullptr, 10));
	}
	while(passwd.empty()) {
		cout << "Please enter the password:" << endl;
		getline(cin, passwd);
	}
	boost::asio::io_service io_service;
	patxi::a3s server(
		 io_service
		,port
		,host
		,[](char const* data, std::size_t length)
		{
			string msg(data, length);
			cout << msg << endl;
		}
		,[&server](bool logged)
		{
			if(logged) {
				cout << "Successfully logged in." << endl;
				server.consoleInput();
			} else {
				cout << "Failed to log in" << endl;
			}
		}
	);
	server.login(passwd);
	io_service.run();
	return 0;
}
