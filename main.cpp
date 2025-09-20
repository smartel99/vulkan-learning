#include "app.h"

#include "log.h"

int main()
{
    Log::Init();

    App app;
    if (app.init())
    {
        app.run();
    }
    return 0;
}
