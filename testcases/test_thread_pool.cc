#include "log.h"
#include <unistd.h>
#include "net_address.h"
#include "thread_pool.h"
#include "config.h"


void* fun(void* arg) {

  DebugLog << "this is test log 1";

  return nullptr;
}

tinyrpc::Logger::ptr gRpcLogger;
tinyrpc::Config::ptr gRpcConfig;

int main(int argc, char** argv) {

  gRpcConfig = std::make_shared<tinyrpc::Config>("../testcases/tinyrpc.xml");
  gRpcConfig->readConf();

  gRpcLogger = std::make_shared<tinyrpc::Logger>();
  gRpcLogger->init("test_thread_pool");

  tinyrpc::ThreadPool pool(4);
  pool.start();

  for (int i = 1; i < 100; ++i) {
    auto cb = [i]() {
      DebugLog << "this is task[" << i << "]";
    };
    pool.addTask(cb);
  }

  DebugLog << "this is main thread, now sleep 10 s";

  sleep(2); 
  DebugLog << "sleep 10 s end, now to stop";

  pool.stop();

  return 0;
}
