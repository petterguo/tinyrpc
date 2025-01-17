#include "http_codec.h"
#include "../../comm/log.h"
#include "../abstract_data.h"
#include "../abstract_codec.h"

namespace tinyrpc {

HttpCodeC::HttpCodeC() {

}

HttpCodeC::~HttpCodeC() {

}

void HttpCodeC::encode(TcpBuffer* buf, AbstractData* data) {
  DebugLog << "test encode";
  buf->readAble();
}

void HttpCodeC::decode(TcpBuffer* buf, AbstractData* data) {

  DebugLog << "test decode start";
  buf->readAble();


  DebugLog << "test decode end";
}

CodeCType HttpCodeC::type() const {
  return CODEC_HTTP;
}

}
