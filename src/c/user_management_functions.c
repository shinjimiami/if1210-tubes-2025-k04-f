
#include "user_management_functions.h"
#include "user_interface.h" 
#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

User sorted_users[MAX_USERS]; 
int sorted_users_count;

int compare_user_by_id_asc_f07(const void* a, const void* b) {
    User* userA = (User*)a;
    User* userB = (User*)b;
    return (userA->id_user - userB->id_user);
}
int compare_user_by_id_desc_f07(const void* a, const void* b) {
    User* userA = (User*)a;
    User* userB = (User*)b;
    return (userB->id_user - userA->id_user);
}

int strcasecmp_custom_f07(const char *s1, const char *s2) {
    while (*s1 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

int compare_user_by_name_asc_f07(const void* a, const void* b) {
    User* userA = (User*)a;
    User* userB = (User*)b;
    return strcasecmp_custom_f07(userA->nama_user, userB->nama_user);
}
int compare_user_by_name_desc_f07(const void* a, const void* b) {
    User* userA = (User*)a;
    User* userB = (User*)b;
    return strcasecmp_custom_f07(userB->nama_user, userA->nama_user);
}

bool get_sort_options_f07(int* sort_by_choice, int* sort_order_choice) {
    char choice_buffer[10];

    display_message("Urutkan berdasarkan?");
    display_message("1. ID");
    display_message("2. Nama");
    get_user_input(">>> Pilihan: ", choice_buffer, 10);
    *sort_by_choice = atoi(choice_buffer);
    if (*sort_by_choice != 1 && *sort_by_choice != 2) {
        display_error("Pilihan pengurutan tidak valid.");
        return false;
    }

    display_message("Urutan sort?");
    display_message("1. ASC (A-Z atau terkecil)");
    display_message("2. DESC (Z-A atau terbesar)");
    get_user_input(">>> Pilihan: ", choice_buffer, 10);
    *sort_order_choice = atoi(choice_buffer);
    if (*sort_order_choice != 1 && *sort_order_choice != 2) {
        display_error("Pilihan urutan sort tidak valid.");
        return false;
    }
    return true;
}


void display_user_data(User* user_array, int count, const char* display_type) {
    const int id_width = 5;
    const int name_width = 20; 
    const int role_width = 10; 
    const int penyakit_width = 25; 

    if (strcmp(display_type, "LIHAT_USER") == 0) {
        printf("%-*s | %-*s | %-*s | %-*s\n", id_width, "ID", name_width, "Nama", role_width, "Role", penyakit_width, "Penyakit");
        printf("-----+----------------------+------------+--------------------------\n"); 
        for (int i = 0; i < count; i++) {
            printf("%-*d | %-*s | %-*s | %-*s\n",
                   id_width, user_array[i].id_user,
                   name_width, user_array[i].nama_user,
                   role_width, user_array[i].role,
                   penyakit_width, (strcmp(user_array[i].role, "pasien") == 0 ? user_array[i].penyakit_utama : ""));
        }
    } else if (strcmp(display_type, "LIHAT_PASIEN") == 0) {
        printf("%-*s | %-*s | %-*s\n", id_width, "ID", name_width, "Nama", penyakit_width, "Penyakit");
        printf("-----+----------------------+--------------------------\n");
        for (int i = 0; i < count; i++) {
             if (strcmp(user_array[i].role, "pasien") == 0) {
                printf("%-*d | %-*s | %-*s\n",
                       id_width, user_array[i].id_user,
                       name_width, user_array[i].nama_user,
                       penyakit_width, user_array[i].penyakit_utama);
            }
        }
    } else if (strcmp(display_type, "LIHAT_DOKTER") == 0) {
        printf("%-*s | %-*s\n", id_width, "ID", name_width, "Nama");
        printf("-----+----------------------\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(user_array[i].role, "dokter") == 0) {
                printf("%-*d | %-*s\n",
                       id_width, user_array[i].id_user,
                       name_width, user_array[i].nama_user);
            }
        }
    }
}

void perform_lihat(const char* command_type) {
    int sort_by_choice, sort_order_choice;
    if (!get_sort_options_f07(&sort_by_choice, &sort_order_choice)) {
        return;
    }

    sorted_users_count = 0;
    if (strcmp(command_type, "LIHAT_USER") == 0) {
        for (int i = 0; i < global_jumlah_users; i++) {
            sorted_users[sorted_users_count++] = global_users[i]; 
        }
        printf("Menampilkan semua pengguna dengan ");
    } else if (strcmp(command_type, "LIHAT_PASIEN") == 0) {
        for (int i = 0; i < global_jumlah_users; i++) { 
            if (strcmp(global_users[i].role, "pasien") == 0) { 
                sorted_users[sorted_users_count++] = global_users[i];
            }
        }
        printf("Menampilkan pasien dengan ");
    } else if (strcmp(command_type, "LIHAT_DOKTER") == 0) {
        for (int i = 0; i < global_jumlah_users; i++) { 
            if (strcmp(global_users[i].role, "dokter") == 0) { 
                sorted_users[sorted_users_count++] = global_users[i];
            }
        }
        printf("Menampilkan dokter dengan ");
    } else {
        return; 
    }
    
    if (sorted_users_count == 0) {
        display_message("Tidak ada data pengguna untuk ditampilkan.");
        return;
    }

    if (sort_by_choice == 1) { 
        printf("ID terurut ");
        qsort(sorted_users, sorted_users_count, sizeof(User),
              (sort_order_choice == 1) ? compare_user_by_id_asc_f07 : compare_user_by_id_desc_f07);
    } else { 
        printf("nama terurut ");
        qsort(sorted_users, sorted_users_count, sizeof(User),
              (sort_order_choice == 1) ? compare_user_by_name_asc_f07 : compare_user_by_name_desc_f07);
    }
    printf("%s...\n", (sort_order_choice == 1) ? "ascending" : "descending");

    display_user_data(sorted_users, sorted_users_count, command_type);
}


void handle_lihat_user() {
    perform_lihat("LIHAT_USER");
}

void handle_lihat_pasien() {
    perform_lihat("LIHAT_PASIEN");
}

void handle_lihat_dokter() {
    perform_lihat("LIHAT_DOKTER");
}


User* binary_search_user_by_id(User* user_array, int count, int target_id) {
    int left = 0;
    int right = count - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (user_array[mid].id_user == target_id) {
            return &user_array[mid];
        }
        if (user_array[mid].id_user < target_id) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return NULL; 
}

User* sequential_search_user_by_name(User* user_array, int count, const char* target_name) {
    for (int i = 0; i < count; i++) {
        if (strcasecmp_custom_f07(user_array[i].nama_user, target_name) == 0) { 
            return &user_array[i];
        }
    }
    return NULL; 
}

void display_single_user_detail(User* user) {
    if (user == NULL) return;

    printf("\n--- Detail Pengguna Ditemukan ---\n");
    if (strcmp(user->role, "pasien") == 0) {
        printf("ID       : %d\n", user->id_user);
        printf("Nama     : %s\n", user->nama_user);
        printf("Role     : %s\n", user->role);
        printf("Penyakit : %s\n", strlen(user->penyakit_utama) > 0 ? user->penyakit_utama : "-");
    } else { 
        printf("ID       : %d\n", user->id_user);
        printf("Nama     : %s\n", user->nama_user);
        printf("Role     : %s\n", user->role);
    }
    printf("---------------------------------\n");
}


void handle_cari_user() {
    char choice_buffer[10];
    char search_term_buffer[MAX_NAMA_USER_LEN]; 

    display_message("Cari pengguna (Dokter/Pasien) berdasarkan?");
    display_message("1. ID");
    display_message("2. Nama");
    get_user_input(">>> Pilihan: ", choice_buffer, 10); 
    int search_by_choice = atoi(choice_buffer);

    User* found_user = NULL;

    if (search_by_choice == 1) { 
        get_user_input("Masukkan nomor ID user: ", search_term_buffer, 10);
        int target_id = atoi(search_term_buffer);

        User temp_users_for_search[MAX_USERS]; 
        int user_count_for_search = 0;
        for (int i = 0; i < global_jumlah_users; i++) { 
             temp_users_for_search[user_count_for_search++] = global_users[i];
        }
        qsort(temp_users_for_search, user_count_for_search, sizeof(User), compare_user_by_id_asc_f07); 

        printf("Mencari pengguna dengan nomor ID %d...\n", target_id);
        found_user = binary_search_user_by_id(temp_users_for_search, user_count_for_search, target_id);

    } else if (search_by_choice == 2) { 
        get_user_input("Masukkan nama user: ", search_term_buffer, MAX_NAMA_USER_LEN); 
        printf("Mencari pengguna dengan nama '%s'...\n", search_term_buffer);
        found_user = sequential_search_user_by_name(global_users, global_jumlah_users, search_term_buffer); 

    } else {
        display_error("Pilihan pencarian tidak valid."); 
        return;
    }

    if (found_user != NULL) {
        display_single_user_detail(found_user);
    } else {
        if (search_by_choice == 1) {
             printf("Tidak ditemukan pengguna dengan nomor ID tersebut!\n");
        } else {
             printf("Tidak ditemukan pengguna dengan nama '%s'!\n", search_term_buffer);
        }
    }
}


void handle_cari_pasien() {
    char choice_buffer[10];
    char search_term_buffer[MAX_NAMA_PENYAKIT_LEN]; 

    display_message("Cari pasien berdasarkan?");
    display_message("1. ID");
    display_message("2. Nama");
    display_message("3. Penyakit");
    get_user_input(">>> Pilihan: ", choice_buffer, 10); 
    int search_by_choice = atoi(choice_buffer);

    User found_patients[MAX_USERS]; 
    int found_patients_count = 0;
    User* single_found_patient = NULL;

    if (search_by_choice == 1) { 
        get_user_input("Masukkan nomor ID pasien: ", search_term_buffer, 10);
        int target_id = atoi(search_term_buffer);

        User temp_patients_for_search[MAX_USERS]; 
        int patient_count_for_search = 0;
        for(int i=0; i < global_jumlah_users; ++i){ 
            if(strcmp(global_users[i].role, "pasien") == 0){ 
                temp_patients_for_search[patient_count_for_search++] = global_users[i];
            }
        }
        qsort(temp_patients_for_search, patient_count_for_search, sizeof(User), compare_user_by_id_asc_f07);

        printf("Mencari pasien dengan nomor ID %d...\n", target_id);
        single_found_patient = binary_search_user_by_id(temp_patients_for_search, patient_count_for_search, target_id);
        if (single_found_patient != NULL && strcmp(single_found_patient->role, "pasien") == 0) { 
            display_single_user_detail(single_found_patient);
        } else {
            printf("Tidak ditemukan pasien dengan nomor ID tersebut!\n");
        }

    } else if (search_by_choice == 2) { 
        get_user_input("Masukkan nama pasien: ", search_term_buffer, MAX_NAMA_USER_LEN); 
        printf("Mencari pasien dengan nama '%s'...\n", search_term_buffer);
        for (int i = 0; i < global_jumlah_users; i++) { 
            if (strcmp(global_users[i].role, "pasien") == 0 && strcasecmp_custom_f07(global_users[i].nama_user, search_term_buffer) == 0) { 
                single_found_patient = &global_users[i];
                break;
            }
        }
        if (single_found_patient != NULL) {
            display_single_user_detail(single_found_patient);
        } else {
            printf("Tidak ditemukan pasien dengan nama '%s'!\n", search_term_buffer);
        }

    } else if (search_by_choice == 3) { 
        get_user_input("Masukkan nama penyakit: ", search_term_buffer, MAX_NAMA_PENYAKIT_LEN); 
        printf("Mencari pasien dengan penyakit '%s'...\n", search_term_buffer);
        for (int i = 0; i < global_jumlah_users; i++) { 
            if (strcmp(global_users[i].role, "pasien") == 0 && strcasecmp_custom_f07(global_users[i].penyakit_utama, search_term_buffer) == 0) { 
                if (found_patients_count < MAX_USERS) { 
                    found_patients[found_patients_count++] = global_users[i];
                }
            }
        }

        if (found_patients_count > 0) {
            int sort_by_sub_choice, sort_order_sub_choice;
            display_message("Ditemukan beberapa pasien. Urutkan hasil berdasarkan?");
            if (!get_sort_options_f07(&sort_by_sub_choice, &sort_order_sub_choice)) {
                return;
            }

            int (*compare_func)(const void*, const void*) = NULL;
            if (sort_by_sub_choice == 1) { 
                compare_func = (sort_order_sub_choice == 1) ? compare_user_by_id_asc_f07 : compare_user_by_id_desc_f07;
            } else { 
                compare_func = (sort_order_sub_choice == 1) ? compare_user_by_name_asc_f07 : compare_user_by_name_desc_f07;
            }
            qsort(found_patients, found_patients_count, sizeof(User), compare_func);

            printf("Menampilkan pasien dengan penyakit '%s' dengan %s terurut %s...\n",
                search_term_buffer,
                (sort_by_sub_choice == 1 ? "ID" : "Nama"),
                (sort_order_sub_choice == 1 ? "ascending" : "descending"));
            // PERBAIKAN: Menggunakan display_user_data bukan display_sorted_user_data
            display_user_data(found_patients, found_patients_count, "LIHAT_PASIEN");

        } else {
            printf("Tidak ditemukan pasien dengan penyakit '%s'!\n", search_term_buffer);
        }
    } else {
        display_error("Pilihan pencarian tidak valid."); 
    }
}

void handle_cari_dokter() {
    char choice_buffer[10];
    char search_term_buffer[MAX_NAMA_USER_LEN]; 

    display_message("Cari dokter berdasarkan?");
    display_message("1. ID");
    display_message("2. Nama");
    get_user_input(">>> Pilihan: ", choice_buffer, 10); 
    int search_by_choice = atoi(choice_buffer);

    User* found_dokter = NULL;

    if (search_by_choice == 1) { 
        get_user_input("Masukkan nomor ID dokter: ", search_term_buffer, 10);
        int target_id = atoi(search_term_buffer);

        User temp_dokters_for_search[MAX_USERS]; 
        int dokter_count_for_search = 0;
        for(int i=0; i < global_jumlah_users; ++i){ 
            if(strcmp(global_users[i].role, "dokter") == 0){ 
                temp_dokters_for_search[dokter_count_for_search++] = global_users[i];
            }
        }
        qsort(temp_dokters_for_search, dokter_count_for_search, sizeof(User), compare_user_by_id_asc_f07);

        printf("Mencari dokter dengan nomor ID %d...\n", target_id);
        found_dokter = binary_search_user_by_id(temp_dokters_for_search, dokter_count_for_search, target_id);
         if (found_dokter == NULL || strcmp(found_dokter->role, "dokter") != 0) { 
            found_dokter = NULL; 
        }

    } else if (search_by_choice == 2) { 
        get_user_input("Masukkan nama dokter: ", search_term_buffer, MAX_NAMA_USER_LEN); 
        printf("Mencari dokter dengan nama '%s'...\n", search_term_buffer);
        for (int i = 0; i < global_jumlah_users; i++) { 
            if (strcmp(global_users[i].role, "dokter") == 0 && strcasecmp_custom_f07(global_users[i].nama_user, search_term_buffer) == 0) { 
                found_dokter = &global_users[i];
                break;
            }
        }
    } else {
        display_error("Pilihan pencarian tidak valid."); 
        return;
    }

    if (found_dokter != NULL) {
        display_single_user_detail(found_dokter);
    } else {
         if (search_by_choice == 1) {
             printf("Tidak ditemukan dokter dengan nomor ID tersebut!\n");
        } else {
             printf("Tidak ditemukan dokter dengan nama '%s'!\n", search_term_buffer);
        }
    }
}
