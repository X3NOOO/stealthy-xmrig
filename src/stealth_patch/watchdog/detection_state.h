#ifndef WATCHDOG_DETECTION_STATE_H
#define WATCHDOG_DETECTION_STATE_H

typedef enum detection_state : unsigned char
{
    RUN,
    PAUSE,
    EXIT,
    ERROR
} detection_state;

#endif // WATCHDOG_DETECTION_STATE_H