#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>   


#include "definitions.h"
#include "user_interface.h"
#include "command_handler.h"
#include "data_manager.h"
#include "session_manager.h" 

    bool is_all_whitespace(const char *str);

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Jumlah ruangan STI tidak valid (harus antara 1 dan %d).\n", MAX_RUANGAN_STI);
        return 1;
    }

    
    jumlah_ruangan_aktual_sti = atoi(argv[1]);
    if (jumlah_ruangan_aktual_sti < 1 || jumlah_ruangan_aktual_sti > MAX_RUANGAN_STI) {
        printf("Jumlah ruangan STI tidak valid (harus antara 1 dan %d).\n", MAX_RUANGAN_STI);
        return 1;
    }

  
    initialize_all_data();     
    initialize_denah_sti();    
    initialize_command_metadata();

    char input_buffer[MAX_INPUT_BUFFER];
    bool running = true;                 

    while (running) {
        display_main_prompt(); 
        get_user_input("", input_buffer, MAX_INPUT_BUFFER); 

        char command_to_check[MAX_INPUT_BUFFER];
        strcpy(command_to_check, input_buffer);
        int start_idx = 0;
        while (isspace((unsigned char)command_to_check[start_idx])) {
            start_idx++;
        }
        int end_idx = strlen(command_to_check) - 1;
        while (end_idx >= start_idx && isspace((unsigned char)command_to_check[end_idx])) {
            command_to_check[end_idx--] = '\0';
        }
        for (int k = start_idx; command_to_check[k]; k++) {
            command_to_check[k] = toupper((unsigned char)command_to_check[k]);
        }

        if (strcmp(command_to_check + start_idx, "EXIT") == 0) { 
        const CommandMetadata* cmd_exit_meta = get_command_metadata_from_string("EXIT"); 
        if (cmd_exit_meta && has_permission_for_command(cmd_exit_meta)) {


          
            process_user_input("EXIT"); 
                         

            running = false;
        } else {
             display_error("Anda tidak memiliki izin untuk keluar saat ini (seharusnya tidak terjadi).");
        }
    } else if (strlen(input_buffer + start_idx) > 0 && !is_all_whitespace(input_buffer + start_idx)) { 
        process_user_input(input_buffer); 
    }
       
    }
    return 0; //
}




    bool is_all_whitespace(const char *str) {
        while (*str) {
            if (!isspace((unsigned char)*str)) {
                return false;
            }
            str++;
        }
        return true;
    }
