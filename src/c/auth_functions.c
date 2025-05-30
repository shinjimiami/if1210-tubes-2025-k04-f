#include "auth_functions.h"
#include "session_manager.h"
#include "user_interface.h"
#include "utils.h"
#include "data_manager.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

void handle_login() {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    get_user_input("Username: ", username, MAX_USERNAME_LEN);
    get_user_input("Password: ", password, MAX_PASSWORD_LEN);
    if (login_user(username, password)) {
        display_message("Login berhasil!");
        if (strcmp(currentUser.role, "manager") == 0) {
            printf("Halo Manager %s!\n", currentUser.nama_user);
        } else if (strcmp(currentUser.role, "dokter") == 0) {
            printf("Halo Dokter %s!\n", currentUser.nama_user);
        } else if (strcmp(currentUser.role, "pasien") == 0) {
            printf("Halo Pasien %s! Ada keluhan apa ?\n", currentUser.nama_user);
        } else {
            printf("Selamat datang, %s!\n", currentUser.nama_user);
        }
    } else {
        bool user_exists = false;
        for(int i=0; i < global_jumlah_users; ++i) {
            if(strcmp(global_users[i].username, username) == 0) {
                user_exists = true;
                break;
            }
        }
        if (!user_exists) {
             printf("Tidak ada Manager, Dokter, atau pun Pasien yang bernama %s!\n", username);
        } else {
             printf("Username atau password salah untuk pengguna yang bernama %s!\n", username);
        }
    }
}

void handle_logout() {
    logout_user();
    display_message("Sampai jumpa!");
}

bool is_username_unique_for_registration(const char* username_baru) {
    char uname_baru_lower[MAX_USERNAME_LEN];
    char uname_db_lower[MAX_USERNAME_LEN];

    int len_baru = strlen(username_baru);
    for (int i = 0; i < len_baru; i++) {
        uname_baru_lower[i] = tolower((unsigned char)username_baru[i]);
    }
    uname_baru_lower[len_baru] = '\0';

    for (int i = 0; i < global_jumlah_users; i++) {
        int len_db = strlen(global_users[i].username);
        for (int j = 0; j < len_db; j++) {
            uname_db_lower[j] = tolower((unsigned char)global_users[i].username[j]);
        }
        uname_db_lower[len_db] = '\0';

        if (strcmp(uname_db_lower, uname_baru_lower) == 0) {
            return false; 
        }
    }
    return true; 
}

void add_new_user_to_db(const char* username, const char* password) { 
    User* newUser = &global_users[global_jumlah_users];
    newUser->id_user = g_next_user_id++; 
    strcpy(newUser->nama_user, username);
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    strcpy(newUser->role, "pasien");
    strcpy(newUser->penyakit_utama, ""); 
    newUser->suhu_tubuh = 0.0f;
    newUser->tekanan_darah_sistolik = 0;
    newUser->tekanan_darah_diastolik = 0;
    newUser->detak_jantung = 0;
    newUser->saturasi_oksigen = 0.0f;
    newUser->kadar_gula_darah = 0;
    newUser->berat_badan = 0.0f;
    newUser->tinggi_badan = 0;
    newUser->kadar_kolesterol = 0;
    newUser->trombosit = 0;
    
    global_jumlah_users++;
}

void handle_register_user() { 
    if (global_jumlah_users >= MAX_USERS) {
        display_error("Kapasitas pengguna sudah penuh, registrasi tidak dapat dilakukan.");
        return;
    }

    char username_baru[MAX_USERNAME_LEN];
    char password_baru[MAX_PASSWORD_LEN];

    display_message("--- Registrasi Pasien Baru ---");
    get_user_input("Username: ", username_baru, MAX_USERNAME_LEN);

    if (!is_username_unique_for_registration(username_baru)) {
        // Sesuai contoh F02 Kasus 2
        printf("Registrasi gagal! Pasien dengan nama %s sudah terdaftar.\n", username_baru);
        return;
    }

    get_user_input("Password: ", password_baru, MAX_PASSWORD_LEN);
    
    // Tambahkan pasien baru ke database
    add_new_user_to_db(username_baru, password_baru);
    printf("Pasien %s berhasil ditambahkan!\n", username_baru);
}

// F04 - Lupa Password
void handle_lupa_password() { 
    char username_input[MAX_USERNAME_LEN];
    char kode_unik_input[MAX_RLE_CODE_LEN];
    char rle_username_actual[MAX_RLE_CODE_LEN];
    char password_baru[MAX_PASSWORD_LEN];
    User* target_user = NULL;
    int user_idx = -1;

    display_message("--- Lupa Password ---");
    get_user_input("Username: ", username_input, MAX_USERNAME_LEN);
    
    // Cari user berdasarkan username
    for (int i = 0; i < global_jumlah_users; i++) {
        if (strcmp(global_users[i].username, username_input) == 0) {
            target_user = &global_users[i];
            user_idx = i;
            break;
        }
    }

    if (target_user == NULL) {
        display_error("Username tidak terdaftar!");
        return;
    }

    get_user_input("Kode Unik: ", kode_unik_input, MAX_RLE_CODE_LEN);

    if (!run_length_encode(target_user->username, rle_username_actual, MAX_RLE_CODE_LEN)) {
        display_error("Error internal saat menghasilkan kode unik dari username tersimpan.");
        return;
    }

    if (strcmp(rle_username_actual, kode_unik_input) != 0) {
        display_error("Kode unik salah!");
        return;
    }

    // Sesuai F04 Kasus 1
    if (strcmp(target_user->role, "manager") == 0) {
        printf("Halo Manager %s, silakan daftarkan ulang password anda!\n", target_user->nama_user);
    } else if (strcmp(target_user->role, "dokter") == 0) {
        printf("Halo Dokter %s, silakan daftarkan ulang password anda!\n", target_user->nama_user);
    } else if (strcmp(target_user->role, "pasien") == 0) {
        printf("Halo Pasien %s, silakan daftarkan ulang password anda!\n", target_user->nama_user);
    } else {
         printf("Halo %s, silakan daftarkan ulang password anda!\n", target_user->nama_user);
    }

    get_user_input("Password Baru: ", password_baru, MAX_PASSWORD_LEN);
    strcpy(global_users[user_idx].password, password_baru);
    display_message("Password berhasil diperbarui.");
}
