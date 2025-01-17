#ifndef TINYRPC_NET_TINYPB_TINYPB_RPC_H
#define TINYRPC_NET_TINYPB_TINYPB_RPC_H

#include <google/protobuf/service.h>
#include "../net_address.h"
#include "../tcp/tcp_client.h"

namespace tinyrpc {

class TinyPbRpcChannel : public google::protobuf::RpcChannel {

 public:
  TinyPbRpcChannel(NetAddress::ptr addr);
  ~TinyPbRpcChannel() = default;

void CallMethod(const google::protobuf::MethodDescriptor* method, 
    google::protobuf::RpcController* controller, 
    const google::protobuf::Message* request, 
    google::protobuf::Message* response, 
    google::protobuf::Closure* done);
 
 private:
  TcpClient::ptr m_client;

};

}



#endif