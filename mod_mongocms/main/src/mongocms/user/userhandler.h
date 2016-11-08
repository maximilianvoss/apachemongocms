#ifndef __USER_USERHANDLER_H__
#define __USER_USERHANDLER_H__

#include "../includes.h"

#define USER_PROFILE_FILENAME "profile.json"
#define USER_LOGIN_FILENAME "login.json"
#define USER_LOGOUT_FILENAME "logout.json"
#define USER_REGISTRATION_FILENAME "register.json"

#define USER_MONGO_PROPERTY_USERNAME "username"
#define USER_MONGO_PROPERTY_PASSWORD "password"

int user_handler(request_rec *request, char *filename);

#endif 
