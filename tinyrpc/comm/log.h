#ifndef TINYRPC_LOG_LOG_H
#define TINYRPC_LOG_LOG_H

#include <sstream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include <queue>
#include "../net/mutex.h"
#include "config.h"


extern tinyrpc::Config::ptr gRpcConfig;

namespace tinyrpc {

enum LogType {
	RPC_LOG = 1,
	APP_LOG = 2,
};

#define DebugLog \
	if (tinyrpc::LogLevel::DEBUG >= gRpcConfig->m_log_level) \
		tinyrpc::LogTmp(tinyrpc::LogEvent::ptr(new tinyrpc::LogEvent(tinyrpc::LogLevel::DEBUG, __FILE__, __LINE__, __func__))).getStringStream()

#define InfoLog \
	if (tinyrpc::LogLevel::INFO >= gRpcConfig->m_log_level) \
		tinyrpc::LogTmp(tinyrpc::LogEvent::ptr(new tinyrpc::LogEvent(tinyrpc::LogLevel::INFO, __FILE__, __LINE__, __func__))).getStringStream()

#define WarnLog \
	if (tinyrpc::LogLevel::WARN >= gRpcConfig->m_log_level) \
		tinyrpc::LogTmp(tinyrpc::LogEvent::ptr(new tinyrpc::LogEvent(tinyrpc::LogLevel::WARN, __FILE__, __LINE__, __func__))).getStringStream()

#define ErrorLog \
	if (tinyrpc::LogLevel::ERROR >= gRpcConfig->m_log_level) \
		tinyrpc::LogTmp(tinyrpc::LogEvent::ptr(new tinyrpc::LogEvent(tinyrpc::LogLevel::ERROR, __FILE__, __LINE__, __func__))).getStringStream()


pid_t gettid();

LogLevel stringToLevel(const std::string& str);

class LogEvent {

 public:
 	
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent(LogLevel level, const char* file_name, int line, const char* func_name);

	~LogEvent();

	std::stringstream& getStringStream();

	void log();


 private:
		
	const char* m_msg_no;
	// uint64_t m_timestamp;
	timeval m_timeval;
	LogLevel m_level;
	pid_t m_pid {0};
	pid_t m_tid {0};
	int m_cor_id {0};

	const char* m_file_name;
	int m_line {0};
	const char* m_func_name;

	std::stringstream m_ss;

};


class LogTmp {
 
 public:
	explicit LogTmp(LogEvent::ptr event);

	~LogTmp();

	std::stringstream& getStringStream();

 private:
	LogEvent::ptr m_event;

};

class AsyncLogger {
 public:
  typedef std::shared_ptr<AsyncLogger> ptr;

	AsyncLogger(const char* file_name, LogType logtype);
	~AsyncLogger();

	void push(std::vector<std::string>& buffer);

	void flush();

	static void* excute(void*);

	void stop();

 public:
	std::queue<std::vector<std::string>> m_tasks;

 private:
	const char* m_file_name;
	// int m_max_size {0};
	LogType m_log_type;
	int m_no {0};
	int m_fd {-1};
	bool m_need_reopen {false};
	FILE* m_file_handle {nullptr};
	std::string m_date;

 	Mutex m_mutex;
  pthread_cond_t m_condition;
	bool m_stop {false};

 public:
  pthread_t m_thread;

};

class Logger {
 public:
  typedef std::shared_ptr<Logger> ptr;

	Logger();
	~Logger();

	void init(const char* file_name, LogType type = RPC_LOG);
	void log();
	void push(const std::string& log_msg);
	void loopFunc();

	void flush();

	AsyncLogger::ptr getAsyncLogger() {
		return m_async_logger;
	}

 public:
	std::vector<std::string> m_buffer;

 private:
 	Mutex m_mutex;
	bool m_is_init {false};
	AsyncLogger::ptr m_async_logger;

};


// do something before exit progress, like flush log
// it will be called when progress get something error
void Exit(int code);

}


#endif
