#include "user_interface.h"

void get_user_input(const char* prompt, char* buffer, int buffer_size) {
    printf("%s", prompt);
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

void display_message(const char* message) {
    printf("%s\n", message);
}

void display_error(const char* error_message) {
    printf("ERROR: %s\n", error_message);
}

void display_main_prompt() {
    if (isLoggedIn) {
        printf("[%s@%s] > ", currentUser.username, currentUser.role);
    } else {
        printf("[guest] > ");
    }
}