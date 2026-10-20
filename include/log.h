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

extern const char *g_log_tag_info;
extern const char *g_log_tag_debug;
extern const char *g_log_tag_warning;
extern const char *g_log_tag_error;

enum log_priority {
	LOG_PRIORITY_INFO,
	LOG_PRIORITY_DEBUG,
	LOG_PRIORITY_WARNING,
	LOG_PRIORITY_ERROR
};

/*
 * Library initialization and teardown.
 */

bool
log_initialize(void);

bool
log_teardown(void);

/*
 * Logging.
 */

bool
log_info(const char* format, ...);

bool
log_debug(const char* format, ...);

bool
log_warning(const char* format, ...);

bool
log_error(const char* format, ...);

#endif // LOG_H_DEFINED
