/**
 * @file a3s.cpp
 *
 *
 * @date 24/11/2014
 * @uthor patxi
 */

#include "a3s.h"
#include <unistd.h>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <boost/crc.hpp>

using boost::asio::ip::udp;
using std::string;
using std::stringstream;
using std::strlen;
using std::memcpy;
using std::cerr;
using std::endl;


namespace patxi {



a3s::a3s(boost::asio::io_service& io_service, uint16_t port
        ,std::string const& addr, on_data_func on_data, on_login_func on_login)
		:loged_in_(false)
		,listening_(false)
		,sequence_(0)
		,on_data_(on_data)
		,on_login_(on_login)
		,socket_(io_service, udp::endpoint(udp::v4(), 0))
		,input_(io_service, ::dup(STDIN_FILENO))
		,console_data_(MAX_COMMAND_LENGTH)
{
	stringstream str_port;
	str_port << port;
	udp::resolver resolver(io_service);
	endpoint_ = *resolver.resolve({udp::v4(), addr, str_port.str()});
}



a3s::~a3s()
{
	socket_.close();
}



void a3s::login(std::string const& password)
{
	if(!listening_) {
		send_request(password.data(), password.size(), LOGIN);
		listening_ = true;
		receive();
	}
}



void a3s::send(const char* data, std::size_t length)
{
	if(loged_in_) send_request(data, length ? length : strlen(data));
}



void a3s::receive()
{
	socket_.async_receive_from(
		 boost::asio::buffer(net_data_, MSG_MAX_LENGTH)
		,endpoint_
		,[this](boost::system::error_code ec, std::size_t length)
		{
			if(!ec && length > 0) {
				request_t* req = reinterpret_cast<request_t*>(net_data_);
				if(req->hdr.begin_header[0] == 'B'
							&& req->hdr.begin_header[1] == 'E'
							&& req->hdr.end_header == 0xff) {
					boost::crc_32_type crc;
					crc.process_bytes(net_data_ + 6, length - 6);
					if(req->hdr.checksum == crc.checksum()) {
						if(req->hdr.begin_msg == 0x00) {
							// Login response or multipart packet
							if(req->payload[0] == 0x00 || req->payload[0] == 0x01) {
								// Login response
								loged_in_ = req->payload[0] == 0x01;
								if(on_login_) on_login_(loged_in_);
							} else {
								// Multipart packet
								if(on_data_) {
									on_data_(
										req->payload,
										length - sizeof(reqheader_t)
									);
								}
							}
						} else if(req->hdr.begin_msg == 0x01) {
							// Normal response
							if(on_data_) {
								on_data_(
									req->payload,
									length - sizeof(reqheader_t)
								);
							}
						} else if(req->hdr.begin_msg == 0x02) {
							// Broadcast packet
							send_request(&req->payload[0], 1, BROADCAST_RESPONSE);
						} else {
							cerr << "Invalid Battleye protocol." << endl;
						}
					} else {
						cerr << "Bad checksum." << endl;
					}
				} else {
					cerr << "Not Battleye protocol." << endl;
				}
			} else {
				cerr << "Error receiving response." << endl;
			}
			receive();
		}
	);
}



void a3s::consoleInput()
{
	boost::asio::async_read_until(
		 input_
		,console_data_
		,'\n'
		,[this](boost::system::error_code const& ec, std::size_t length)
		{
			if(!ec && length > 0) {
				send_request(boost::asio::buffer_cast<char const*>(console_data_.data()), length - 1);
				console_data_.consume(length);
			}
			consoleInput();
		}
	);
}



void a3s::send_request(char const* data, std::size_t length, request_type rtp)
{
	request_t req;
	char* request = reinterpret_cast<char*>(&req);
	if(rtp == LOGIN) {
		memcpy(req.payload, data, length);
		length += sizeof(reqheader_t);
	} else if(rtp == BROADCAST_RESPONSE) {
		req.hdr.begin_msg = 0x02;
		memcpy(req.payload, data, length);
		length += sizeof(reqheader_t);
	} else {
		req.hdr.begin_msg = 0x01;
		req.payload[0] = *reinterpret_cast<char*>(&sequence_);
		sequence_ += 1;
		memcpy(req.payload + 1, data, length);
		length += sizeof(reqheader_t) + 1;
	}
	boost::crc_32_type crc;
	crc.process_bytes(request + 6, length - 6);
	req.hdr.checksum = crc.checksum();
	socket_.async_send_to(
		boost::asio::buffer(request, length),
		endpoint_,
		[](boost::system::error_code ec, std::size_t /*bytes_sent*/)
		{
			if(ec) {
				cerr << "Error sending request" << ec << endl;
			}
		}
	);
}

} // namespace patxi
