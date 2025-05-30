
#include "patient_functions.h"
#include "user_interface.h"  
#include "session_manager.h"  
#include "data_manager.h"   
#include "definitions.h" 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <float.h>
#include "patient_functions.h" 

// --- SEMUA FUNGSI HELPER  ---
static bool is_perut_kosong_f13(User* pasien) {
    return pasien->top_perut_obat == -1;
}


static const char* get_nama_obat_by_id_f13(int id_obat) {
    for (int i = 0; i < global_jumlah_obat; i++) { 
        if (global_obat_list[i].id_obat == id_obat) { 
            return global_obat_list[i].nama_obat; 
        }
    }
    return "Obat Tidak Dikenal";
}

static User* get_user_by_id_f13(int user_id) __attribute__((unused));
static User* get_user_by_id_f13(int user_id) {
    for (int i = 0; i < global_jumlah_users; i++) {
        if (global_users[i].id_user == user_id) { 
            return &global_users[i];
        }
    }
    return NULL;
}
static int get_user_idx_by_id_f13(int user_id) __attribute__((unused));
static int get_user_idx_by_id_f13(int user_id) {
     for (int i = 0; i < global_jumlah_users; i++) {
        if (global_users[i].id_user == user_id) {
            return i;
        }
    }
    return -1;
}

static void remove_pasien_from_list_by_index_f13(int* list_pasien_id, int* jumlah_pasien, int index_to_remove) {
    if (index_to_remove < 0 || index_to_remove >= *jumlah_pasien) {
        return;
    }
    for (int i = index_to_remove; i < (*jumlah_pasien) - 1; i++) {
        list_pasien_id[i] = list_pasien_id[i + 1];
    }
    (*jumlah_pasien)--;
}

static int compare_obat_urutan(const void* a, const void* b) {
    ObatPenyakitRelasi* relasiA = (ObatPenyakitRelasi*)a;
    ObatPenyakitRelasi* relasiB = (ObatPenyakitRelasi*)b;
    return (relasiA->urutan_minum - relasiB->urutan_minum);
}

static User* get_user_by_id_f14(int user_id) {
    for (int i = 0; i < global_jumlah_users; i++) {
        if (global_users[i].id_user == user_id) {
            return &global_users[i];
        }
    }
    return NULL;
}

static int get_user_idx_by_id_f14(int user_id) {
    for (int i = 0; i < global_jumlah_users; i++) {
        if (global_users[i].id_user == user_id) {
            return i;
        }
    }
    return -1;
}

bool is_pasien_sudah_di_antrian(int id_pasien) {
    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) {
        RuanganSTI* ruangan = &denah_rs_sti[i];
        for (int j = 0; j < ruangan->jumlah_pasien_di_ruangan; j++) {
            if (ruangan->daftar_id_pasien_di_ruangan[j] == id_pasien) return true;
        }
        for (int j = 0; j < ruangan->jumlah_pasien_antrian_luar; j++) {
            if (ruangan->daftar_id_pasien_antrian_luar[j] == id_pasien) return true;
        }
    }
    return false;
}

float get_valid_float_input(const char* prompt, float min_val, float max_val) {
    char buffer[MAX_INPUT_BUFFER];
    float value;
    while (1) {
        get_user_input(prompt, buffer, MAX_INPUT_BUFFER);
        value = atof(buffer);
        if (value >= min_val && value <= max_val) {
            return value;
        }
        printf("Input tidak valid. Masukkan angka antara %.1f dan %.1f.\n", min_val, max_val);
    }
}

int get_valid_int_input(const char* prompt, int min_val, int max_val) {
    char buffer[MAX_INPUT_BUFFER];
    int value;
    long long long_val;
    char* endptr;
    while (1) {
        get_user_input(prompt, buffer, MAX_INPUT_BUFFER);
        long_val = strtoll(buffer, &endptr, 10);
        bool conversion_ok = (endptr != buffer && *endptr == '\0');
        if (conversion_ok && long_val >= min_val && long_val <= max_val) {
            value = (int)long_val;
            return value;
        }
        printf("Input tidak valid. Masukkan angka bulat antara %d dan %d.\n", min_val, max_val);
    }
}

static bool is_perut_penuh_f16(User* pasien) {
    if (pasien == NULL) return true;
    return pasien->top_perut_obat >= MAX_OBAT_DI_PERUT_PASIEN - 1; //
}

/**
 * @brief
 * @param pasien_global_idx
 * @param obat_diminum 
 * @return
 */
static bool push_obat_ke_perut_f16(int pasien_global_idx, ItemObatPasien obat_diminum) { 
    if (pasien_global_idx < 0 || pasien_global_idx >= global_jumlah_users) {
        display_error("Error internal: Index pasien tidak valid saat push obat ke perut."); 
        return false;
    }
    User* pasien = &global_users[pasien_global_idx]; //

    if (is_perut_penuh_f16(pasien)) {
        display_error("Perut sudah penuh, tidak bisa minum obat lagi!");
        return false;
    }

    pasien->top_perut_obat++;
    pasien->perut_obat[pasien->top_perut_obat] = obat_diminum;
    return true;
}

static ItemObatPasien pop_obat_dari_perut_f17(int pasien_global_idx) {
    ItemObatPasien obat_dikeluarkan = {0};
    if (pasien_global_idx < 0 || pasien_global_idx >= global_jumlah_users) {
        display_error("Error internal: Index pasien tidak valid saat pop obat dari perut."); 
        return obat_dikeluarkan;
    }
    User* pasien = &global_users[pasien_global_idx]; //

    if (is_perut_kosong_f13(pasien)) {
        return obat_dikeluarkan;
    }

    obat_dikeluarkan = pasien->perut_obat[pasien->top_perut_obat];
    pasien->perut_obat[pasien->top_perut_obat].id_obat = 0;
    pasien->top_perut_obat--;
    return obat_dikeluarkan;
}

/**
 * @brief 
 * @param pasien_global_idx
 * @param obat_ditambahkan
 * @return
 */
static bool add_obat_ke_inventory_f17(int pasien_global_idx, ItemObatPasien obat_ditambahkan) { 
    if (pasien_global_idx < 0 || pasien_global_idx >= global_jumlah_users) { 
         display_error("Error internal: Index pasien tidak valid saat tambah obat ke inventory.");
        return false;
    }
    User* pasien = &global_users[pasien_global_idx]; //

    if (pasien->jumlah_obat_di_inventory >= MAX_OBAT_DI_INVENTORY_PASIEN) { 
        display_error("Inventory obat penuh, tidak bisa menambahkan obat lagi!"); 
        printf("Sayang sekali, obat %s tidak bisa dikembalikan ke inventory karena penuh.\n", get_nama_obat_by_id_f13(obat_ditambahkan.id_obat)); // Menggunakan helper dari F13
        return false;
    }

    pasien->inventory_obat[pasien->jumlah_obat_di_inventory] = obat_ditambahkan; 
    pasien->jumlah_obat_di_inventory++; 
    return true;
}


// --- Implementasi F17 - Minum Penawar ---
void handle_minum_penawar() {
    if (strcmp(currentUser.role, "pasien") != 0) { 
        display_error("Perintah ini hanya untuk Pasien."); 
        return;
    }

    User* pasien_login_ptr = get_current_user_data(); 
    if (pasien_login_ptr == NULL) {
        display_error("Gagal mendapatkan data pasien saat ini."); 
        return;
    }
    int pasien_global_idx = get_user_idx_by_id_f14(currentUser.id_user);
     if (pasien_global_idx == -1) {
        display_error("Error internal: Data pasien tidak sinkron."); 
        return;
    }
    User* pasien = &global_users[pasien_global_idx]; 


    if (is_perut_kosong_f13(pasien)) { 
        display_message("Perut kosong!! Belum ada obat yang dimakan."); 
        return;
    }

    // Keluarkan obat terakhir dari perut
    ItemObatPasien obat_dikeluarkan = pop_obat_dari_perut_f17(pasien_global_idx);

    if (obat_dikeluarkan.id_obat != 0) { // Jika berhasil pop
        const char* nama_obat_dikeluarkan = get_nama_obat_by_id_f13(obat_dikeluarkan.id_obat); 
        if (add_obat_ke_inventory_f17(pasien_global_idx, obat_dikeluarkan)) {
            printf("Uwekkk!!! %s keluar dan kembali ke inventory\n", nama_obat_dikeluarkan);
        } else {
        }
    } else {
        display_error("Gagal mengeluarkan obat dari perut."); 
    }
}

void handle_antrian_saya() { 
    if (strcmp(currentUser.role, "pasien") != 0) { 
        display_error("Perintah ini hanya untuk Pasien."); 
        return;
    }

    int id_pasien_login = currentUser.id_user;
    bool ditemukan_di_antrian_luar = false;
    bool ditemukan_di_dalam_ruangan = false;
    int posisi_antrian = 0;
    int total_antrian_luar_di_ruangan_itu = 0;
    User* dokter_bertugas = NULL;
    int id_ruangan_pasien = -1;

    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) { 
        RuanganSTI* ruangan = &denah_rs_sti[i]; 

        for (int j = 0; j < ruangan->jumlah_pasien_di_ruangan; j++) { 
            if (ruangan->daftar_id_pasien_di_ruangan[j] == id_pasien_login) {
                ditemukan_di_dalam_ruangan = true;
                dokter_bertugas = get_user_by_id_f14(ruangan->id_dokter_bertugas); 
                id_ruangan_pasien = ruangan->id_ruangan; 
                break; 
            }
        }
        if (ditemukan_di_dalam_ruangan) break;
        for (int j = 0; j < ruangan->jumlah_pasien_antrian_luar; j++) { 
            if (ruangan->daftar_id_pasien_antrian_luar[j] == id_pasien_login) { 
                ditemukan_di_antrian_luar = true;
                posisi_antrian = j + 1;
                total_antrian_luar_di_ruangan_itu = ruangan->jumlah_pasien_antrian_luar;
                dokter_bertugas = get_user_by_id_f14(ruangan->id_dokter_bertugas); 
                id_ruangan_pasien = ruangan->id_ruangan;
                break;
            }
        }
        if (ditemukan_di_antrian_luar) break;
    }

    if (ditemukan_di_dalam_ruangan) {
        printf("Anda sedang berada di dalam ruangan dokter!\n");
        if (dokter_bertugas != NULL) {
            printf("Dokter   : Dr. %s\n", dokter_bertugas->nama_user);
            printf("Ruangan  : %d\n", id_ruangan_pasien);
        }
    } else if (ditemukan_di_antrian_luar) {
        printf("Status antrian Anda:\n");
        if (dokter_bertugas != NULL) {
            printf("Dokter   : Dr. %s\n", dokter_bertugas->nama_user); //
        } else {
            printf("Dokter   : (Tidak ada info dokter untuk ruangan ini)\n");
        }
        printf("Ruangan  : %d\n", id_ruangan_pasien);

        printf("Posisi antrian (luar ruangan): %d dari %d orang.\n", posisi_antrian, total_antrian_luar_di_ruangan_itu);
    
    } else {
        display_message("Anda belum terdaftar dalam antrian check-up!");
        display_message("Silakan daftar terlebih dahulu dengan command DAFTAR_CHECKUP.");
    }
}


void handle_daftar_checkup() {
    if (strcmp(currentUser.role, "pasien") != 0) {
        display_error("Perintah ini hanya untuk Pasien."); 
        return;
    }

    User* pasien_login_ptr = get_current_user_data(); 
    if (pasien_login_ptr == NULL) {
        display_error("Gagal mendapatkan data pasien saat ini."); 
        return;
    }
    int pasien_global_idx = get_user_idx_by_id_f14(currentUser.id_user);
     if (pasien_global_idx == -1) {
        display_error("Error internal: Data pasien tidak sinkron.");
        return;
    }

    if (is_pasien_sudah_di_antrian(currentUser.id_user)) { 
        display_message("Anda sudah terdaftar dalam antrian check-up!"); 
        display_message("Silakan selesaikan check-up yang sudah terdaftar terlebih dahulu."); 
        return;
    }

    printf("Silakan masukkan data check-up Anda:\n");
    global_users[pasien_global_idx].suhu_tubuh = get_valid_float_input("Suhu Tubuh (Celcius): ", 30.0f, 45.0f); 

    char tekanan_buffer[20];
    int sistolik, diastolik;
    while(1) {
        get_user_input("Tekanan Darah (sistol/diastol, contoh 120 80): ", tekanan_buffer, 20); 
        if (sscanf(tekanan_buffer, "%d %d", &sistolik, &diastolik) == 2) {
            if (sistolik > 0 && diastolik > 0 && sistolik > diastolik && sistolik < 300 && diastolik < 200) {
                global_users[pasien_global_idx].tekanan_darah_sistolik = sistolik; 
                global_users[pasien_global_idx].tekanan_darah_diastolik = diastolik; 
                break;
            }
        }
        display_error("Format tekanan darah tidak valid atau nilai tidak masuk akal. Contoh: 120 80"); 
    }

    global_users[pasien_global_idx].detak_jantung = get_valid_int_input("Detak Jantung (bpm): ", 30, 250); 
    global_users[pasien_global_idx].saturasi_oksigen = get_valid_float_input("Saturasi Oksigen (%): ", 70.0f, 100.0f); 
    global_users[pasien_global_idx].kadar_gula_darah = get_valid_int_input("Kadar Gula Darah (mg/dL): ", 30, 700); 
    global_users[pasien_global_idx].berat_badan = get_valid_float_input("Berat Badan (kg): ", 1.0f, 500.0f); 
    global_users[pasien_global_idx].tinggi_badan = get_valid_int_input("Tinggi Badan (cm): ", 30, 300); 
    global_users[pasien_global_idx].kadar_kolesterol = get_valid_int_input("Kadar Kolestrol (mg/dL): ", 50, 600); 
    global_users[pasien_global_idx].trombosit = get_valid_int_input("Trombosit (ribu/mikroL): ", 10, 1000); 

    global_users[pasien_global_idx].penyakit_utama[0] = '\0'; 
    global_users[pasien_global_idx].id_penyakit_terdiagnosis = 0; 
    global_users[pasien_global_idx].sudah_didiagnosis = false; 

    printf("\nBerikut adalah daftar dokter yang tersedia:\n");
    int dokter_tersedia_idx[MAX_USERS];
    int count_dokter_tersedia = 0;

    for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) { 
        if (denah_rs_sti[i].id_dokter_bertugas != ID_DOKTER_KOSONG) { 
            User* dokter = get_user_by_id_f14(denah_rs_sti[i].id_dokter_bertugas); 
            if (dokter != NULL && strcmp(dokter->role, "dokter") == 0) {
                if (denah_rs_sti[i].jumlah_pasien_antrian_luar < MAX_PASIEN_ANTRIAN_LUAR_STI) { 
                    printf("%d. Dr. %s - Ruangan %d (Antrian Luar: %d orang)\n",
                           count_dokter_tersedia + 1,
                           dokter->nama_user, 
                           denah_rs_sti[i].id_ruangan, 
                           denah_rs_sti[i].jumlah_pasien_antrian_luar); 
                    dokter_tersedia_idx[count_dokter_tersedia] = i;
                    count_dokter_tersedia++;
                }
            }
        }
    }

    if (count_dokter_tersedia == 0) {
        display_message("Maaf, tidak ada dokter yang tersedia atau semua antrian penuh saat ini."); 
        return;
    }

    char pilihan_dokter_buffer[10];
    int pilihan_dokter_int;
    while(1) {
        get_user_input("Pilih dokter (nomor): ", pilihan_dokter_buffer, 10); 
        pilihan_dokter_int = atoi(pilihan_dokter_buffer);
        if (pilihan_dokter_int > 0 && pilihan_dokter_int <= count_dokter_tersedia) {
            break;
        }
        display_error("Pilihan dokter tidak valid."); 
    }

    int index_ruangan_dipilih = dokter_tersedia_idx[pilihan_dokter_int - 1];
    RuanganSTI* ruangan_dipilih = &denah_rs_sti[index_ruangan_dipilih]; 

    ruangan_dipilih->daftar_id_pasien_antrian_luar[ruangan_dipilih->jumlah_pasien_antrian_luar] = currentUser.id_user; 
    ruangan_dipilih->jumlah_pasien_antrian_luar++;
    User* dokter_dipilih_obj = get_user_by_id_f14(ruangan_dipilih->id_dokter_bertugas); 

    display_message("Pendaftaran check-up berhasil!"); 
    if(dokter_dipilih_obj != NULL) {
         printf("Anda terdaftar pada antrian Dr. %s di ruangan %d.\n", dokter_dipilih_obj->nama_user, ruangan_dipilih->id_ruangan); 
    } else {
         printf("Anda terdaftar pada antrian di ruangan %d.\n", ruangan_dipilih->id_ruangan); 
    }
    printf("Posisi antrian Anda (di luar ruangan): %d dari %d kapasitas.\n", ruangan_dipilih->jumlah_pasien_antrian_luar, kapasitas_maks_antrian_luar_sti); 
}

void handle_pulangdok() {
    if (strcmp(currentUser.role, "pasien") != 0) { 
        display_error("Perintah ini hanya untuk Pasien.");
        return;
    }
    // Ambil data pasien yang sedang login
    User* pasien_login = get_current_user_data();
    if (pasien_login == NULL) {
        display_error("Data pasien tidak ditemukan. Silakan login ulang.");
        return;
    }
    // Cek apakah pasien sudah login
    int pasien_global_idx = get_user_idx_by_id_f13(pasien_login->id_user);
    if (pasien_global_idx == -1) {
         display_error("Error internal: data pasien tidak sinkron.");
        return;
    }
    // Cek apakah pasien sudah didiagnosis
    if (!global_users[pasien_global_idx].sudah_didiagnosis) { 
        display_message("Kamu belum menerima diagnosis apapun dari dokter, jangan buru-buru pulang!");
        return;
    }
    // Cek apakah pasien sudah didiagnosis dengan penyakit
    if (global_users[pasien_global_idx].id_penyakit_terdiagnosis == 0 && global_users[pasien_global_idx].sudah_didiagnosis) { 
        display_message("Dokter sedang memeriksa keadaanmu..");
        display_message("Kamu sudah dinyatakan sehat sebelumnya dan seharusnya sudah bisa pulang.");
        return;
    }
    // Cek apakah pasien sudah minum obat sesuai resep
    ObatPenyakitRelasi resep_seharusnya[MAX_OBAT_PENYAKIT_RELATIONS]; 
    int jumlah_obat_diresep_seharusnya = 0;
    for (int i = 0; i < global_jumlah_obat_penyakit; i++) { 
        if (global_obat_penyakit_list[i].id_penyakit == global_users[pasien_global_idx].id_penyakit_terdiagnosis) { 
            if (jumlah_obat_diresep_seharusnya < MAX_OBAT_PENYAKIT_RELATIONS) { 
                resep_seharusnya[jumlah_obat_diresep_seharusnya++] = global_obat_penyakit_list[i]; 
            }
        }
    }
    // Jika tidak ada obat yang diresepkan, pasien bisa pulang
    if (jumlah_obat_diresep_seharusnya > 0) {
        qsort(resep_seharusnya, jumlah_obat_diresep_seharusnya, sizeof(ObatPenyakitRelasi), compare_obat_urutan); 
    }

    // Kasus 2: Pasien belum menghabiskan seluruh obat
    int jumlah_obat_diminum = global_users[pasien_global_idx].top_perut_obat + 1; //
    if (jumlah_obat_diminum < jumlah_obat_diresep_seharusnya) {
        display_message("Dokter sedang memeriksa keadaanmu...");
        display_message("Masih ada obat yang belum kamu habiskan, minum semuanya dulu yukk!");
        return;
    }

    // Kasus 3 & 4: Pasien sudah menghabiskan obat (jumlahnya sama), cek urutan
    bool urutan_benar = true;
    if (jumlah_obat_diminum == jumlah_obat_diresep_seharusnya && jumlah_obat_diminum > 0) {
        for (int i = 0; i < jumlah_obat_diminum; i++) {

            if (global_users[pasien_global_idx].perut_obat[i].id_obat != resep_seharusnya[i].id_obat) {
                urutan_benar = false;
                break;
            }
        }
    } else if (jumlah_obat_diminum > 0 && jumlah_obat_diminum != jumlah_obat_diresep_seharusnya) {
        urutan_benar = false;
    }


    display_message("Dokter sedang memeriksa keadaanmu...");
    if (!urutan_benar || jumlah_obat_diminum != jumlah_obat_diresep_seharusnya) { 
        if (jumlah_obat_diminum == 0 && jumlah_obat_diresep_seharusnya > 0) {
             display_message("Masih ada obat yang belum kamu habiskan, minum semuanya dulu yukk!");
             return;
        }

        display_message("Maaf, tapi kamu masih belum bisa pulang!");
        printf("Urutan peminuman obat yang diharapkan:\n");
        for (int i = 0; i < jumlah_obat_diresep_seharusnya; i++) {
            printf("  %s", get_nama_obat_by_id_f13(resep_seharusnya[i].id_obat));
            if (i < jumlah_obat_diresep_seharusnya - 1) printf(" -> ");
        }
        printf("\n");

        printf("Urutan obat yang kamu minum:\n");
        if (is_perut_kosong_f13(&global_users[pasien_global_idx])) { 
            printf("  (Kamu belum minum obat apapun)\n");
        } else {
            for (int i = 0; i <= global_users[pasien_global_idx].top_perut_obat; i++) { 
                printf("  %s", get_nama_obat_by_id_f13(global_users[pasien_global_idx].perut_obat[i].id_obat)); 
                if (i < global_users[pasien_global_idx].top_perut_obat) printf(" -> "); 
            }
            printf("\n");
        }
        display_message("Silahkan kunjungi dokter untuk meminta penawar yang sesuai !");
    } else {
        display_message("Selamat! Kamu sudah dinyatakan sembuh oleh dokter.");
        display_message("Silahkan pulang dan semoga sehat selalu!");

        strcpy(global_users[pasien_global_idx].penyakit_utama, ""); 
        global_users[pasien_global_idx].id_penyakit_terdiagnosis = 0; 
        global_users[pasien_global_idx].sudah_didiagnosis = false; 
        global_users[pasien_global_idx].jumlah_obat_di_inventory = 0; 
        global_users[pasien_global_idx].top_perut_obat = -1; 
        bool pasien_dikeluarkan = false;
        for (int i = 0; i < jumlah_ruangan_aktual_sti; i++) { 
            RuanganSTI* ruangan = &denah_rs_sti[i];
            // Cek di dalam ruanga
            for (int j = 0; j < ruangan->jumlah_pasien_di_ruangan; j++) { 
                if (ruangan->daftar_id_pasien_di_ruangan[j] == pasien_login->id_user) { 
                    remove_pasien_from_list_by_index_f13(ruangan->daftar_id_pasien_di_ruangan, &ruangan->jumlah_pasien_di_ruangan, j); //
                    printf("Pasien %s telah meninggalkan ruangan %d.\n", pasien_login->nama_user, ruangan->id_ruangan); //
                    pasien_dikeluarkan = true;
                    break;
                }
            }
            if (pasien_dikeluarkan) break;
    
        }
        if (!pasien_dikeluarkan) {
            // display_message("Catatan: Pasien tidak ditemukan di antrian/ruangan manapun untuk dikeluarkan.");
        }
    }
}

static ItemObatPasien remove_obat_from_inventory_by_index(int pasien_global_idx, int index_obat_di_inventory) {
    ItemObatPasien obat_dihapus = {0}; // Obat kosong jika gagal
    if (pasien_global_idx < 0 || pasien_global_idx >= global_jumlah_users) return obat_dihapus;
    User* pasien = &global_users[pasien_global_idx];

    if (index_obat_di_inventory < 0 || index_obat_di_inventory >= pasien->jumlah_obat_di_inventory) {
        return obat_dihapus; // Index tidak valid
    }

    obat_dihapus = pasien->inventory_obat[index_obat_di_inventory];

    // Geser elemen lain untuk mengisi celah
    for (int i = index_obat_di_inventory; i < pasien->jumlah_obat_di_inventory - 1; i++) {
        pasien->inventory_obat[i] = pasien->inventory_obat[i + 1];
    }
    pasien->jumlah_obat_di_inventory--;
    return obat_dihapus;
}

void handle_minum_obat() { // F16
    if (strcmp(currentUser.role, "pasien") != 0) {
        display_error("Perintah ini hanya untuk Pasien.");
        return;
    }

    User* pasien_login_ptr = get_current_user_data();
    if (pasien_login_ptr == NULL) {
        display_error("Gagal mendapatkan data pasien saat ini.");
        return;
    }
    int pasien_global_idx = get_user_idx_by_id_f14(currentUser.id_user);
    if (pasien_global_idx == -1) {
        display_error("Error internal: Data pasien tidak sinkron.");
        return;
    }

    User* pasien = &global_users[pasien_global_idx];

    if (pasien->jumlah_obat_di_inventory == 0) {
        display_message("Inventory obat Anda kosong. Tidak ada obat untuk diminum.");
        return;
    }

    printf("====== DAFTAR OBAT DI INVENTORY ANDA ======\n");
    for (int i = 0; i < pasien->jumlah_obat_di_inventory; i++) {
        const char* nama_obat = get_nama_obat_by_id_f13(pasien->inventory_obat[i].id_obat);
        printf("%d. %s\n", i + 1, nama_obat);
    }
    printf("==========================================\n");

    char pilihan_buffer[10];
    get_user_input("Pilih obat untuk diminum (nomor): ", pilihan_buffer, 10);
    int pilihan_int = atoi(pilihan_buffer);

    if (pilihan_int > 0 && pilihan_int <= pasien->jumlah_obat_di_inventory) {
        int index_obat_dipilih = pilihan_int - 1;

        ItemObatPasien obat_berhasil_dihapus_dari_inv = remove_obat_from_inventory_by_index(pasien_global_idx, index_obat_dipilih);
        
        if (obat_berhasil_dihapus_dari_inv.id_obat != 0) { 
            if (push_obat_ke_perut_f16(pasien_global_idx, obat_berhasil_dihapus_dari_inv)) {
                
                const char* nama_obat_diminum = get_nama_obat_by_id_f13(obat_berhasil_dihapus_dari_inv.id_obat);
                printf("GLEKGLEKGLEK... %s berhasil diminum!!!\n", nama_obat_diminum);
            } else {
                // Gagal push ke perut (perut penuh), pesan error sudah ditampilkan dari push_obat_ke_perut_f16
                // Jika ingin mengembalikan ke inventory, perlu fungsi add_obat_ke_inventory.
                // Untuk saat ini, kita anggap jika perut penuh, obat tidak jadi diminum dan tetap hilang dari inventory
                // (atau, Anda bisa mengimplementasikan add_obat_ke_inventory(pasien_global_idx, obat_berhasil_dihapus_dari_inv);)
                // display_message("Obat tidak jadi diminum karena perut penuh."); // Pesan ini sudah ada di push_obat_ke_perut_f16
            }
        } else {
             display_error("Gagal mengambil obat dari inventory.");
        }
    } else { 
        display_error("Pilihan nomor tidak tersedia!");
    }

    printf("\n====== DAFTAR OBAT SAAT INI DI INVENTORY ======\n");
    if (pasien->jumlah_obat_di_inventory == 0) {
        display_message("(Inventory obat Anda kosong)");
    } else {
        for (int i = 0; i < pasien->jumlah_obat_di_inventory; i++) {
            const char* nama_obat = get_nama_obat_by_id_f13(pasien->inventory_obat[i].id_obat);
            printf("%d. %s\n", i + 1, nama_obat);
        }
    }
    printf("===========================================\n");
}