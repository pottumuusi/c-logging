#include <stdio.h>

#include "log.h"

int main(void)
{
    log_info("testing info: %f", 0.5);
    log_debug("testing debug: %d", 80);
    log_warning("testing warning: %08x", 0x55);
    log_error("testing error: %s", "123");

    return 0;
}
