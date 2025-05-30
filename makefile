# Makefile untuk Proyek Nimons Hospital

# Compiler
CC = gcc

# Flags untuk compiler
# -Wall: Tampilkan semua warning
# -g: Sertakan debugging information
# -I./src/h: Beritahu compiler untuk mencari header files di direktori ./src/h
CFLAGS = -Wall -g -I./src/header

# Nama direktori
# SRC_C_DIR untuk file .c di dalam src/c/
# SRC_H_DIR untuk file .h di dalam src/h/ (digunakan di CFLAGS)
# MAIN_C adalah file main.c di root
SRC_C_DIR = src/c
OBJ_DIR = obj
BIN_DIR = bin

# File main.c di root
MAIN_C_SOURCE = main.c
MAIN_C_OBJECT = $(OBJ_DIR)/main.o

# Daftar semua file source .c di direktori src/c/
# Pastikan semua file .c Anda (selain main.c) ada di direktori src/c/ dan tercantum di sini
OTHER_C_SOURCES = $(SRC_C_DIR)/admin_functions.c \
				  $(SRC_C_DIR)/auth_functions.c \
                  $(SRC_C_DIR)/command_handler.c \
                  $(SRC_C_DIR)/data_manager.c \
                  $(SRC_C_DIR)/denah_functions.c \
                  $(SRC_C_DIR)/doctor_functions.c \
                  $(SRC_C_DIR)/help_functions.c \
                  $(SRC_C_DIR)/patient_functions.c \
                  $(SRC_C_DIR)/session_manager.c \
                  $(SRC_C_DIR)/user_management_functions.c \
                  $(SRC_C_DIR)/user_manager.c \
                  $(SRC_C_DIR)/utils.c

# Daftar file object .o yang akan dibuat di direktori obj/ untuk file .c di src/c/
OTHER_C_OBJECTS = $(OTHER_C_SOURCES:$(SRC_C_DIR)/%.c=$(OBJ_DIR)/%.o)

# Gabungkan semua file object
OBJECTS = $(MAIN_C_OBJECT) $(OTHER_C_OBJECTS)

# Nama file executable yang akan dibuat di direktori bin/
EXECUTABLE = $(BIN_DIR)/nimons_hospital

# Target default: build executable
all: $(EXECUTABLE)

# Aturan untuk membuat direktori jika belum ada
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Aturan untuk membuat file executable dari file object
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking..."
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)
	@echo "Build selesai. Executable ada di: $(EXECUTABLE)"

# Aturan untuk membuat file object .o dari main.c (yang ada di root)
$(MAIN_C_OBJECT): $(MAIN_C_SOURCE) | $(OBJ_DIR)
	@echo "Compiling $(MAIN_C_SOURCE) ..."
	$(CC) $(CFLAGS) -c $(MAIN_C_SOURCE) -o $(MAIN_C_OBJECT)

# Aturan untuk membuat file object .o dari file source .c di dalam src/c/
$(OBJ_DIR)/%.o: $(SRC_C_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $< ..."
	$(CC) $(CFLAGS) -c $< -o $@

# Target untuk menjalankan program
# Menggunakan argumen default 5 untuk jumlah ruangan. Ubah sesuai kebutuhan.
run: $(EXECUTABLE)
	@echo "Menjalankan program dengan 5 ruangan..."
	./$(EXECUTABLE) 5

# Target untuk membersihkan file hasil kompilasi
clean:
	@echo "Membersihkan build files..."
	rm -f $(OBJ_DIR)/*.o
	rm -f $(EXECUTABLE)
	@echo "Selesai membersihkan."
	@-rmdir $(OBJ_DIR) 2>/dev/null || true
	@-rmdir $(BIN_DIR) 2>/dev/null || true

# Phony targets (target yang bukan nama file sebenarnya)
.PHONY: all clean run
