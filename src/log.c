#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

static void
log_common(const char* priority_tag, const char* format, va_list variadic_arguments);

#if 0
int fprintf_like (void * data, const char * format, ...)
{
    int printed;
    va_list ap;
    va_start (ap, format);
    printed = vfprintf (stdout, format, ap);
    va_end (ap);
    return printed;
}
#endif

// TODO document call to va_start(). Found an alternative explanations for
// va_start arguments from the web:
// * https://docs.w3cub.com/c/variadic/va_start.html
// * https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/va-arg-va-copy-va-end-va-start?view=msvc-170
//
// "If parmN is declared with register storage class specifier, with an array
// type, with a function type, or with a type not compatible with the type that
// results from default argument promotions, the behavior is undefined. (until
// C23)"
// - https://docs.w3cub.com/c/variadic/va_start.html
//
// "va_start sets arg_ptr to the first optional argument in the list of
// arguments that's passed to the function. The argument arg_ptr must have the
// va_list type. The argument prev_param is the name of the required parameter
// that immediately precedes the first optional argument in the argument list.
// If prev_param is declared with the register storage class, the macro's
// behavior is undefined. va_start must be used before va_arg is used for the
// first time."
// - https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/va-arg-va-copy-va-end-va-start?view=msvc-170
//
// * Asked from Kagi: "What restrictions are there for 'last' parameter of
// va_start function?"
// Received:
// * "Cannot have register storage class: The last parameter must not have
//   register storage class 3."
// * "Type must not be changed by the translator: The type of the last
//   parameter must not be altered by the compiler during translation 3."
// * [3] https://stackoverflow.com/questions/1353407/last-named-parameter-not-function-or-array
//
// Conclusion, instead of documenting mysterious call to va_start(), use C23.
void
log_error(const char* log_error_format_string, ...)
{
    va_list variadic_arguments;

    va_start(variadic_arguments, log_error_format_string);
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
