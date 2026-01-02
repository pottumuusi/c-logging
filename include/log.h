#ifndef LOG_H_DEFINED
#define LOG_H_DEFINED

#ifndef PROGRAM_NAME
#error PROGRAM_NAME not defined. Please define using compiler flags for example.
#endif // PROGRAM_NAME

#define ENABLE_PRINT 1
#define ENABLE_LOG_WRITE 1

#define stringize_value_of(input) stringize(input)
#define stringize(input) #input
#define LOG_FILE_NAME stringize_value_of(PROGRAM_NAME)".log"

#define LOG_MESSAGE_MAX_SIZE 256

int log_initialize(void);
int log_teardown(void);

void log_info(const char* format, ...);
void log_debug(const char* format, ...);
void log_warning(const char* format, ...);
void log_error(const char* format, ...);

#endif // LOG_H_DEFINED
