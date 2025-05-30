#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// --- Konstanta Global ---
#define MAX_USERS 50
#define MAX_RIWAYAT_PENYAKIT_PER_PASIEN 10
#define MAX_NAMA_USER_LEN 100
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_ROLE_LEN 10      
#define MAX_INPUT_BUFFER 256
#define MAX_RLE_CODE_LEN (MAX_USERNAME_LEN * 2 + 1) 
#define MAX_COMMAND_STRING_LEN 50
#define MAX_ROLES_PER_PERMISSION 4 
#define MAX_COMMANDS 35            
#define MAX_RUANGAN_STI 20 
#define ID_DOKTER_KOSONG 0 
#define MAX_NAMA_PENYAKIT_LEN 50
#define MAX_STRING 100
#define MAX_PASIEN_PER_RUANGAN_STI 10 
#define MAX_PASIEN_ANTRIAN_LUAR_STI 10 
#define MAX_PENYAKIT_DB 10
#define MAX_NAMA_OBAT_LEN 50
#define MAX_OBAT 20
#define MAX_OBAT_PENYAKIT_RELATIONS 50
#define MAX_OBAT_DI_PERUT_PASIEN 10
#define MAX_OBAT_DI_INVENTORY_PASIEN 20
#define MAX_PASIEN_PER_RUANGAN_STI 10 
#define MAX_PASIEN_ANTRIAN_LUAR_STI 10 



// --- Enum untuk Tipe Role Pengguna ---
typedef enum {
    ROLE_GUEST,
    ROLE_PASIEN,
    ROLE_DOKTER,
    ROLE_MANAGER
} UserRoleType;

// --- Enum untuk Tipe Perintah ---
typedef enum {
    COMMAND_UNKNOWN = 0,
    COMMAND_LOGIN,
    COMMAND_LOGOUT,
    COMMAND_REGISTRASI,
    COMMAND_LUPA_PASSWORD,     
    COMMAND_KELUAR,
    COMMAND_HELP,              
    COMMAND_LIHAT_DENAH,       
    COMMAND_LIHAT_RUANGAN,     
    COMMAND_LIHAT_USER,        
    COMMAND_LIHAT_PASIEN,      
    COMMAND_LIHAT_DOKTER,      
    COMMAND_CARI_USER,         
    COMMAND_CARI_PASIEN,       
    COMMAND_CARI_DOKTER,       
    COMMAND_LIHAT_SEMUA_ANTRIAN, 
    COMMAND_TAMBAH_DOKTER,     
    COMMAND_ASSIGN_DOKTER,     
    COMMAND_DIAGNOSIS,
    COMMAND_NGOBATIN,
    COMMAND_PULANGDOK,
    COMMAND_DAFTAR_CHECKUP,
    COMMAND_ANTRIAN,
    COMMAND_MINUM_OBAT,
    COMMAND_PENAWAR, 
    COMMAND_MAX_VALUE,
    COMMAND_EXIT 
} CommandEnumType;

// --- Struktur Data untuk Permission ---
typedef struct {
    UserRoleType allowed_roles[MAX_ROLES_PER_PERMISSION];
    int count;
} CommandPermission;

// --- Struktur Data untuk Metadata Perintah ---
typedef struct {
    CommandEnumType type;
    char string_command[MAX_COMMAND_STRING_LEN];
    CommandPermission permission;
} CommandMetadata;

// --- Definisi Struktur Data Utama ---
typedef struct {
    int id_penyakit;
} RiwayatPenyakitEntry;

typedef struct {
    int id_obat;
} ItemObatPasien;

typedef struct {
    int id_user;
    char nama_user[MAX_NAMA_USER_LEN];
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char role[MAX_ROLE_LEN]; 

    char penyakit_utama[MAX_NAMA_PENYAKIT_LEN];
    int id_penyakit_terdiagnosis;
    bool sudah_didiagnosis;
    ItemObatPasien inventory_obat[MAX_OBAT_DI_INVENTORY_PASIEN];
    int jumlah_obat_di_inventory;

    ItemObatPasien perut_obat[MAX_OBAT_DI_PERUT_PASIEN];
    int top_perut_obat;
    float suhu_tubuh;
    int tekanan_darah_sistolik;
    int tekanan_darah_diastolik;
    int detak_jantung;
    float saturasi_oksigen;
    int kadar_gula_darah;
    float berat_badan;
    int tinggi_badan;
    int kadar_kolesterol;
    int trombosit;
} User;

typedef struct {
    int id_ruangan;
    int id_dokter_bertugas; 
    int daftar_id_pasien_di_ruangan[MAX_PASIEN_PER_RUANGAN_STI]; 
    int jumlah_pasien_di_ruangan;
    int daftar_id_pasien_antrian_luar[MAX_PASIEN_ANTRIAN_LUAR_STI]; 
    int jumlah_pasien_antrian_luar;
} RuanganSTI;

typedef struct {
    int id;
    char nama_penyakit[MAX_NAMA_PENYAKIT_LEN];
    float suhu_tubuh_min;
    float suhu_tubuh_max;
    int tekanan_darah_sistolik_min;
    int tekanan_darah_sistolik_max;
    int tekanan_darah_diastolik_min;
    int tekanan_darah_diastolik_max;
    int detak_jantung_min;
    int detak_jantung_max;
    float saturasi_oksigen_min;
    float saturasi_oksigen_max;
    int kadar_gula_darah_min;
    int kadar_gula_darah_max;
    float berat_badan_min; 
    float berat_badan_max;
    int tinggi_badan_min;
    int tinggi_badan_max;
    int kadar_kolesterol_min;
    int kadar_kolesterol_max;
    int trombosit_min;
    int trombosit_max;
} Penyakit;

typedef struct {
    int id_obat;
    char nama_obat[MAX_NAMA_OBAT_LEN];
} Obat;

typedef struct {
    int id_penyakit;
    int id_obat;
    int urutan_minum;
} ObatPenyakitRelasi;

// --- Global Session Variable ---
extern User currentUser;
extern bool isLoggedIn;

// --- Global Data Denah STI ---
extern RuanganSTI denah_rs_sti[MAX_RUANGAN_STI];
extern int jumlah_ruangan_aktual_sti; 
extern int kapasitas_maks_pasien_per_ruangan_sti; 
extern int kapasitas_maks_antrian_luar_sti; 

// --- Global Data Arrays ---
extern User global_users[MAX_USERS];
extern int global_jumlah_users;
extern int g_next_user_id;

// --- Global Database Penyakit (Hardcoded untuk STI) ---
extern Penyakit global_penyakit_db[MAX_PENYAKIT_DB];
extern int global_jumlah_penyakit;

// BARU: Global Data Obat (didefinisikan di data_manager.c)
extern Obat global_obat_list[MAX_OBAT];
extern int global_jumlah_obat;
extern ObatPenyakitRelasi global_obat_penyakit_list[MAX_OBAT_PENYAKIT_RELATIONS];
extern int global_jumlah_obat_penyakit;


// --- Tabel Metadata Perintah ---
extern CommandMetadata command_metadata_table[MAX_COMMANDS];
extern int command_metadata_count;

#endif // DEFINITIONS_H
