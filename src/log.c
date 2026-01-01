#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

static void
log_common(const char* priority_tag, const char* format, va_list variadic_arguments);

void
log_error(const char* log_error_format_string, ...)
{
    va_list variadic_arguments;

    va_start(variadic_arguments);
    log_common("[ERR ]", log_error_format_string, variadic_arguments);
    va_end(variadic_arguments);
}

// TODO configure clang-format:
// https://www.kernel.org/doc./html/latest/dev-tools/clang-format.html
//
// Each log message should be on its own line, so always append a line break to
// received message to log.
static void
log_common(const char* priority_tag, const char* format, va_list variadic_arguments)
{

    FILE *handle_log;

    int ret;
    char log_message[LOG_MESSAGE_MAX_SIZE];

    ret = 0;
    for (int i = 0; i < LOG_MESSAGE_MAX_SIZE; i++) {
        log_message[i] = '\0';
    }

    vsnprintf(log_message, LOG_MESSAGE_MAX_SIZE, format, variadic_arguments);

#if ENABLE_PRINT
    printf("%s %s\n", priority_tag, log_message);
#endif // ENABLE_PRINT

#if ENABLE_LOG_WRITE
    handle_log = fopen(LOG_FILE_NAME, "w");
    if (NULL == handle_log) {
        perror("Failed to get handle for " LOG_FILE_NAME);
        return;
    }

    ret = fprintf(handle_log, "%s %s\n", priority_tag, log_message);
    if (ret < 0) {
        perror("Failed to write to " LOG_FILE_NAME);
        return;
    }

    ret = fclose(handle_log);
    if (0 != ret) {
        perror("Failed to close handle of " LOG_FILE_NAME);
        return;
    }
    handle_log = NULL;
#endif // ENABLE_LOG_WRITE
}
