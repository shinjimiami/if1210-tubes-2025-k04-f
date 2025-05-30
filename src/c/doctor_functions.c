#include "doctor_functions.h"
#include "user_interface.h"
#include "session_manager.h"
#include "data_manager.h"
#include "definitions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Fungsi Helper---
static User* get_user_by_id(int user_id) {
    for (int i = 0; i < global_jumlah_users; i++) {
        if (global_users[i].id_user == user_id) {
            return &global_users[i];
        }
    }
    return NULL;
}

static int get_user_idx_by_id(int user_id) { 
    for (int i = 0; i < global_jumlah_users; i++) {
        if (global_users[i].id_user == user_id) {
            return i;
        }
    }
    return -1;
}

static void remove_pasien_from_list_by_index(int* list_pasien_id, int* jumlah_pasien, int index_to_remove) {
    if (index_to_remove < 0 || index_to_remove >= *jumlah_pasien) {
        return;
    }
    for (int i = index_to_remove; i < (*jumlah_pasien) - 1; i++) {
        list_pasien_id[i] = list_pasien_id[i + 1];
    }
    (*jumlah_pasien)--;
}

static const char* get_nama_obat_by_id(int id_obat) {
    for (int i = 0; i < global_jumlah_obat; i++) {
        if (global_obat_list[i].id_obat == id_obat) {
            return global_obat_list[i].nama_obat;
        }
    }
    return "Obat Tidak Dikenal";
}

static int compare_obat_urutan(const void* a, const void* b) {
    ObatPenyakitRelasi* relasiA = (ObatPenyakitRelasi*)a;
    ObatPenyakitRelasi* relasiB = (ObatPenyakitRelasi*)b;
    return (relasiA->urutan_minum - relasiB->urutan_minum);
}

void handle_diagnosis() {
    if (strcmp(currentUser.role, "dokter") != 0) {
        display_error("Perintah ini hanya untuk Dokter.");
        return;
    }

    int id_dokter_login = currentUser.id_user;
    int doctor_room_idx = -1;

    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) { 
        if (denah_rs_sti[i].id_dokter_bertugas == id_dokter_login) { 
            doctor_room_idx = i;
            break;
        }
    }

    if (doctor_room_idx == -1) {
        display_message("Anda tidak sedang bertugas di ruangan manapun untuk melakukan diagnosis.");
        return;
    }

    RuanganSTI* ruangan_dokter = &denah_rs_sti[doctor_room_idx];
    User* pasien_untuk_diagnosa_obj = NULL;
    int pasien_global_idx = -1;
    int pasien_dalam_ruangan_idx_to_process = -1;
    int target_pasien_id = -1;

    for (int i = 0; i < ruangan_dokter->jumlah_pasien_di_ruangan; i++) {
        User* calon_pasien = get_user_by_id(ruangan_dokter->daftar_id_pasien_di_ruangan[i]);
        if (calon_pasien != NULL && !calon_pasien->sudah_didiagnosis) {
            target_pasien_id = calon_pasien->id_user;
            pasien_dalam_ruangan_idx_to_process = i;
            break;
        }
    }

    if (target_pasien_id == -1) {
        if (ruangan_dokter->jumlah_pasien_antrian_luar > 0) {
            if (ruangan_dokter->jumlah_pasien_di_ruangan < kapasitas_maks_pasien_per_ruangan_sti) {
                int id_pasien_dari_luar = ruangan_dokter->daftar_id_pasien_antrian_luar[0];
                User* pasien_dari_luar_obj = get_user_by_id(id_pasien_dari_luar);

                if (pasien_dari_luar_obj != NULL && !pasien_dari_luar_obj->sudah_didiagnosis) {
                    target_pasien_id = id_pasien_dari_luar;
                    
                    ruangan_dokter->daftar_id_pasien_di_ruangan[ruangan_dokter->jumlah_pasien_di_ruangan] = target_pasien_id;
                    pasien_dalam_ruangan_idx_to_process = ruangan_dokter->jumlah_pasien_di_ruangan;
                    ruangan_dokter->jumlah_pasien_di_ruangan++;
                    
                    remove_pasien_from_list_by_index(ruangan_dokter->daftar_id_pasien_antrian_luar, &ruangan_dokter->jumlah_pasien_antrian_luar, 0);
                    printf("Memanggil pasien %s dari antrian luar ke dalam ruangan...\n", pasien_dari_luar_obj->nama_user);
                }
            } else {
                display_message("Ruangan penuh, tidak bisa memanggil pasien dari antrian luar saat ini.");
            }
        }
    }

    if (target_pasien_id == -1) {
        display_message("Tidak ada pasien baru untuk diperiksa!");
        return;
    }

    pasien_untuk_diagnosa_obj = get_user_by_id(target_pasien_id);
    pasien_global_idx = get_user_idx_by_id(target_pasien_id);

    if (pasien_untuk_diagnosa_obj == NULL || pasien_global_idx == -1) {
        display_error("Data pasien terdepan tidak valid. Proses dibatalkan.");
        if (pasien_dalam_ruangan_idx_to_process != -1 && pasien_dalam_ruangan_idx_to_process < ruangan_dokter->jumlah_pasien_di_ruangan && ruangan_dokter->daftar_id_pasien_di_ruangan[pasien_dalam_ruangan_idx_to_process] == target_pasien_id) { //
             remove_pasien_from_list_by_index(ruangan_dokter->daftar_id_pasien_di_ruangan, &ruangan_dokter->jumlah_pasien_di_ruangan, pasien_dalam_ruangan_idx_to_process);
        }
        return;
    }
    
    bool penyakit_ditemukan_sekarang = false;
    global_users[pasien_global_idx].id_penyakit_terdiagnosis = 0;
    strcpy(global_users[pasien_global_idx].penyakit_utama, "");

    for (int i = 0; i < global_jumlah_penyakit; i++) {
        Penyakit* db_penyakit = &global_penyakit_db[i];
        bool match = true;

        if (!(pasien_untuk_diagnosa_obj->suhu_tubuh >= db_penyakit->suhu_tubuh_min && pasien_untuk_diagnosa_obj->suhu_tubuh <= db_penyakit->suhu_tubuh_max)) match = false;
        if (match && !(pasien_untuk_diagnosa_obj->tekanan_darah_sistolik >= db_penyakit->tekanan_darah_sistolik_min && pasien_untuk_diagnosa_obj->tekanan_darah_sistolik <= db_penyakit->tekanan_darah_sistolik_max)) match = false;
        if (pasien_untuk_diagnosa_obj->berat_badan > 0) {
             if (match && !(pasien_untuk_diagnosa_obj->berat_badan >= db_penyakit->berat_badan_min && pasien_untuk_diagnosa_obj->berat_badan <= db_penyakit->berat_badan_max)) match = false;
        }
        if (pasien_untuk_diagnosa_obj->tinggi_badan > 0) {
            if (match && !(pasien_untuk_diagnosa_obj->tinggi_badan >= db_penyakit->tinggi_badan_min && pasien_untuk_diagnosa_obj->tinggi_badan <= db_penyakit->tinggi_badan_max)) match = false;
        }
        
        if (match) {
            strcpy(global_users[pasien_global_idx].penyakit_utama, db_penyakit->nama_penyakit);
            global_users[pasien_global_idx].id_penyakit_terdiagnosis = db_penyakit->id;
            printf("%s terdiagnosa penyakit %s!\n", pasien_untuk_diagnosa_obj->nama_user, db_penyakit->nama_penyakit);
            penyakit_ditemukan_sekarang = true;
            break; 
        }
    }

    global_users[pasien_global_idx].sudah_didiagnosis = true;

    if (!penyakit_ditemukan_sekarang) {
        printf("%s tidak terdiagnosis penyakit apapun!\n", pasien_untuk_diagnosa_obj->nama_user);
        if (pasien_dalam_ruangan_idx_to_process != -1 && pasien_dalam_ruangan_idx_to_process < ruangan_dokter->jumlah_pasien_di_ruangan && ruangan_dokter->daftar_id_pasien_di_ruangan[pasien_dalam_ruangan_idx_to_process] == target_pasien_id) { 
             remove_pasien_from_list_by_index(ruangan_dokter->daftar_id_pasien_di_ruangan, &ruangan_dokter->jumlah_pasien_di_ruangan, pasien_dalam_ruangan_idx_to_process); 
             printf("%s telah dikeluarkan dari ruangan dokter karena dinyatakan sehat.\n", pasien_untuk_diagnosa_obj->nama_user); 
        }
    }
}


// --- F12 - Ngobatin (BARU) ---
void handle_ngobatin() {
    if (strcmp(currentUser.role, "dokter") != 0) {
        display_error("Perintah ini hanya untuk Dokter.");
        return;
    }

    int id_dokter_login = currentUser.id_user;
    int doctor_room_idx = -1;

    //Cari ruangan dokter yang sedang login
    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) { 
        if (denah_rs_sti[i].id_dokter_bertugas == id_dokter_login) {
            doctor_room_idx = i;
            break;
        }
    }

    if (doctor_room_idx == -1) {
        display_message("Anda tidak sedang bertugas di ruangan manapun untuk memberikan obat.");
        return;
    }

    RuanganSTI* ruangan_dokter = &denah_rs_sti[doctor_room_idx]; 
    User* pasien_untuk_diobati = NULL;
    int pasien_global_idx = -1;

    if (ruangan_dokter->jumlah_pasien_di_ruangan > 0) { 
        pasien_untuk_diobati = get_user_by_id(ruangan_dokter->daftar_id_pasien_di_ruangan[0]); 
        if(pasien_untuk_diobati != NULL) {
            pasien_global_idx = get_user_idx_by_id(pasien_untuk_diobati->id_user);
        }
    }

    if (pasien_untuk_diobati == NULL || pasien_global_idx == -1) {
        printf("Dokter sedang mengobati pasien!\n");
        display_message("Tidak ada pasien di ruangan Anda untuk diobati!");
        return;
    }

    printf("Dokter sedang mengobati pasien!\n");

    //Cek status diagnosis pasien
    if (!global_users[pasien_global_idx].sudah_didiagnosis) { 
        printf("Pasien %s belum di diagnosis!\n", global_users[pasien_global_idx].nama_user);
        return;
    }

    if (global_users[pasien_global_idx].id_penyakit_terdiagnosis == 0) { 
        printf("Pasien %s tidak memiliki penyakit (berdasarkan diagnosis sebelumnya)!\n", global_users[pasien_global_idx].nama_user);
        return;
    }

    //Jika pasien memiliki penyakit dan sudah didiagnosis
    printf("Pasien memiliki penyakit %s\n", global_users[pasien_global_idx].penyakit_utama); 
    printf("Obat yang harus diberikan:\n");

    ObatPenyakitRelasi resep_pasien[MAX_OBAT_PENYAKIT_RELATIONS];
    int jumlah_obat_diresep = 0;

    // Kumpulkan semua obat untuk penyakit pasien
    for (int i = 0; i < global_jumlah_obat_penyakit; i++) {
        if (global_obat_penyakit_list[i].id_penyakit == global_users[pasien_global_idx].id_penyakit_terdiagnosis) { 
            if (jumlah_obat_diresep < MAX_OBAT_PENYAKIT_RELATIONS) {
                resep_pasien[jumlah_obat_diresep++] = global_obat_penyakit_list[i];
            }
        }
    }

    if (jumlah_obat_diresep == 0) {
        printf("  Tidak ada resep obat yang ditemukan untuk penyakit %s.\n", global_users[pasien_global_idx].penyakit_utama);
    } else {
        // Urutkan resep berdasarkan urutan_minum
        qsort(resep_pasien, jumlah_obat_diresep, sizeof(ObatPenyakitRelasi), compare_obat_urutan);

        for (int i = 0; i < jumlah_obat_diresep; i++) {
            const char* nama_obat = get_nama_obat_by_id(resep_pasien[i].id_obat);
            printf("  %d. %s\n", i + 1, nama_obat);
        }
    }
}
