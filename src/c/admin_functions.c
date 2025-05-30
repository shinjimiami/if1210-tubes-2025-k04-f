#include "admin_functions.h"
#include "user_interface.h"
#include "data_manager.h"
#include "session_manager.h" 
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Implementasi fungsi helper untuk validasi username unik (case-insensitive)
bool is_username_unique_case_insensitive(const char* username_baru) {
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

// Implementasi F10 - Tambah Dokter
void handle_tambah_dokter() {
    if (global_jumlah_users >= MAX_USERS) {
        display_error("Kapasitas pengguna (termasuk dokter) sudah penuh.");
        return;
    }

    char username_baru[MAX_USERNAME_LEN];
    char password_baru[MAX_PASSWORD_LEN];
    char nama_user_baru[MAX_NAMA_USER_LEN];


    display_message("--- Tambah Dokter Baru ---");
    get_user_input("Username Dokter: ", username_baru, MAX_USERNAME_LEN);

    if (!is_username_unique_case_insensitive(username_baru)) {
        printf("Sudah ada Pengguna (atau Dokter) dengan username %s!\n", username_baru);
        return;
    }
    
    strcpy(nama_user_baru, username_baru); 

    get_user_input("Password Dokter: ", password_baru, MAX_PASSWORD_LEN);

    User* new_dokter = &global_users[global_jumlah_users];
    new_dokter->id_user = g_next_user_id++; 
    strcpy(new_dokter->nama_user, nama_user_baru); 
    strcpy(new_dokter->username, username_baru);
    strcpy(new_dokter->password, password_baru);
    strcpy(new_dokter->role, "dokter");
    strcpy(new_dokter->penyakit_utama, ""); 
    
    new_dokter->suhu_tubuh = 0.0f;
    new_dokter->tekanan_darah_sistolik = 0;
    new_dokter->tekanan_darah_diastolik = 0;
    new_dokter->detak_jantung = 0;
    new_dokter->saturasi_oksigen = 0.0f;
    new_dokter->kadar_gula_darah = 0;
    new_dokter->berat_badan = 0.0f;
    new_dokter->tinggi_badan = 0;
    new_dokter->kadar_kolesterol = 0;
    new_dokter->trombosit = 0;

    global_jumlah_users++;

    printf("Dokter %s berhasil ditambahkan!\n", username_baru); 
}

// Implementasi F10 - Assign Dokter ke Ruangan
void handle_assign_dokter() {
    char username_dokter[MAX_USERNAME_LEN];
    char id_ruangan_str[10];
    int id_ruangan_input;
    User* target_dokter = NULL;
    int ruangan_idx = -1;

    display_message("--- Assign Dokter ke Ruangan ---");
    get_user_input("Username Dokter yang akan diassign: ", username_dokter, MAX_USERNAME_LEN);
    get_user_input("Nomor Ruangan Tujuan (misal 1, 2, dst.): ", id_ruangan_str, 10);
    id_ruangan_input = atoi(id_ruangan_str);

    if (id_ruangan_input <= 0 || id_ruangan_input > jumlah_ruangan_aktual_sti) {
        printf("Nomor ruangan tidak valid. Ruangan tersedia: 1-%d.\n", jumlah_ruangan_aktual_sti);
        return;
    }
    ruangan_idx = id_ruangan_input - 1; 

    for (int i = 0; i < global_jumlah_users; i++) {
        if (strcmp(global_users[i].username, username_dokter) == 0 && strcmp(global_users[i].role, "dokter") == 0) {
            target_dokter = &global_users[i];
            break;
        }
    }

    if (target_dokter == NULL) {
        printf("Dokter dengan username '%s' tidak ditemukan.\n", username_dokter);
        return;
    }

    int current_room_of_doctor = -1;
    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) {
        if (denah_rs_sti[i].id_dokter_bertugas == target_dokter->id_user) {
            current_room_of_doctor = denah_rs_sti[i].id_ruangan;
            break;
        }
    }

    bool ruangan_tujuan_kosong = (denah_rs_sti[ruangan_idx].id_dokter_bertugas == ID_DOKTER_KOSONG);
    int id_dokter_di_ruangan_tujuan = denah_rs_sti[ruangan_idx].id_dokter_bertugas;
    char nama_dokter_di_ruangan_tujuan[MAX_NAMA_USER_LEN] = "";

    if (!ruangan_tujuan_kosong) {
        for(int i=0; i < global_jumlah_users; ++i) {
            if(global_users[i].id_user == id_dokter_di_ruangan_tujuan) {
                strcpy(nama_dokter_di_ruangan_tujuan, global_users[i].nama_user);
                break;
            }
        }
    }

    if (current_room_of_doctor != -1) { 
        if (ruangan_tujuan_kosong) {
            printf("Dokter %s sudah diassign ke ruangan %d!\n", target_dokter->username, current_room_of_doctor);
        } else { 
            printf("Dokter %s sudah menempati ruangan %d!\n", target_dokter->username, current_room_of_doctor);
            if (id_dokter_di_ruangan_tujuan != target_dokter->id_user) { 
                 printf("Ruangan %d juga sudah ditempati oleh Dokter %s!\n", id_ruangan_input, nama_dokter_di_ruangan_tujuan);
            }
        }
    } else { 
        if (ruangan_tujuan_kosong) {
            denah_rs_sti[ruangan_idx].id_dokter_bertugas = target_dokter->id_user;
            printf("Dokter %s berhasil diassign ke ruangan %d!\n", target_dokter->username, id_ruangan_input);
        } else {
            printf("Dokter %s sudah menempati ruangan %d!\n", nama_dokter_di_ruangan_tujuan, id_ruangan_input);
            printf("Silakan cari ruangan lain untuk Dokter %s.\n", target_dokter->username);
        }
    }
}
