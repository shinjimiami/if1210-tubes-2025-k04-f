
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "definitions.h" //

void initialize_command_metadata();
const CommandMetadata* get_command_metadata_from_string(const char* input_string);
bool has_permission_for_command(const CommandMetadata* cmd_metadata); 
void process_user_input(const char* input_command_string); 

#endif // COMMAND_HANDLER_H