#if defined _WIN32 || defined _WIN64 
    #include <Windows.h>
#elif defined __linux__
    #include <unistd>
#endif