/**
 * @file a3s.h
 *
 *
 * @date 24/11/2014
 * @uthor patxi
 */

#ifndef A3S_H_
#define A3S_H_

#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <string>
#include <functional>
#include <boost/asio.hpp>

namespace utum {

class a3s
{
public:
	typedef enum {
		NORMAL,
		LOGIN,
		BROADCAST_RESPONSE
	} request_type;
	using on_login_func = std::function<void (bool)>;
	using on_data_func = std::function<void (char const*, std::size_t)>;
	static const std::size_t MSG_MAX_LENGTH = 1024;
	static const std::size_t PAYLOAD_MAX_LENGTH = 256;

	a3s(boost::asio::io_service& io_service, uint16_t port, char const* addr,
	    on_data_func on_data=nullptr, on_login_func on_login=nullptr);
	~a3s();

	void login(char const* password);
	void onData(on_data_func fnc) {on_data_ = fnc;}
	void onLogin(on_login_func fnc) {on_login_ = fnc;}
	void send(char const* data, std::size_t length=0);
	bool listening() const {return listening_;}
	bool logedIn() const {return loged_in_;}

private:
#pragma pack(push, 1)
	struct reqheader_t {
		char begin_header[2];
		uint32_t checksum;
		uint8_t end_header;
		uint8_t begin_msg;
		reqheader_t()
			:begin_header{'B','E'}
			,checksum{0}
			,end_header{0xff}
			,begin_msg{0} {}
	};
	struct request_t {
		reqheader_t hdr;
		char payload[PAYLOAD_MAX_LENGTH];
		request_t(): hdr() {}
	};
#pragma pack(pop)
	void receive();
	void send_request(char const* data, std::size_t length,
	                  request_type rtp=NORMAL);
	bool loged_in_;
	bool listening_;
	uint8_t sequence_;
	on_data_func on_data_;
	on_login_func on_login_;
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint endpoint_;
	char data_[MSG_MAX_LENGTH];
};

} // namespace utum

#endif // A3S_H_
