#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "definitions.h"

bool login_user(const char* username, const char* password);
void logout_user();
User* get_current_user_data();

#endif // SESSION_MANAGER_H