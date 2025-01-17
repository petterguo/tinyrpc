## This is an example that how to make a simple rpc testcase with "tinyrpc"

#### 1. Define protobuf file

A simple **protobuf** file like this:

```c++
// testcases/tinypb.proto
syntax = "proto3";
option cc_generic_services = true;

message queryAgeReq {
  int32 req_no = 1;
  int32 id = 2;
}

message queryAgeRes {
  int32 ret_code = 1;
  string res_info = 2;
  int32 req_no = 3;
  int32 id = 4;
  int32 age = 5;
}

message queryNameReq {
  int32 req_no = 1;
  int32 id = 2;
  int32 type = 3;
}

message queryNameRes {
  int32 ret_code = 1;
  string res_info = 2;
  int32 req_no = 3;
  int32 id = 4;
  string name = 5;
}


service QueryService {
  // rpc method name
  rpc query_name(queryNameReq) returns (queryNameRes);

  // rpc method name
  rpc query_age(queryAgeReq) returns (queryAgeRes);
}
```

#### 2. Use protoc to auto-gen code
```
protoc --cpp_out=./ tinypb.proto
```
And you will get two files:

**tinypb.pb.cc**   **tinypb.pb.h**

It contains main class, for example: **queryAgeReq、QueryService、QueryService_Stub**


#### 3. Implementation abstract interface "QueryService"
You should inherit class "**QueryService**" and write business code to finish method "**query_name**" and "**query_age**", because these methods is vritual function in **QueryService**.

```c++
class QueryServiceImpl : public QueryService {
 public:
  QueryServiceImpl() {}
  ~QueryServiceImpl() {}

  void query_name(google::protobuf::RpcController* controller,
                       const ::queryNameReq* request,
                       ::queryNameRes* response,
                       ::google::protobuf::Closure* done) {
    
    DebugLog << "========================";
    DebugLog << "this is query_name func";
    response->set_ret_code(0);
    response->set_res_info("OK");
    response->set_req_no(request->req_no());
    response->set_id(request->id());
    response->set_name("ikerli");
    
    DebugLog << "========================";
    done->Run();
  }
  void query_age(google::protobuf::RpcController* controller,
                       const ::queryAgeReq* request,
                       ::queryAgeRes* response,
                       ::google::protobuf::Closure* done) {

    DebugLog << "========================";
    DebugLog << "this is query_age func";
    response->set_ret_code(0);
    response->set_res_info("OK");
    response->set_req_no(request->req_no());
    response->set_id(request->id());
    response->set_age(20);
    DebugLog << "========================";
    done->Run();
  }

};
```

#### 4. Set Up RpcServer1
It's very convenient to set uo RpcServer with **tinyrpc** framework

```c++

// testcases/test_rpc_server1.cc
int main(int argc, char* argv[]) {
  // set ip and port
  tinyrpc::IPAddress::ptr addr = std::make_shared<tinyrpc::IPAddress>("127.0.0.1", 39999);
  
  tinyrpc::TcpServer server(addr);
  // you should register service to RpcDispatcher first
  tinyrpc::TinyPbRpcDispacther* dispatcher = server.getDispatcher();
  QueryService* service = new QueryServiceImpl();
  
  DebugLog << "================";

  // register Service Instanse on RpcServer
  // it make RpcServer can find the service and method which client call
  dispatcher->registerService(service);

  // now start server
  server.start();
  return 0;
}
```

#### 5.Set Up RpcServer2 
Like RpcServer1, but we don't register Service, because we just user RpcServer2 to call method of RpcServer1.
```c++
// testcases/test_rpc_server2.cc
void fun() {
  // peer addr is RpcServer1's addr
  tinyrpc::IPAddress::ptr peer_addr = std::make_shared<tinyrpc::IPAddress>("127.0.0.1", 39999);

  // init RpcChannel
  tinyrpc::TinyPbRpcChannel channel(peer_addr);
  // init RpcController
  tinyrpc::TinyPbRpcController rpc_controller;
  DebugLog << "input an integer to set count that send tinypb data";
  int n;
  std::cin >> n;

  while (n--) {

  queryNameReq req_name;
  req_name.set_req_no(20220315);
  req_name.set_id(1234);
  req_name.set_type(1);

  queryNameRes res_name;

  queryAgeReq req_age;
  req_age.set_req_no(00001111);
  req_age.set_id(6781);

  queryAgeRes res_age;

  tinyrpc::TinyPbRpcClosure cb([]() {
    DebugLog << "==========================";
    DebugLog << "succ call rpc";
    DebugLog << "==========================";
  });

  QueryService_Stub stub(&channel);
  // directly call rpc method
  stub.query_name(&rpc_controller, &req_name, &res_name, &cb);
  stub.query_age(&rpc_controller, &req_age, &res_age, &cb);

  DebugLog << "get res_name.age = " << res_name.name();
  DebugLog << "get res_age.age = " << res_age.age();

  }

  DebugLog << "================";

}

int main(int argc, char* argv[]) {

  tinyrpc::IPAddress::ptr self_addr = std::make_shared<tinyrpc::IPAddress>("127.0.0.1", 20000);
  tinyrpc::TcpServer server(self_addr, 1);
  tinyrpc::Coroutine::ptr cor = tinyrpc::GetCoroutinePool()->getCoroutineInstanse();
  cor->setCallBack(&fun);
  server.addCoroutine(cor);

  server.start();

  return 0;
}
```

#### 6. Test Rpc
Directly run RpcServer1 and RpcServer2, notice if you recive correct result;
