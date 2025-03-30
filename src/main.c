#include <stdio.h>

#include "log.h"

int main(void)
{
    log_error("testing error: %s", "123");

    return 0;
}
