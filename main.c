#include <stdio.h>
#include <stdlib.h> // Untuk atoi, exit
#include <string.h> // Untuk strcmp
#include <stdbool.h> // Untuk bool
#include <ctype.h>   // Untuk isspace, toupper

    // Karena CFLAGS menyertakan -I./src/h, kita bisa include header seolah-olah mereka ada di path include standar
#include "definitions.h"
#include "user_interface.h"
#include "command_handler.h"
#include "data_manager.h"
#include "session_manager.h" 
    // utils.h tidak di-include langsung di main.c, tapi pastikan ada di src/h/ untuk modul lain

    // Variabel global dideklarasikan sebagai extern di definitions.h
    // dan didefinisikan di data_manager.c atau session_manager.c
    // int jumlah_ruangan_aktual_sti; // Sudah extern
    // int kapasitas_maks_pasien_per_ruangan_sti; // Sudah extern
    // int kapasitas_maks_antrian_luar_sti; // Sudah extern

    // Deklarasi prototipe untuk fungsi helper
    bool is_all_whitespace(const char *str);

// main.c
// ... (include yang sudah ada) ...

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Jumlah ruangan STI tidak valid (harus antara 1 dan %d).\n", MAX_RUANGAN_STI);
        return 1;
    }

    // Inisialisasi jumlah ruangan
    jumlah_ruangan_aktual_sti = atoi(argv[1]);
    if (jumlah_ruangan_aktual_sti < 1 || jumlah_ruangan_aktual_sti > MAX_RUANGAN_STI) {
        printf("Jumlah ruangan STI tidak valid (harus antara 1 dan %d).\n", MAX_RUANGAN_STI);
        return 1;
    }

    // PENTING: Inisialisasi data sebelum memulai program
    initialize_all_data();        // Inisialisasi users, penyakit, dan obat
    initialize_denah_sti();       // Inisialisasi denah rumah sakit
    initialize_command_metadata(); // Inisialisasi metadata command

    char input_buffer[MAX_INPUT_BUFFER]; //
    bool running = true;                 //

    while (running) {
        display_main_prompt(); //
        get_user_input("", input_buffer, MAX_INPUT_BUFFER); //

        // --- Bagian ini dimodifikasi dari versi Anda sebelumnya ---
        char command_to_check[MAX_INPUT_BUFFER];
        strcpy(command_to_check, input_buffer);
        // Trim spasi di awal command_to_check
        int start_idx = 0;
        while (isspace((unsigned char)command_to_check[start_idx])) {
            start_idx++;
        }
        // Trim spasi di akhir command_to_check
        int end_idx = strlen(command_to_check) - 1;
        while (end_idx >= start_idx && isspace((unsigned char)command_to_check[end_idx])) {
            command_to_check[end_idx--] = '\0';
        }
        // Konversi ke uppercase untuk pengecekan KELUAR yang case-insensitive
        for (int k = start_idx; command_to_check[k]; k++) {
            command_to_check[k] = toupper((unsigned char)command_to_check[k]);
        }
        // --- Akhir Modifikasi untuk command_to_check ---

        if (strcmp(command_to_check + start_idx, "EXIT") == 0) { 
        // GANTI "KELUAR" MENJADI "EXIT" DI SINI JUGA
        const CommandMetadata* cmd_exit_meta = get_command_metadata_from_string("EXIT"); 
        if (cmd_exit_meta && has_permission_for_command(cmd_exit_meta)) {
            // ... (prompt konfirmasi save) ...

            // Panggil handler KELUAR dengan string yang benar atau biarkan main yang handle pesan
            process_user_input("EXIT"); // Atau input_buffer jika masih berisi "EXIT" yang bersih
                                        // Atau display_message(...) langsung di sini

            running = false;
        } else {
             display_error("Anda tidak memiliki izin untuk keluar saat ini (seharusnya tidak terjadi).");
        }
    } else if (strlen(input_buffer + start_idx) > 0 && !is_all_whitespace(input_buffer + start_idx)) { 
        process_user_input(input_buffer); 
    }
        // Jika input hanya whitespace, loop akan berlanjut tanpa memproses apa pun.
    }
    return 0; //
}

// ... (fungsi is_all_whitespace) ...

    // Fungsi helper untuk mengecek apakah string hanya berisi whitespace
    bool is_all_whitespace(const char *str) {
        while (*str) {
            if (!isspace((unsigned char)*str)) {
                return false;
            }
            str++;
        }
        return true;
    }
