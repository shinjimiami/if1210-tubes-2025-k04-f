#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "definitions.h"

void get_user_input(const char* prompt, char* buffer, int buffer_size);
void display_message(const char* message);
void display_error(const char* error_message);
void display_main_prompt();

#endif // USER_INTERFACE_H