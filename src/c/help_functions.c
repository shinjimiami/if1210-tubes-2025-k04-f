
#include "help_functions.h"
#include "user_interface.h"   
#include "session_manager.h"  
#include "definitions.h"      
#include <stdio.h>
#include <string.h>

void display_help_footnote_indented() {
    printf("\n"); 
    printf("Footnote:\n");
    printf("  1. Untuk menggunakan aplikasi, silahkan masukkan nama fungsi yang terdaftar\n");
    printf("  2. Jangan lupa untuk memasukkan input yang valid\n");
}

void display_menu_item_indented(int number, const char* command_name, const char* description) {
    const int command_field_width = 25; 
    char padded_command[command_field_width + 1];
    int name_len = strlen(command_name);

    if (name_len >= command_field_width) {
        strncpy(padded_command, command_name, command_field_width -1); 
        padded_command[command_field_width -1] = '\0'; 
    } else {
        strcpy(padded_command, command_name);
        for (int i = name_len; i < command_field_width; i++) {
            padded_command[i] = ' ';
        }
        padded_command[command_field_width] = '\0'; 
    }
    printf("  %2d. %s: %s\n", number, padded_command, description);
}

void display_help_menu() { 
    int item_number = 1;
    if (!isLoggedIn) { 
        printf("========= HELP =========\n");
        printf("Kamu belum login sebagai role apapun. Silahkan login terlebih dahulu.\n");
        display_menu_item_indented(item_number++, "LOGIN", "Masuk ke dalam akun yang sudah terdaftar");
        display_menu_item_indented(item_number++, "REGISTRASI", "Membuat akun baru sebagai pasien");
        
    } else { 
        if (strcmp(currentUser.role, "manager") == 0) { 
            printf("========= HELP =========\n"); 
            printf("Halo Manager %s. Kenapa kamu memanggil command HELP?\n", currentUser.username); 
            printf("Kan kamu manager, tapi yasudahlah kamu pasti sedang kebingungan. Berikut\n");
            printf("adalah hal-hal yang dapat kamu lakukan sekarang:\n");
            
            display_menu_item_indented(item_number++, "LOGOUT", "Keluar dari akun yang sedang digunakan");
            // F07 - Lihat User, Lihat Pasien, Lihat Dokter
            display_menu_item_indented(item_number++, "LIHAT_USER", "Melihat daftar semua pengguna (dokter dan pasien)");
            display_menu_item_indented(item_number++, "LIHAT_PASIEN", "Melihat daftar semua pasien");
            display_menu_item_indented(item_number++, "LIHAT_DOKTER", "Melihat daftar semua dokter");
            // F08 - Cari User, Cari Pasien, Cari Dokter
            display_menu_item_indented(item_number++, "CARI_USER", "Mencari pengguna spesifik (dokter atau pasien)");
            display_menu_item_indented(item_number++, "CARI_PASIEN", "Mencari pasien spesifik");
            display_menu_item_indented(item_number++, "CARI_DOKTER", "Mencari dokter spesifik");
            // S01 - Lihat Denah, Lihat Ruangan
            display_menu_item_indented(item_number++, "LIHAT_DENAH", "Melihat denah rumah sakit");
            display_menu_item_indented(item_number++, "LIHAT_RUANGAN", "Melihat detail spesifik sebuah ruangan");
            // S02- Lihat Semua Antrian
            display_menu_item_indented(item_number++, "LIHAT_SEMUA_ANTRIAN", "Melihat rincian antrian di semua ruangan");
            // F10 - Tambah Dokter
            display_menu_item_indented(item_number++, "TAMBAH_DOKTER", "Mendaftarkan dokter baru ke sistem"); 
            // F10 - Assign Dokter
            display_menu_item_indented(item_number++, "ASSIGN_DOKTER", "Menugaskan dokter ke ruangan tertentu");
            printf("  %2d. #...dan seterusnya (fitur lain untuk Manager)\n", item_number++);


        } else if (strcmp(currentUser.role, "dokter") == 0) { 
            printf("========= HELP =========\n"); 
            printf("Halo Dokter %s.\n", currentUser.nama_user); 
            printf("Kamu memanggil command HELP. Kamu pasti sedang kebingungan. Berikut\n");
            printf("adalah hal-hal yang dapat kamu lakukan sekarang:\n");
            
            display_menu_item_indented(item_number++, "LOGOUT", "Keluar dari akun yang sedang digunakan");
            // S01 - Lihat Denah, Lihat Ruangan
            display_menu_item_indented(item_number++, "LIHAT_DENAH", "Melihat denah rumah sakit");
            display_menu_item_indented(item_number++, "LIHAT_RUANGAN", "Melihat detail spesifik sebuah ruangan");
            // F11 - Diagnosis
            display_menu_item_indented(item_number++, "DIAGNOSIS", "Melakukan diagnosis penyakit pasien"); 
            // F12 - Ngobatin
            display_menu_item_indented(item_number++, "NGOBATIN", "Memberikan resep obat kepada pasien");
            printf("  %2d. #...dan seterusnya (fitur lain untuk Dokter)\n", item_number++);


        } else if (strcmp(currentUser.role, "pasien") == 0) { 

            printf("========= HELP =========\n"); 
            printf("Selamat datang, %s.\n", currentUser.username); 
            printf("Kamu memanggil command HELP. Kamu pasti sedang kebingungan. Berikut\n");
            printf("adalah hal-hal yang dapat kamu lakukan sekarang:\n");
            
            display_menu_item_indented(item_number++, "LOGOUT", "Keluar dari akun yang sedang digunakan");
            // S01 - Lihat Denah, Lihat Ruangan
            display_menu_item_indented(item_number++, "LIHAT_DENAH", "Melihat denah rumah sakit");
            display_menu_item_indented(item_number++, "LIHAT_RUANGAN", "Melihat detail spesifik sebuah ruangan");
            // F13 - Aku boleh pulang ga, dok?
            display_menu_item_indented(item_number++, "PULANGDOK", "Meminta izin pulang kepada dokter");
            // F14 - Daftar Check-Up
            display_menu_item_indented(item_number++, "DAFTAR_CHECKUP", "Mendaftarkan diri untuk pemeriksaan dokter"); 
            // F15 - Antrian Saya!
            display_menu_item_indented(item_number++, "ANTRIAN", "Melihat status antrian Anda saat ini");
            // F16 - Minum Obat
            display_menu_item_indented(item_number++, "MINUM_OBAT", "Meminum obat dari inventory");
            // F17 - Minum Penawar
            display_menu_item_indented(item_number++, "PENAWAR", "Meminum penawar untuk mengeluarkan obat terakhir");
            printf("  %2d. #...dan seterusnya (fitur lain untuk Pasien)\n", item_number++);

        } else {
            printf("ERROR: Role pengguna tidak dikenal.\n");
        }
    }
    display_help_footnote_indented();
}