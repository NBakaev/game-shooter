// grpcTest.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//#include <iostream>
//#include <sstream>
#include "MagicManNet.h"

#include "User.pb.h"

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

//#pragma comment(lib, "Ws2_32.lib")
//#define ZLIB_WINAPI

using namespace std;


#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
//#include "helper.h"
#include "UsersGet.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using namespace  protobuf_User_2eproto;

int main()
{

//	cout << "Hello";

	auto channel = grpc::CreateChannel("localhost:25001", grpc::InsecureChannelCredentials());
	auto stub = Users::NewStub(channel);

	ClientContext context;
	UserGetRequest request;
	UserGetResponse response;

//	request.set_id("123");
	request.set_id("57da8d66ee69784410714e7c");

	stub->GetUserById(&context, request, &response);

	cout << "\r\n";
	cout << response.valid() << "\r\n";
	cout << response.error() << "\r\n";
	cout << response.user().username() << "\r\n";

//	while (true)
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(10));
//	}

	system("pause");
	return 0;
}

//class UserClient
//{
//	
//public:
////	UserClient(std::shared_ptr<grpc::Channel> channel): stub_(UsersGet::NewStub(channel)) {}
//
//	UserClient(std::shared_ptr<Channel> channel, const std::string& db)  :_stub(Users::NewStub(channel)) {
//		routeguide::ParseDb(db, &feature_list_);
//	}
//
//};