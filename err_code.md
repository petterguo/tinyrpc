# Error_code 说明文档
err_code 详细说明如下表：


|  **错误码** | **错误代码** | **错误码描述** |
|  ----  | ----  | ---- |
| ERROR_PEER_CLOSED | 10000000 | connect 时对端关闭，一般是对端没有进程在监听此端口 |
| ERROR_FAILED_CONNECT | 10000001 | connect 失败|
| ERROR_FAILED_GET_REPLY | 10000002 | RPC 调用未收到对端回包数据 |
| ERROR_FAILED_DESERIALIZE | 10000003 | 反序列化失败，这种情况一般是 TinyPb 里面的 pb_data 有问题 |
| ERROR_FAILED_SERIALIZE | 10000004 | 序列化失败|
| ERROR_FAILED_ENCODE | 10000005 | 编码失败 |
| ERROR_FAILED_DECODE | 10000006 |  解码失败|
| ERROR_RPC_CALL_TIMEOUT | 10000007 | 调用 RPC 超时 |
| ERROR_SERVICE_NOT_FOUND | 10000008 | Service 不存在，即对方没有注册这个 Service |
| ERROR_METHOD_NOT_FOUND | 10000009 | Method 不存在，对方没有这个 方法|
| ERROR_PARSE_SERVICE_NAME | 10000010 | 解析 service_name 失败|