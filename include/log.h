#ifndef LOG_H_DEFINED
#define LOG_H_DEFINED

#ifndef PROGRAM_NAME
#error PROGRAM_NAME not defined. Please define using compiler flags for example.
#endif // PROGRAM_NAME

#define ENABLE_PRINT 1
#define ENABLE_LOG_WRITE 1
#define ENABLE_LOG_FILE_TRUNCATION 1

#define stringize_value_of(input) stringize(input)
#define stringize(input) #input
#define LOG_FILE_NAME stringize_value_of(PROGRAM_NAME)".log"

#define LOG_MESSAGE_MAX_SIZE 256

#define LOG_TAG_INFO "[ INF ]"
#define LOG_TAG_DEBUG "[ DBG ]"
#define LOG_TAG_WARNING "[ WRN ]"
#define LOG_TAG_ERROR "[ ERR ]"

bool log_initialize(void);
bool log_teardown(void);

bool log_info(const char* format, ...);
bool log_debug(const char* format, ...);
bool log_warning(const char* format, ...);
bool log_error(const char* format, ...);

#endif // LOG_H_DEFINED
