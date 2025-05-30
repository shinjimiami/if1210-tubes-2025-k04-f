#include "denah_functions.h"
#include "definitions.h"    
#include "user_interface.h" 
#include "data_manager.h"   
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char* get_user_name_by_id_internal(int user_id) {
    if (user_id == ID_DOKTER_KOSONG) return ""; 
    for (int i = 0; i < global_jumlah_users; i++) { 
        if (global_users[i].id_user == user_id) { 
            return global_users[i].nama_user; 
        }
    }
    static char not_found_buffer[30];
    sprintf(not_found_buffer, "ID %d Tidak Dikenal", user_id);
    return not_found_buffer; 
}


void handle_lihat_denah_sti() { 
    if (jumlah_ruangan_aktual_sti <= 0) {
        display_message("Denah rumah sakit belum diinisialisasi atau tidak ada ruangan.");
        return;
    }

    printf("+");
    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) {
        printf("----"); 
    }
    printf("+\n");

    printf("|");
    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) {
        printf(" %2d |", denah_rs_sti[i].id_ruangan); 
    }
    printf("\n");

    printf("+");
    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) {
        printf("----");
    }
    printf("+\n");
}

void display_detail_ruangan_sti(int nomor_ruangan_input) {
    if (nomor_ruangan_input <= 0 || nomor_ruangan_input > jumlah_ruangan_aktual_sti) {
        printf("Nomor ruangan tidak valid. Ruangan tersedia: 1-%d.\n", jumlah_ruangan_aktual_sti);
        return;
    }

    RuanganSTI* ruangan = &denah_rs_sti[nomor_ruangan_input - 1]; 

    printf("Detail Ruangan %d\n", ruangan->id_ruangan);
    printf("Kapasitas: %d\n", kapasitas_maks_pasien_per_ruangan_sti); 

    const char* nama_dokter = get_user_name_by_id_internal(ruangan->id_dokter_bertugas);
    if (ruangan->id_dokter_bertugas != ID_DOKTER_KOSONG && strlen(nama_dokter) > 0 && strcmp(nama_dokter, "ID Tidak Dikenal") !=0) {
        printf("Dokter   : %s\n", nama_dokter);
    } else {
        printf("Dokter   : -\n"); 
    }

    printf("Pasien di dalam ruangan:\n");
    if (ruangan->jumlah_pasien_di_ruangan == 0) {
        printf("  Tidak ada pasien di dalam ruangan saat ini.\n");
    } else {
        for (int i = 0; i < ruangan->jumlah_pasien_di_ruangan; i++) {
            const char* nama_pasien = get_user_name_by_id_internal(ruangan->daftar_id_pasien_di_ruangan[i]);
            printf("  %d. %s\n", i + 1, nama_pasien);
        }
    }
}

void handle_lihat_ruangan_sti() {
    char buffer_nomor_ruangan[10];
    get_user_input("Masukkan nomor ruangan yang ingin dilihat detailnya: ", buffer_nomor_ruangan, 10);
    int nomor_ruangan = atoi(buffer_nomor_ruangan);

    if (nomor_ruangan == 0 && strcmp(buffer_nomor_ruangan, "0") != 0 && strcmp(buffer_nomor_ruangan, "") != 0) { 
        display_error("Input nomor ruangan tidak valid.");
        return;
    }
    display_detail_ruangan_sti(nomor_ruangan);
}

// Implementasi untuk S02 (F09 STI)
void handle_lihat_semua_antrian_sti() {
    handle_lihat_denah_sti();
    printf("\n");

    bool ada_ruangan_dengan_dokter = false;
    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) {
        RuanganSTI* ruangan = &denah_rs_sti[i];
        if (ruangan->id_dokter_bertugas != ID_DOKTER_KOSONG) {
            ada_ruangan_dengan_dokter = true;
            printf("============ %d ============\n", ruangan->id_ruangan);
            printf("Kapasitas : %d\n", kapasitas_maks_pasien_per_ruangan_sti);

            const char* nama_dokter = get_user_name_by_id_internal(ruangan->id_dokter_bertugas);
             if (strlen(nama_dokter) > 0 && strcmp(nama_dokter, "ID Tidak Dikenal") !=0) {
                printf("Dokter    : %s\n", nama_dokter);
            } else {
                printf("Dokter    : (Data Dokter Tidak Lengkap untuk ID %d)\n", ruangan->id_dokter_bertugas);
            }
            

            printf("Pasien di dalam ruangan:\n");
            if (ruangan->jumlah_pasien_di_ruangan == 0) {
                printf("  Tidak ada pasien di dalam ruangan saat ini.\n");
            } else {
                for (int j = 0; j < ruangan->jumlah_pasien_di_ruangan; j++) {
                    const char* nama_pasien = get_user_name_by_id_internal(ruangan->daftar_id_pasien_di_ruangan[j]);
                    printf("  %d. %s\n", j + 1, nama_pasien);
                }
            }

            printf("Pasien di antrian:\n");
            if (ruangan->jumlah_pasien_antrian_luar == 0) {
                printf("  Tidak ada pasien di antrian saat ini.\n");
            } else {
                for (int j = 0; j < ruangan->jumlah_pasien_antrian_luar; j++) {
                    const char* nama_pasien_antrian = get_user_name_by_id_internal(ruangan->daftar_id_pasien_antrian_luar[j]);
                    printf("  %d. %s\n", j + 1, nama_pasien_antrian);
                }
            }
            printf("\n");
        }
    }

    if (!ada_ruangan_dengan_dokter) {
        display_message("Tidak ada dokter yang bertugas di ruangan manapun saat ini.");
    }
}