
#include "command_handler.h"
#include "definitions.h"
#include "user_interface.h"
#include "auth_functions.h"
#include "session_manager.h"
#include "help_functions.h"  
#include "denah_functions.h" 
#include "user_management_functions.h" 
#include "admin_functions.h" 
#include "doctor_functions.h"
#include "patient_functions.h"
#include <ctype.h> 

CommandMetadata command_metadata_table[MAX_COMMANDS];
int command_metadata_count = 0;

void initialize_command_metadata() {
    int idx = 0;

    // F01: LOGIN
    strcpy(command_metadata_table[idx].string_command, "LOGIN");
    command_metadata_table[idx].type = COMMAND_LOGIN;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_GUEST;
    command_metadata_table[idx].permission.allowed_roles[1] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.allowed_roles[2] = ROLE_DOKTER;
    command_metadata_table[idx].permission.allowed_roles[3] = ROLE_PASIEN;
    command_metadata_table[idx].permission.count = 4;
    idx++;

    // F03 Logout 
    strcpy(command_metadata_table[idx].string_command, "LOGOUT");
    command_metadata_table[idx].type = COMMAND_LOGOUT;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.allowed_roles[1] = ROLE_DOKTER;
    command_metadata_table[idx].permission.allowed_roles[2] = ROLE_PASIEN;
    command_metadata_table[idx].permission.count = 3;
    idx++;

    // F02 Register Pasien
    strcpy(command_metadata_table[idx].string_command, "REGISTRASI"); 
    command_metadata_table[idx].type = COMMAND_REGISTRASI;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_GUEST; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F04: LUPA_PASSWORD
    strcpy(command_metadata_table[idx].string_command, "LUPA_PASSWORD");
    command_metadata_table[idx].type = COMMAND_LUPA_PASSWORD;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_GUEST;
    command_metadata_table[idx].permission.allowed_roles[1] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.allowed_roles[2] = ROLE_DOKTER;  
    command_metadata_table[idx].permission.allowed_roles[3] = ROLE_PASIEN;
    command_metadata_table[idx].permission.count = 4; 
    idx++;

    // F18 Exit
    strcpy(command_metadata_table[idx].string_command, "KELUAR"); 
    command_metadata_table[idx].type = COMMAND_KELUAR;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER;
    command_metadata_table[idx].permission.allowed_roles[1] = ROLE_DOKTER;
    command_metadata_table[idx].permission.allowed_roles[2] = ROLE_PASIEN;
    command_metadata_table[idx].permission.allowed_roles[3] = ROLE_GUEST; 
    command_metadata_table[idx].permission.count = 4;
    idx++;

    // F05 HELP
    strcpy(command_metadata_table[idx].string_command, "HELP");
    command_metadata_table[idx].type = COMMAND_HELP;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER;
    command_metadata_table[idx].permission.allowed_roles[1] = ROLE_DOKTER;
    command_metadata_table[idx].permission.allowed_roles[2] = ROLE_PASIEN;
    command_metadata_table[idx].permission.allowed_roles[3] = ROLE_GUEST;
    command_metadata_table[idx].permission.count = 4;
    idx++;

    // S01 (F06 STI) - LIHAT_DENAH
    strcpy(command_metadata_table[idx].string_command, "LIHAT_DENAH");
    command_metadata_table[idx].type = COMMAND_LIHAT_DENAH;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.allowed_roles[1] = ROLE_DOKTER;  
    command_metadata_table[idx].permission.allowed_roles[2] = ROLE_PASIEN;  
    command_metadata_table[idx].permission.count = 3;
    idx++;

    // S01 (F06 STI) - LIHAT_RUANGAN
    strcpy(command_metadata_table[idx].string_command, "LIHAT_RUANGAN"); 
    command_metadata_table[idx].type = COMMAND_LIHAT_RUANGAN;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.allowed_roles[1] = ROLE_DOKTER;  
    command_metadata_table[idx].permission.allowed_roles[2] = ROLE_PASIEN;  
    command_metadata_table[idx].permission.count = 3;
    idx++;

    // F07 - LIHAT_USER
    strcpy(command_metadata_table[idx].string_command, "LIHAT_USER");
    command_metadata_table[idx].type = COMMAND_LIHAT_USER;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F07 - LIHAT_PASIEN
    strcpy(command_metadata_table[idx].string_command, "LIHAT_PASIEN");
    command_metadata_table[idx].type = COMMAND_LIHAT_PASIEN;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F07 - LIHAT_DOKTER
    strcpy(command_metadata_table[idx].string_command, "LIHAT_DOKTER");
    command_metadata_table[idx].type = COMMAND_LIHAT_DOKTER;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F08 - CARI_USER
    strcpy(command_metadata_table[idx].string_command, "CARI_USER");
    command_metadata_table[idx].type = COMMAND_CARI_USER; 
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F08 - CARI_PASIEN
    strcpy(command_metadata_table[idx].string_command, "CARI_PASIEN");
    command_metadata_table[idx].type = COMMAND_CARI_PASIEN; 
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F08 - CARI_DOKTER
    strcpy(command_metadata_table[idx].string_command, "CARI_DOKTER");
    command_metadata_table[idx].type = COMMAND_CARI_DOKTER; 
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.count = 1;
    idx++;
    
    // S02 (F09 STI) - LIHAT_SEMUA_ANTRIAN
    strcpy(command_metadata_table[idx].string_command, "LIHAT_SEMUA_ANTRIAN"); 
    command_metadata_table[idx].type = COMMAND_LIHAT_SEMUA_ANTRIAN;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F10 - TAMBAH_DOKTER
    strcpy(command_metadata_table[idx].string_command, "TAMBAH_DOKTER");
    command_metadata_table[idx].type = COMMAND_TAMBAH_DOKTER;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER;
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F10 - ASSIGN_DOKTER
    strcpy(command_metadata_table[idx].string_command, "ASSIGN_DOKTER");
    command_metadata_table[idx].type = COMMAND_ASSIGN_DOKTER;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER;
    command_metadata_table[idx].permission.count = 1;
    idx++;

    // F11 - DIAGNOSIS
    strcpy(command_metadata_table[idx].string_command, "DIAGNOSIS");
    command_metadata_table[idx].type = COMMAND_DIAGNOSIS;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_DOKTER;
    command_metadata_table[idx].permission.count = 1;
    idx++;

    strcpy(command_metadata_table[idx].string_command, "NGOBATIN");
    command_metadata_table[idx].type = COMMAND_NGOBATIN;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_DOKTER;
    command_metadata_table[idx].permission.count = 1;
    idx++;

    strcpy(command_metadata_table[idx].string_command, "PULANGDOK");
    command_metadata_table[idx].type = COMMAND_PULANGDOK;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_PASIEN;
    command_metadata_table[idx].permission.count = 1;
    idx++;

    strcpy(command_metadata_table[idx].string_command, "DAFTAR_CHECKUP");
    command_metadata_table[idx].type = COMMAND_DAFTAR_CHECKUP;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_PASIEN; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    strcpy(command_metadata_table[idx].string_command, "ANTRIAN");
    command_metadata_table[idx].type = COMMAND_ANTRIAN;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_PASIEN; 
    command_metadata_table[idx].permission.count = 1;
    idx++;

    strcpy(command_metadata_table[idx].string_command, "MINUM_OBAT");
    command_metadata_table[idx].type = COMMAND_MINUM_OBAT;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_PASIEN;
    command_metadata_table[idx].permission.count = 1;
    idx++;

    strcpy(command_metadata_table[idx].string_command, "PENAWAR");
    command_metadata_table[idx].type = COMMAND_PENAWAR;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_PASIEN;
    command_metadata_table[idx].permission.count = 1;
    idx++;

    strcpy(command_metadata_table[idx].string_command, "EXIT"); 
    command_metadata_table[idx].type = COMMAND_KELUAR;
    command_metadata_table[idx].permission.allowed_roles[0] = ROLE_MANAGER;
    command_metadata_table[idx].permission.allowed_roles[1] = ROLE_DOKTER;
    command_metadata_table[idx].permission.allowed_roles[2] = ROLE_PASIEN;
    command_metadata_table[idx].permission.allowed_roles[3] = ROLE_GUEST; 
    command_metadata_table[idx].permission.count = 4;
    idx++;

    command_metadata_count = idx;
}

const CommandMetadata* get_command_metadata_from_string(const char* input_string) {
    char upper_input[MAX_COMMAND_STRING_LEN];
    char upper_table_cmd[MAX_COMMAND_STRING_LEN];
    int i_input = 0;
    int k = 0;

    while (isspace((unsigned char)input_string[i_input])) {
        i_input++;
    }

    while (input_string[i_input] != '\0' && !isspace((unsigned char)input_string[i_input]) && k < MAX_COMMAND_STRING_LEN - 1) {
        upper_input[k] = toupper((unsigned char)input_string[i_input]);
        k++;
        i_input++;
    }
    upper_input[k] = '\0';

    if (strlen(upper_input) == 0) {
        return NULL;
    }

    for (int i = 0; i < command_metadata_count; i++) {
        strcpy(upper_table_cmd, command_metadata_table[i].string_command);
        for(int j = 0; upper_table_cmd[j]; j++) {
            upper_table_cmd[j] = toupper((unsigned char)upper_table_cmd[j]);
        }

        if (strcmp(upper_input, upper_table_cmd) == 0) {
            return &command_metadata_table[i];
        }
    }

    return NULL;
}

bool has_permission_for_command(const CommandMetadata* cmd_metadata) {
    if (cmd_metadata == NULL) return false;

    UserRoleType current_user_role_type;
    if (!isLoggedIn) {
        current_user_role_type = ROLE_GUEST;
    } else {
        if (strcmp(currentUser.role, "manager") == 0) { 
            current_user_role_type = ROLE_MANAGER;
        } else if (strcmp(currentUser.role, "dokter") == 0) {
            current_user_role_type = ROLE_DOKTER;
        } else if (strcmp(currentUser.role, "pasien") == 0) {
            current_user_role_type = ROLE_PASIEN;
        } else {
            return false; 
        }
    }

    for (int i = 0; i < cmd_metadata->permission.count; i++) {
        if (cmd_metadata->permission.allowed_roles[i] == current_user_role_type) {
            return true;
        }
    }
    return false;
}

void process_user_input(const char* input_string_raw) {
    char input_string[MAX_INPUT_BUFFER];
    int start = 0;
    
    // Trim leading whitespace
    while (isspace((unsigned char)input_string_raw[start])) {
        start++;
    }
    strcpy(input_string, input_string_raw + start);
    
    // Trim trailing whitespace
    int end = strlen(input_string) - 1;
    while (end >= 0 && isspace((unsigned char)input_string[end])) {
        input_string[end--] = '\0';
    }

    if (strlen(input_string) == 0) { 
        return; 
    }

    const CommandMetadata* cmd_meta = get_command_metadata_from_string(input_string);

    if (cmd_meta == NULL) {
        display_error("Perintah tidak dikenal. Ketik 'HELP' untuk daftar perintah."); 
        return;
    }

    // Validasi login status
    if (cmd_meta->type == COMMAND_LOGIN && isLoggedIn) {
        display_error("Anda sudah login. Silakan logout terlebih dahulu jika ingin login dengan akun lain.");
        return;
    }
    if (cmd_meta->type == COMMAND_REGISTRASI && isLoggedIn) { 
        display_error("Anda sudah login. Registrasi hanya untuk pengguna yang belum memiliki akun.");
        return;
    }
    if (cmd_meta->type == COMMAND_LOGOUT && !isLoggedIn) { 
        display_error("Logout gagal!");
        display_message("Anda belum login, silahkan login terlebih dahulu sebelum melakukan logout");
        return;
    }
    
    // Check if command requires login
    bool requires_login = true;
    if (cmd_meta->type == COMMAND_LOGIN || 
        cmd_meta->type == COMMAND_REGISTRASI || 
        cmd_meta->type == COMMAND_HELP || 
        cmd_meta->type == COMMAND_KELUAR ||
        cmd_meta->type == COMMAND_LUPA_PASSWORD) { 
        requires_login = false;
    }

    if (requires_login && !isLoggedIn) {
        display_error("Anda harus login untuk menggunakan perintah ini.");
        return;
    }

    if (!has_permission_for_command(cmd_meta)) { 
        display_error("Anda tidak memiliki izin untuk menjalankan perintah ini."); 
        return;
    }

    // Handle commands
    switch (cmd_meta->type) { 
        case COMMAND_LOGIN:         handle_login(); break; 
        case COMMAND_LOGOUT:        handle_logout(); break; 
        case COMMAND_REGISTRASI:    handle_register_user(); break; 
        case COMMAND_LUPA_PASSWORD: handle_lupa_password(); break; 
        case COMMAND_KELUAR: 
            display_message("Terima kasih telah menggunakan sistem Nimons. Sampai jumpa!"); 
            break;
        case COMMAND_HELP: 
            display_help_menu(); 
            break;             
        case COMMAND_LIHAT_DENAH:
            handle_lihat_denah_sti();
            break;
        case COMMAND_LIHAT_RUANGAN:
            handle_lihat_ruangan_sti(); 
            break;
        case COMMAND_LIHAT_USER:
            handle_lihat_user();
            break;
        case COMMAND_LIHAT_PASIEN:
            handle_lihat_pasien();
            break;
        case COMMAND_LIHAT_DOKTER:
            handle_lihat_dokter();
            break;
        case COMMAND_CARI_USER:
            handle_cari_user();
            break;
        case COMMAND_CARI_PASIEN:
            handle_cari_pasien();
            break;
        case COMMAND_CARI_DOKTER:
            handle_cari_dokter();
            break;
        case COMMAND_LIHAT_SEMUA_ANTRIAN: 
            handle_lihat_semua_antrian_sti();
            break;
        case COMMAND_TAMBAH_DOKTER: 
            handle_tambah_dokter();
            break;
        case COMMAND_ASSIGN_DOKTER: 
            handle_assign_dokter();
            break;
        case COMMAND_DIAGNOSIS:
            handle_diagnosis();
            break;
        case COMMAND_NGOBATIN:
            handle_ngobatin();
            break;
        case COMMAND_PULANGDOK: 
            handle_pulangdok(); 
            break;
        case COMMAND_DAFTAR_CHECKUP:
            handle_daftar_checkup(); 
            break;
        case COMMAND_ANTRIAN:
            handle_antrian_saya();
            break;
        case COMMAND_MINUM_OBAT:
            handle_minum_obat();
            break;
        case COMMAND_PENAWAR:
            handle_minum_penawar();
            break;
        case COMMAND_EXIT:
            display_message("Terima kasih telah menggunakan sistem Nimons. Sampai jumpa!");
            break;
        default:
            display_error("Handler untuk perintah ini belum diimplementasikan."); 
            break;
    }
}