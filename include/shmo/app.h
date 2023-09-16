//
// Created by scott on 9/15/2023.
//

#ifndef SHMOCODEBASE_APP_H
#define SHMOCODEBASE_APP_H

#include "base.h"

bool app_open(void);
void app_poll_events(void);
bool app_should_close(void);
void app_close(void);

#endif //SHMOCODEBASE_APP_H
