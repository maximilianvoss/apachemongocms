#ifndef __USER_REGISTRATION_H__
#define __USER_REGISTRATION_H__

#include "../includes.h"

#define USER_REGISTRATION_DEBUG

int registration_user(request_rec *request);
int registration_removeUser(request_rec *request);

#endif
