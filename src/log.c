#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

const char *g_log_tag_info = "[ INF ]";
const char *g_log_tag_debug = "[ DBG ]";
const char *g_log_tag_warning = "[ WRN ]";
const char *g_log_tag_error = "[ ERR ]";

static bool
log_common(
	enum log_priority priority,
	const char* format,
	va_list variadic_arguments);

static bool
log_write_to_file(
	enum log_priority priority,
	const char *format_final,
	char log_message[LOG_MESSAGE_MAX_SIZE]);

static bool
log_write_to_stdout(
	enum log_priority priority,
	const char *format_final,
	char log_message[LOG_MESSAGE_MAX_SIZE]);

/*
 * https://en.wikipedia.org/wiki/C23_(C_standard_revision)#Syntax
 * > `va_start` macro no longer needs a second argument nor does it evaluate
 *   any argument after the first one if present
 */

bool
log_info(const char* format_string, ...)
{
	bool performance_success;
	va_list variadic_arguments;

	performance_success = false;

	va_start(variadic_arguments);
	performance_success = log_common(
		LOG_PRIORITY_INFO,
		format_string,
		variadic_arguments);
	va_end(variadic_arguments);

	return performance_success;
}

bool
log_debug(const char* format_string, ...)
{
	bool performance_success;
	va_list variadic_arguments;

	performance_success = false;

	va_start(variadic_arguments);
	performance_success = log_common(
		LOG_PRIORITY_DEBUG,
		format_string,
		variadic_arguments);
	va_end(variadic_arguments);

	return performance_success;
}

bool
log_warning(const char* format_string, ...)
{
	bool performance_success;
	va_list variadic_arguments;

	performance_success = false;

	va_start(variadic_arguments);
	performance_success = log_common(
		LOG_PRIORITY_WARNING,
		format_string,
		variadic_arguments);
	va_end(variadic_arguments);

	return performance_success;
}

bool
log_error(const char* format_string, ...)
{
	bool performance_success;
	va_list variadic_arguments;

	performance_success = false;

	va_start(variadic_arguments);
	performance_success = log_common(
		LOG_PRIORITY_ERROR,
		format_string,
		variadic_arguments);
	va_end(variadic_arguments);

	return performance_success;
}

bool
log_initialize(void)
{
#if ENABLE_LOG_FILE_TRUNCATION
	FILE *handle_log;
	int return_value;

	return_value = -1;
	handle_log = 0;

	handle_log = fopen(LOG_FILE_NAME, "w");
	if (NULL == handle_log) {
		fprintf(stderr,
			"%s: Failed to get handle for: %s\n",
			__func__,
			LOG_FILE_NAME);
		goto fail;
	}

	return_value = fclose(handle_log);
	if (0 != return_value) {
		fprintf(stderr,
			"%s: Failed to close handle of: %s\n",
			__func__,
			LOG_FILE_NAME);
		goto fail;
	}
#endif // ENABLE_LOG_FILE_TRUNCATION

	return true;

fail:
	return false;
}

const char*
priority_to_priority_tag(enum log_priority priority)
{
	if (LOG_PRIORITY_INFO == priority) {
		return g_log_tag_info;
	} else if (LOG_PRIORITY_DEBUG == priority) {
		return g_log_tag_debug;
	} else if (LOG_PRIORITY_WARNING == priority) {
		return g_log_tag_warning;
	} else if (LOG_PRIORITY_ERROR == priority) {
		return g_log_tag_error;
	} else {
		fprintf(stderr, "Unknown priority: %d\n", priority);
	}

	return NULL;
}

#if ENABLE_LOG_WRITE
static bool
log_write_to_file(
	enum log_priority priority,
	const char *format_final,
	char log_message[LOG_MESSAGE_MAX_SIZE])
{
	int return_value;
	FILE *handle_log;

	return_value = -1;
	handle_log = NULL;

	// TODO consider opening the log file once during program lifespan
	handle_log = fopen(LOG_FILE_NAME, "a");
	if (NULL == handle_log) {
		perror("Failed to get handle for " LOG_FILE_NAME);
		goto fail;
	}

	return_value = fprintf(
		handle_log,
		format_final,
		priority_to_priority_tag(priority),
		log_message);
	if (return_value < 0) {
		perror("Failed to write to " LOG_FILE_NAME);
		goto fail;
	}

	return_value = fclose(handle_log);
	if (0 != return_value) {
		perror("Failed to close handle of " LOG_FILE_NAME);
		goto fail;
	}
	handle_log = NULL;

	return true;

fail:
	if (NULL != handle_log) {
		return_value = fclose(handle_log);
		if (0 != return_value) {
			fprintf(stderr,
				"Failed to close handle of file: %s\n",
				LOG_FILE_NAME);
		}
		handle_log = NULL;
	}

	return false;
}
#endif // ENABLE_LOG_WRITE

#if ENABLE_PRINT
static bool
log_write_to_stdout(
	enum log_priority priority,
	const char *format_final,
	char log_message[LOG_MESSAGE_MAX_SIZE])
{
	printf(
		format_final,
		priority_to_priority_tag(priority),
		log_message);
}
#endif // ENABLE_PRINT

#define ERROR_LOG_COMMON_VSNPRINTF "Failed to write resulting log to a character array.\n"
/*
 * Each log message should be on its own line, so always append a line break to
 * received message to log.
 */
static bool
log_common(
	const enum log_priority priority,
	const char* format,
	va_list variadic_arguments)
{
	const char *format_final = "%s %s\n";

	int return_value;
	char log_message[LOG_MESSAGE_MAX_SIZE];

	for (int i = 0; i < LOG_MESSAGE_MAX_SIZE; i++) {
		log_message[i] = '\0';
	}
	return_value = -1;

	return_value = vsnprintf(
		log_message,
		LOG_MESSAGE_MAX_SIZE,
		format,
		variadic_arguments);
	if (return_value < 0) {
		fprintf(stderr, ERROR_LOG_COMMON_VSNPRINTF);
		goto fail;
	}

#if ENABLE_PRINT
	log_write_to_stdout(priority, format_final, log_message);
#endif // ENABLE_PRINT

#if ENABLE_LOG_WRITE
	log_write_to_file(priority, format_final, log_message);
#endif // ENABLE_LOG_WRITE

	return true;

fail:
	return false;
}
