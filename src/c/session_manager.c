#include "session_manager.h"

User currentUser;
bool isLoggedIn = false;

bool login_user(const char* username, const char* password) {
    printf("DEBUG: Mencoba login dengan username: %s\n", username);
    printf("DEBUG: Jumlah user dalam sistem: %d\n", global_jumlah_users);
    
    for (int i = 0; i < global_jumlah_users; i++) {
        printf("DEBUG: Checking user %d: %s\n", i, global_users[i].username);
        if (strcmp(global_users[i].username, username) == 0 &&
            strcmp(global_users[i].password, password) == 0) {
            currentUser = global_users[i];
            isLoggedIn = true;
            return true;
        }
    }
    return false;
}

void logout_user() {
    isLoggedIn = false;
}

User* get_current_user_data() {
    if (isLoggedIn) {
        for (int i = 0; i < global_jumlah_users; i++) {
            if (global_users[i].id_user == currentUser.id_user) {
                return &global_users[i];
            }
        }
    }
    return NULL;
}