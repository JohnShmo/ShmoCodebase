//
// Created by scott on 9/15/2023.
//

#ifndef SHMOCODEBASE_APP_H
#define SHMOCODEBASE_APP_H

#include "base.h"

#define APP_POS_CENTERED (0x2FFF0000u | (0))

bool app_open(const char *title, v2i_t position, v2i_t size);
void app_poll_events(void);
bool app_should_close(void);
void app_close(void);

#endif //SHMOCODEBASE_APP_H
