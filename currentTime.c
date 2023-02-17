#include <include/currentTime.h>

#include <time.h>

time_t CurrentTime_getCurrentTime(void)
{
    time_t currentTime = time(NULL);
    return currentTime;
}

int CurrentTime_getCurrentHour(void)
{
    time_t currentTime = time(NULL);
    struct tm *tm_struct = localtime(&currentTime);
    return tm_struct->tm_hour;
}

int CurrentTime_getCurrentMinute(void)
{
    time_t currentTime = time(NULL);
    struct tm *tm_struct = localtime(&currentTime);
    return tm_struct->tm_min;
}

int CurrentTime_getCurrentSecond(void)
{
    time_t currentTime = time(NULL);
    struct tm *tm_struct = localtime(&currentTime);
    return tm_struct->tm_sec;
}
