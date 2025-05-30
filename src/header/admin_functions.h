#ifndef ADMIN_FUNCTIONS_H
#define ADMIN_FUNCTIONS_H

#include "definitions.h"

// Deklarasi fungsi untuk F10
void handle_tambah_dokter();
void handle_assign_dokter();

// Fungsi helper untuk validasi username unik (case-insensitive)
// Ini bisa juga diletakkan di auth_functions.h jika dipakai di sana juga (misal untuk F02)
// Untuk sekarang, kita buat spesifik untuk kebutuhan admin di sini.
bool is_username_unique_case_insensitive(const char* username_baru);

#endif // ADMIN_FUNCTIONS_H
