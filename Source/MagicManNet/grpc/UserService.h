// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "grpc/UsersGet.grpc.pb.h"

/**
 * 
 */
class UserService 
{
	
	
public:
	UserService();

	void print();

	FString getUsernameById(FString id);

private:
	std::unique_ptr< Users::Stub> stub;
};
