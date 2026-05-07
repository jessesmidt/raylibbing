#include "engine.h"

int main(void)
{
    init();
    while (!should_close())
        update();
    shutdown();
    return 0;
}
