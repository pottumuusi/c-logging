#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

static bool
log_common(
	const char* priority_tag,
	const char* format,
	va_list variadic_arguments);

static bool
log_write_to_file(
	const char *format_log,
	const char* priority_tag,
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
		LOG_TAG_INFO,
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
		LOG_TAG_DEBUG,
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
		LOG_TAG_WARNING,
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
		LOG_TAG_ERROR,
		format_string,
		variadic_arguments);
	va_end(variadic_arguments);

	return performance_success;
}

#if ENABLE_LOG_WRITE
static bool
log_write_to_file(
	const char *format_log,
	const char* priority_tag,
	char log_message[LOG_MESSAGE_MAX_SIZE])
{
	int return_value;
	FILE *handle_log;

	return_value = -1;
	handle_log = NULL;

	// TODO consider opening the log file once during program lifespan
	handle_log = fopen(LOG_FILE_NAME, "w");
	if (NULL == handle_log) {
		perror("Failed to get handle for " LOG_FILE_NAME);
		goto fail;
	}

	return_value = fprintf(handle_log, format_log, priority_tag, log_message);
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

#define ERROR_LOG_COMMON_VSNPRINTF "Failed to write resulting log to a character array.\n"
/*
 * Each log message should be on its own line, so always append a line break to
 * received message to log.
 */
static bool
log_common(
	const char* priority_tag,
	const char* format,
	va_list variadic_arguments)
{
	const char *format_log = "%s %s\n";

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
	(void) printf(format_log, priority_tag, log_message);
#endif // ENABLE_PRINT

#if ENABLE_LOG_WRITE
	log_write_to_file(format_log, priority_tag, log_message);
#endif // ENABLE_LOG_WRITE

	return true;

fail:
	return false;
}
