// src/c/data_manager.c

#include "data_manager.h"
#include <string.h> 
#include "definitions.h" 
#include <stdio.h>

User global_users[MAX_USERS];
int global_jumlah_users = 0;
int g_next_user_id = 1;

RuanganSTI denah_rs_sti[MAX_RUANGAN_STI];
int jumlah_ruangan_aktual_sti = 0; 
int kapasitas_maks_pasien_per_ruangan_sti = 3; 
int kapasitas_maks_antrian_luar_sti = 4; 

//Definisi global database penyakit
Penyakit global_penyakit_db[MAX_PENYAKIT_DB];
int global_jumlah_penyakit = 0;

//Definisi Variabel Global untuk Data Obat
Obat global_obat_list[MAX_OBAT];
int global_jumlah_obat = 0;
ObatPenyakitRelasi global_obat_penyakit_list[MAX_OBAT_PENYAKIT_RELATIONS];
int global_jumlah_obat_penyakit = 0;

void initialize_obat_data() {
    global_jumlah_obat = 0;
    global_jumlah_obat_penyakit = 0;

    if (global_jumlah_obat < MAX_OBAT) { global_obat_list[global_jumlah_obat++] = (Obat){2, "Vitamin C"}; }
    if (global_jumlah_obat < MAX_OBAT) { global_obat_list[global_jumlah_obat++] = (Obat){1, "Oseltamivir"}; }
    if (global_jumlah_obat < MAX_OBAT) { global_obat_list[global_jumlah_obat++] = (Obat){4, "Lisinopril"}; }
    if (global_jumlah_obat < MAX_OBAT) { global_obat_list[global_jumlah_obat++] = (Obat){5, "Metformin"}; }
    if (global_jumlah_obat < MAX_OBAT) { global_obat_list[global_jumlah_obat++] = (Obat){3, "Remdesivir"}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){1, 1, 1}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){3, 2, 1}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){2, 1, 2}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){4, 4, 2}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){4, 3, 1}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){5, 4, 1}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){2, 5, 2}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){3, 4, 3}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){2, 4, 4}; }
    if (global_jumlah_obat_penyakit < MAX_OBAT_PENYAKIT_RELATIONS) { global_obat_penyakit_list[global_jumlah_obat_penyakit++] = (ObatPenyakitRelasi){3, 5, 1}; }

    printf("DEBUG: Database obat dan relasi obat-penyakit hardcoded telah diinisialisasi. Jumlah obat: %d, Jumlah relasi: %d\n", global_jumlah_obat, global_jumlah_obat_penyakit);
}

void initialize_penyakit_db() {
    global_jumlah_penyakit = 0;

    // Penyakit 1: Influenza
    if (global_jumlah_penyakit < MAX_PENYAKIT_DB) {
        Penyakit* p = &global_penyakit_db[global_jumlah_penyakit];
        p->id = 1;
        strcpy(p->nama_penyakit, "Influenza");
        p->suhu_tubuh_min = 36.0f; p->suhu_tubuh_max = 38.5f;
        p->tekanan_darah_sistolik_min = 90; p->tekanan_darah_sistolik_max = 120;
        p->tekanan_darah_diastolik_min = 60; p->tekanan_darah_diastolik_max = 80;
        p->detak_jantung_min = 60; p->detak_jantung_max = 100;
        p->saturasi_oksigen_min = 95.0f; p->saturasi_oksigen_max = 100.0f;
        p->kadar_gula_darah_min = 70; p->kadar_gula_darah_max = 140;
        p->berat_badan_min = 45.0f; p->berat_badan_max = 90.0f; 
        p->tinggi_badan_min = 150; p->tinggi_badan_max = 185;   
        p->kadar_kolesterol_min = 150; p->kadar_kolesterol_max = 240;
        p->trombosit_min = 150; p->trombosit_max = 450;
        global_jumlah_penyakit++;
    }

    // Penyakit 2: COVID-19
    if (global_jumlah_penyakit < MAX_PENYAKIT_DB) {
        Penyakit* p = &global_penyakit_db[global_jumlah_penyakit];
        p->id = 2;
        strcpy(p->nama_penyakit, "COVID-19");
        p->suhu_tubuh_min = 36.0f; p->suhu_tubuh_max = 39.0f;
        p->tekanan_darah_sistolik_min = 90; p->tekanan_darah_sistolik_max = 130;
        p->tekanan_darah_diastolik_min = 60; p->tekanan_darah_diastolik_max = 85;
        p->detak_jantung_min = 60; p->detak_jantung_max = 120;
        p->saturasi_oksigen_min = 90.0f; p->saturasi_oksigen_max = 100.0f;
        p->kadar_gula_darah_min = 70; p->kadar_gula_darah_max = 180;
        p->berat_badan_min = 45.0f; p->berat_badan_max = 90.0f;
        p->tinggi_badan_min = 150; p->tinggi_badan_max = 185;
        p->kadar_kolesterol_min = 150; p->kadar_kolesterol_max = 240;
        p->trombosit_min = 150; p->trombosit_max = 450;
        global_jumlah_penyakit++;
    }
    
    // Penyakit 3: Hipertensi
    if (global_jumlah_penyakit < MAX_PENYAKIT_DB) {
        Penyakit* p = &global_penyakit_db[global_jumlah_penyakit];
        p->id = 3;
        strcpy(p->nama_penyakit, "Hipertensi");
        p->suhu_tubuh_min = 36.0f; p->suhu_tubuh_max = 37.5f;
        p->tekanan_darah_sistolik_min = 140; p->tekanan_darah_sistolik_max = 180; 
        p->tekanan_darah_diastolik_min = 90; p->tekanan_darah_diastolik_max = 120;   
        p->detak_jantung_min = 60; p->detak_jantung_max = 100;
        p->saturasi_oksigen_min = 90.0f; p->saturasi_oksigen_max = 100.0f;
        p->kadar_gula_darah_min = 70; p->kadar_gula_darah_max = 140;
        p->berat_badan_min = 45.0f; p->berat_badan_max = 90.0f;
        p->tinggi_badan_min = 150; p->tinggi_badan_max = 185;
        p->kadar_kolesterol_min = 150; p->kadar_kolesterol_max = 240;
        p->trombosit_min = 150; p->trombosit_max = 450;
        global_jumlah_penyakit++;
    }

    // Penyakit 4: Diabetes Mellitus
    if (global_jumlah_penyakit < MAX_PENYAKIT_DB) {
        Penyakit* p = &global_penyakit_db[global_jumlah_penyakit];
        p->id = 4;
        strcpy(p->nama_penyakit, "Diabetes Mellitus");
        p->suhu_tubuh_min = 36.0f; p->suhu_tubuh_max = 37.5f;
        p->tekanan_darah_sistolik_min = 90; p->tekanan_darah_sistolik_max = 140;
        p->tekanan_darah_diastolik_min = 60; p->tekanan_darah_diastolik_max = 90;
        p->detak_jantung_min = 60; p->detak_jantung_max = 100;
        p->saturasi_oksigen_min = 90.0f; p->saturasi_oksigen_max = 100.0f;
        p->kadar_gula_darah_min = 126; p->kadar_gula_darah_max = 500; 
        p->berat_badan_min = 45.0f; p->berat_badan_max = 90.0f;
        p->tinggi_badan_min = 150; p->tinggi_badan_max = 185;
        p->kadar_kolesterol_min = 150; p->kadar_kolesterol_max = 240;
        p->trombosit_min = 150; p->trombosit_max = 450;
        global_jumlah_penyakit++;
    }
    
    // Penyakit 5: Anemia
    if (global_jumlah_penyakit < MAX_PENYAKIT_DB) {
        Penyakit* p = &global_penyakit_db[global_jumlah_penyakit];
        p->id = 5;
        strcpy(p->nama_penyakit, "Anemia");
        p->suhu_tubuh_min = 36.0f; p->suhu_tubuh_max = 37.0f;
        p->tekanan_darah_sistolik_min = 90; p->tekanan_darah_sistolik_max = 120; 
        p->tekanan_darah_diastolik_min = 60; p->tekanan_darah_diastolik_max = 80; 
        p->detak_jantung_min = 60; p->detak_jantung_max = 100; 
        p->saturasi_oksigen_min = 95.0f; p->saturasi_oksigen_max = 100.0f;
        p->kadar_gula_darah_min = 70; p->kadar_gula_darah_max = 140;
        p->berat_badan_min = 45.0f; p->berat_badan_max = 90.0f;
        p->tinggi_badan_min = 150; p->tinggi_badan_max = 185;
        p->kadar_kolesterol_min = 150; p->kadar_kolesterol_max = 240;
        p->trombosit_min = 150; p->trombosit_max = 450; 
        global_jumlah_penyakit++;
    }

    printf("DEBUG: Database penyakit hardcoded telah diinisialisasi. Jumlah penyakit: %d\n", global_jumlah_penyakit);
}


void initialize_denah_sti() {
    if (jumlah_ruangan_aktual_sti <= 0 || jumlah_ruangan_aktual_sti > MAX_RUANGAN_STI) {
        printf("Peringatan: Jumlah ruangan aktual STI tidak valid (%d), tidak bisa inisialisasi denah.\n", jumlah_ruangan_aktual_sti);
        return;
    }
    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) {
        denah_rs_sti[i].id_ruangan = i + 1;
        denah_rs_sti[i].id_dokter_bertugas = ID_DOKTER_KOSONG; 
        denah_rs_sti[i].jumlah_pasien_di_ruangan = 0;
        denah_rs_sti[i].jumlah_pasien_antrian_luar = 0; 

        if (i == 0 && global_jumlah_users > 0) { 
            int id_dokter_target = 10; 
            bool dokter_ditemukan = false;
            for(int u_idx = 0; u_idx < global_jumlah_users; ++u_idx) {
                if (global_users[u_idx].id_user == id_dokter_target && strcmp(global_users[u_idx].role, "dokter") == 0) {
                    denah_rs_sti[i].id_dokter_bertugas = id_dokter_target;
                    dokter_ditemukan = true;
                    break;
                }
            }

            if (dokter_ditemukan) {
                int pasien_ids_dalam[] = {1, 2, 3}; 
                int count_pasien_dalam = sizeof(pasien_ids_dalam) / sizeof(pasien_ids_dalam[0]);
                for (int p_id_idx = 0; p_id_idx < count_pasien_dalam; ++p_id_idx) {
                    for (int u_idx = 0; u_idx < global_jumlah_users; ++u_idx) {
                        if (global_users[u_idx].id_user == pasien_ids_dalam[p_id_idx] && strcmp(global_users[u_idx].role, "pasien") == 0) {
                            if (denah_rs_sti[i].jumlah_pasien_di_ruangan < MAX_PASIEN_PER_RUANGAN_STI) { 
                                denah_rs_sti[i].daftar_id_pasien_di_ruangan[denah_rs_sti[i].jumlah_pasien_di_ruangan++] = pasien_ids_dalam[p_id_idx];
                            }
                            break;
                        }
                    }
                }
                
                int pasien_ids_luar[] = {16, 20}; 
                int count_pasien_luar = sizeof(pasien_ids_luar) / sizeof(pasien_ids_luar[0]);
                 for (int p_id_idx = 0; p_id_idx < count_pasien_luar; ++p_id_idx) {
                    for (int u_idx = 0; u_idx < global_jumlah_users; ++u_idx) {
                        if (global_users[u_idx].id_user == pasien_ids_luar[p_id_idx] && strcmp(global_users[u_idx].role, "pasien") == 0) {
                            if (denah_rs_sti[i].jumlah_pasien_antrian_luar < MAX_PASIEN_ANTRIAN_LUAR_STI) {
                                denah_rs_sti[i].daftar_id_pasien_antrian_luar[denah_rs_sti[i].jumlah_pasien_antrian_luar++] = pasien_ids_luar[p_id_idx];
                            }
                            break;
                        }
                    }
                }
            }
        } else if (i == 1 && global_jumlah_users > 0) { 
            int id_dokter_target = 11; 
            for(int u_idx = 0; u_idx < global_jumlah_users; ++u_idx) {
                if (global_users[u_idx].id_user == id_dokter_target && strcmp(global_users[u_idx].role, "dokter") == 0) {
                    denah_rs_sti[i].id_dokter_bertugas = id_dokter_target;
                    break;
                }
            }
        }
    }
}

void initialize_users() {
    global_jumlah_users = 0;
    g_next_user_id = 1;

    // User 1: Manager "zeru" (ID 88)
    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 88; strcpy(u->nama_user, "Zeru Manager"); strcpy(u->username, "zeru");
        strcpy(u->password, "pass77"); strcpy(u->role, "manager"); strcpy(u->penyakit_utama, "");
        u->id_penyakit_terdiagnosis = 0;
        u->sudah_didiagnosis = false; 
        u->jumlah_obat_di_inventory = 0;
        u->top_perut_obat = -1;     
        u->suhu_tubuh = 0.0f; u->tekanan_darah_sistolik = 0; u->tekanan_darah_diastolik = 0; u->detak_jantung = 0;
        u->saturasi_oksigen = 0.0f; u->kadar_gula_darah = 0; u->berat_badan = 0.0f; u->tinggi_badan = 0;
        u->kadar_kolesterol = 0; u->trombosit = 0;
        // u->jumlah_riwayat_aktif = 0; // Dihapus karena field tidak ada di struct User
        global_jumlah_users++;
    }

    // User 2: Dokter "neronimo" (ID 10)
    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 10; strcpy(u->nama_user, "Dr. Neronimo"); strcpy(u->username, "neronimo");
        strcpy(u->password, "pass1010"); strcpy(u->role, "dokter"); strcpy(u->penyakit_utama, "");
        u->id_penyakit_terdiagnosis = 0;
        u->sudah_didiagnosis = false;
        u->jumlah_obat_di_inventory = 0;
        u->top_perut_obat = -1;    
        u->suhu_tubuh = 0.0f; u->tekanan_darah_sistolik = 0; u->tekanan_darah_diastolik = 0; u->detak_jantung = 0;
        u->saturasi_oksigen = 0.0f; u->kadar_gula_darah = 0; u->berat_badan = 0.0f; u->tinggi_badan = 0;
        u->kadar_kolesterol = 0; u->trombosit = 0;
        global_jumlah_users++;
    }
    
    // User 3: Dokter "ciciko" (ID 11)
    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 11; strcpy(u->nama_user, "Dr. Ciciko"); strcpy(u->username, "ciciko");
        strcpy(u->password, "pass1111"); strcpy(u->role, "dokter"); strcpy(u->penyakit_utama, "");
        u->id_penyakit_terdiagnosis = 0;
        u->sudah_didiagnosis = false;
        u->jumlah_obat_di_inventory = 0;
        u->top_perut_obat = -1;      
        u->suhu_tubuh = 0.0f; u->tekanan_darah_sistolik = 0; u->tekanan_darah_diastolik = 0; u->detak_jantung = 0;
        u->saturasi_oksigen = 0.0f; u->kadar_gula_darah = 0; u->berat_badan = 0.0f; u->tinggi_badan = 0;
        u->kadar_kolesterol = 0; u->trombosit = 0;
        global_jumlah_users++;
    }

    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 1; strcpy(u->nama_user, "Stewart Pasien"); strcpy(u->username, "stewart");
        strcpy(u->password, "pass11"); strcpy(u->role, "pasien"); strcpy(u->penyakit_utama, "");
        u->id_penyakit_terdiagnosis = 0;
        u->sudah_didiagnosis = false;
        u->jumlah_obat_di_inventory = 0;
        u->top_perut_obat = -1;
        u->suhu_tubuh = 36.1f; u->tekanan_darah_sistolik = 92; u->tekanan_darah_diastolik = 77; u->detak_jantung = 66;
        u->saturasi_oksigen = 93.7f; u->kadar_gula_darah = 127; u->berat_badan = 52.4f; u->tinggi_badan = 177;
        u->kadar_kolesterol = 193; u->trombosit = 328;
        global_jumlah_users++;
    }

    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 2; strcpy(u->nama_user, "Gro Pasien"); strcpy(u->username, "gro");
        strcpy(u->password, "pass22"); strcpy(u->role, "pasien"); strcpy(u->penyakit_utama, "COVID-19");
        u->id_penyakit_terdiagnosis = 0;
        u->sudah_didiagnosis = false;
        u->jumlah_obat_di_inventory = 0;
        u->top_perut_obat = -1;
        u->suhu_tubuh = 36.6f; u->tekanan_darah_sistolik = 126; u->tekanan_darah_diastolik = 85; u->detak_jantung = 67;
        u->saturasi_oksigen = 96.5f; u->kadar_gula_darah = 175; u->berat_badan = 45.7f; u->tinggi_badan = 156;
        u->kadar_kolesterol = 235; u->trombosit = 212;
        global_jumlah_users++;
    }
    
    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 3; strcpy(u->nama_user, "Kebin Pasien"); strcpy(u->username, "kebin"); 
        strcpy(u->password, "pass33"); strcpy(u->role, "pasien"); strcpy(u->penyakit_utama, "");
        u->id_penyakit_terdiagnosis = 0;
        u->sudah_didiagnosis = false; 
        u->jumlah_obat_di_inventory = 0;
        u->top_perut_obat = -1;
        u->suhu_tubuh = 36.4f; u->tekanan_darah_sistolik = 96; u->tekanan_darah_diastolik = 68; u->detak_jantung = 94;
        u->saturasi_oksigen = 92.3f; u->kadar_gula_darah = 162; u->berat_badan = 64.1f; u->tinggi_badan = 158;
        u->kadar_kolesterol = 240; u->trombosit = 380;
        global_jumlah_users++;
    }

    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 16; strcpy(u->nama_user, "Tobokan Pasien"); strcpy(u->username, "tobokan"); 
        strcpy(u->password, "pass1234"); strcpy(u->role, "pasien"); strcpy(u->penyakit_utama, "");
        u->id_penyakit_terdiagnosis = 0; 
        u->sudah_didiagnosis = false;   
        u->jumlah_obat_di_inventory = 0; 
        u->top_perut_obat = -1;  
        u->suhu_tubuh = 36.8f; u->tekanan_darah_sistolik = 114; u->tekanan_darah_diastolik = 74; u->detak_jantung = 61;
        u->saturasi_oksigen = 94.6f; u->kadar_gula_darah = 100; u->berat_badan = 80.6f; u->tinggi_badan = 173;
        u->kadar_kolesterol = 152; u->trombosit = 386;
        global_jumlah_users++;
    }
    
    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 20; strcpy(u->nama_user, "Popokan Pasien"); strcpy(u->username, "popokan"); 
        strcpy(u->password, "passpopokan"); strcpy(u->role, "pasien"); strcpy(u->penyakit_utama, "");
        u->id_penyakit_terdiagnosis = 0;
        u->sudah_didiagnosis = false; 
        u->jumlah_obat_di_inventory = 0;
        u->top_perut_obat = -1;
        u->suhu_tubuh = 36.9f; u->tekanan_darah_sistolik = 110; u->tekanan_darah_diastolik = 85; u->detak_jantung = 73;
        u->saturasi_oksigen = 98.7f; u->kadar_gula_darah = 152; u->berat_badan = 62.8f; u->tinggi_badan = 157;
        u->kadar_kolesterol = 184; u->trombosit = 390;
        global_jumlah_users++;
    }

    if (global_jumlah_users < MAX_USERS) {
        User* u = &global_users[global_jumlah_users];
        u->id_user = 4; strcpy(u->nama_user, "Pop Pasien"); strcpy(u->username, "pop");
        strcpy(u->password, "pass44"); strcpy(u->role, "pasien"); strcpy(u->penyakit_utama, "Diabetes Mellitus");
        u->id_penyakit_terdiagnosis = 0;
        u->sudah_didiagnosis = false; 
        u->jumlah_obat_di_inventory = 0;
        u->top_perut_obat = -1;
        u->suhu_tubuh = 36.9f; u->tekanan_darah_sistolik = 110; u->tekanan_darah_diastolik = 85; u->detak_jantung = 73;
        u->saturasi_oksigen = 98.7f; u->kadar_gula_darah = 152; u->berat_badan = 62.8f; u->tinggi_badan = 157;
        u->kadar_kolesterol = 184; u->trombosit = 390; 
        global_jumlah_users++;
    }


    int max_id_val = 0;
    if (global_jumlah_users > 0) { 
        for (int i = 0; i < global_jumlah_users; ++i) {
            if (global_users[i].id_user > max_id_val) {
                max_id_val = global_users[i].id_user;
            }
        }
        g_next_user_id = max_id_val + 1;
    } else {
        g_next_user_id = 1; 
    }
}


void initialize_all_data() {
    initialize_users();
    initialize_penyakit_db();
    initialize_obat_data();
    printf("Data hardcode (users, penyakit, obat) telah diinisialisasi.\n");
}