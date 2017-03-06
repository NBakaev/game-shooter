// Fill out your copyright notice in the Description page of Project Settings.

#include "MagicManNet.h"
#include "UserService.h"

#include "User.pb.h"

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include "UsersGet.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using namespace protobuf_User_2eproto;
using namespace std;

UserService::UserService()
{
	auto channel = grpc::CreateChannel("localhost:25001", grpc::InsecureChannelCredentials());
	this->stub = Users::NewStub(channel);
}

void UserService::print()
{
	
}

FString UserService::getUsernameById(FString id)
{
	try
	{
		ClientContext context;
		UserGetRequest request;
		UserGetResponse response;

		request.set_id(TCHAR_TO_UTF8(*id));
		auto user_by_id = stub->GetUserById(&context, request, &response);
		if (user_by_id.ok() && response.valid())
		{
			return FString(response.user().username().c_str());
		}
	}
	catch (...)
	{
		UE_LOG(LogClass, Log, TEXT("WasCollected_Implementation %s"), "Error request");
	}
	return FString("null");
}
