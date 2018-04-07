
### grpc and protobuf

Currently all is compiled for windows x64; Compiled in release mode (as dynamic lib)

Libs in `Source\ThirdParty\Libraries`

### Know resolved issues

 - Protobuf and UE4 have macros with the same name (#defined check), so after include header file, you shuould fix it in `Source\ThirdParty\Includes\google\protobuf\stubs\type_traits.h` (see https://answers.unrealengine.com/questions/241746/error-c3861-identifier-not-found-when-including-pr.html)
 
#### modification if file UserGet.pb.cc

 - add #pragma warning( disable : 4125) in 
 - Some memory delete... do not know
```c
UserGetRequest::~UserGetRequest() {
  // @@protoc_insertion_point(destructor:UserGetRequest)
//  SharedDtor();
}
```

grpc microservice is https://github.com/biqasoft/auth-microservie

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -BUILD_DLL=ON -DCMAKE_INSTALL_PREFIX=../../../../install ../..

Before run - move dll files (libprotobuf.dll, zlib.dll) from ThirdParty\Libraries to Binaries\Win64

// based on https://www.youtube.com/watch?v=JCjSyzXv9CQ