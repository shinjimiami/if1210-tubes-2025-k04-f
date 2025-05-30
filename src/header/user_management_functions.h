#ifndef USER_MANAGEMENT_FUNCTIONS_H
#define USER_MANAGEMENT_FUNCTIONS_H

#include "definitions.h"
#include <stdbool.h>

// Fungsi-fungsi pembanding untuk sorting
int compare_user_by_id_asc_f07(const void* a, const void* b);
int compare_user_by_id_desc_f07(const void* a, const void* b);
int compare_user_by_name_asc_f07(const void* a, const void* b);
int compare_user_by_name_desc_f07(const void* a, const void* b);
int strcasecmp_custom_f07(const char *s1, const char *s2);

// Fungsi-fungsi handler untuk perintah
void handle_lihat_user();
void handle_lihat_pasien();
void handle_lihat_dokter();
void handle_cari_user();
void handle_cari_pasien();
void handle_cari_dokter();

// Fungsi-fungsi utilitas
bool get_sort_options_f07(int* sort_by_choice, int* sort_order_choice);
void display_user_data(User* user_array, int count, const char* display_type);
void display_single_user_detail(User* user);

#endif