#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "system.h"
#include "string.h"
#include <stdint.h>

#define INPUT_BUFFER_SIZE 128

static char input_buffer[INPUT_BUFFER_SIZE]; // buffer to hold user input
static uint8_t buffer_len = 0;

void shell_print_prompt(){
    print(">>");
}

void shell_handle_char(char c){
    if (c == '\b') { // backspace
        if (buffer_len > 0) {
            buffer_len--;
            input_buffer[buffer_len] = '\0';
            print("\b \b");
        }
        return;
    } 
    if (c == '\n') { // enter
        print("\n");
        input_buffer[buffer_len] = '\0';
        shell_execute(input_buffer);
        buffer_len = 0;
        shell_print_prompt();
        return;
    }

    // add char to buffer
    if (buffer_len < INPUT_BUFFER_SIZE - 1) {
        input_buffer[buffer_len++] = c;
        char str[2] = {c, '\0'};
        print(str);
    }
}

void shell_execute(char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        print(">Commands:\n");
        print(" help     -  list commands\n");
        print(" clear    -  clear screen\n");
        print(" echo     -  print text on screen\n");
        print(" about    -  info about OS\n");
        print(" reboot   -  restart system\n");
        print(" shutdown -  power off system\n");
        return;
    }

    if (strcmp(cmd, "clear") == 0) {
        clear_screen();
        return;
    }
    if (strncmp(cmd, "echo ", 5) == 0) {
        print(cmd + 5);
        print("\n");
        return;
    }
    if (strcmp(cmd, "about") == 0) {
        print("\nShemaOS v1.0\n");
        print("Developed by ShemaBruno\n");
        print("Features: interrupts, VGA text mode, keyboard driver, shell\n");
        return;
    }
    if (strcmp(cmd, "reboot") == 0) {
        print("\nRebooting system...\n");
        reboot();
        return;
    }
    if (strcmp(cmd, "shutdown") == 0) {
        shut_down();
        return;
    }

    print("\nUnknown command. Type <help> for commands.\n");
}

void shell_run(){
    clear_screen();
    print("----------------------------------\n");
    print("-*-*-*-*Welcome to ShemaOS*-*-*-*-\n");
    print("----------------------------------\n \n");
    print("Type <help> for commands\n");

    shell_print_prompt();

}

