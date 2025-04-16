#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define COM1 0x3F8
#define VIDEO_MEMORY ((volatile uint16_t*)0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_CMD_LEN 128
#define MOUSE_PORT 0x60
#define MOUSE_CTRL 0x64
#define PIT_FREQUENCY 1193182
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND 0x43
#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

#define COLOR_BLACK   0x0
#define COLOR_BLUE    0x1
#define COLOR_GREEN   0x2
#define COLOR_CYAN    0x3
#define COLOR_RED     0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN   0x6
#define COLOR_GRAY    0x7
#define COLOR_WHITE   0xF

#define KEY_ENTER     0x1C
#define KEY_BACKSPACE 0x0E
#define KEY_ESC       0x01
#define KEY_UP        0x48
#define KEY_DOWN      0x50
#define KEY_LEFT      0x4B
#define KEY_RIGHT     0x4D
#define KEY_LSHIFT    0x2A
#define KEY_RSHIFT    0x36
#define KEY_CTRL      0x1D
#define KEY_ALT       0x38
#define KEY_CAPSLOCK  0x3A

#define MOUSE_ENABLE  0xA8
#define MOUSE_WRITE   0xD4
#define MOUSE_INIT    0xF4

void move_cursor(size_t x, size_t y);
void terminal_initialize();
void terminal_clear();
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void terminal_setcolor(uint8_t fg, uint8_t bg);
void terminal_write(const char* data, size_t size);
void terminal_scroll();
uint8_t cmos_read(uint8_t reg);
int bcd_to_bin(int bcd);
char keyboard_getchar();
void init_timer();
void mouse_wait(uint8_t type);
void mouse_write(uint8_t data);
uint8_t mouse_read();
void mouse_init();
void kernel_panic();
void execute_fino();
void execute_color(char* color);
void execute_memory();
void execute_disk();
void execute_ls();
void execute_pwd();
void execute_echo(char* args[], int arg_count);
void execute_date();
void execute_whoami();
void execute_uptime();
void execute_kptest();
void execute_poweroff();
void execute_reboot();
void execute_opengwm();
void show_ascii_art();
void execute_about();
void execute_command(char* cmd);
void print_prompt();
void wait_for_enter();
void shell();

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

volatile uint32_t timer_ticks = 0;
#define TIMER_HZ 18.2
uint32_t boot_time = 0;

#define TOTAL_MEMORY_KB 640
static uintptr_t next_node_addr = 0x100000;

typedef struct {
    char name[32];
    uint64_t total_bytes;
    uint64_t free_bytes;
} Disk;

Disk disks[] = {
    {"Primary HDD", 500 * 1024 * 1024, 250 * 1024 * 1024},
    {"Secondary HDD", 1024 * 1024 * 1024, 768 * 1024 * 1024}
};
int disk_count = 2;

static size_t str_len(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

static int str_cmp_case_insensitive(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        char c1 = *s1;
        char c2 = *s2;
        
        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        
        if (c1 != c2) return c1 - c2;
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

static void str_cpy(char* dest, const char* src) {
    while ((*dest++ = *src++));
}

static void int_to_str(int num, char* str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
    } else {
        while (num != 0) {
            int rem = num % 10;
            str[i++] = rem + '0';
            num = num / 10;
        }
    }
    str[i] = '\0';
    
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color = 0;
volatile uint16_t* terminal_buffer = 0;

bool shift_pressed = false;
bool ctrl_pressed = false;
bool alt_pressed = false;
bool caps_lock = false;

void terminal_setcolor(uint8_t fg, uint8_t bg) {
    terminal_color = (bg << 4) | (fg & 0x0F);
}

void terminal_clear() {
    for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            terminal_buffer[y * SCREEN_WIDTH + x] = ' ' | (terminal_color << 8);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
    move_cursor(0, 0);
}

void terminal_initialize() {
    terminal_buffer = VIDEO_MEMORY;
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    terminal_clear();
}

void terminal_scroll() {
    for (size_t y = 1; y < SCREEN_HEIGHT; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            terminal_buffer[(y-1)*SCREEN_WIDTH + x] = terminal_buffer[y*SCREEN_WIDTH + x];
        }
    }
    
    for (size_t x = 0; x < SCREEN_WIDTH; x++) {
        terminal_buffer[(SCREEN_HEIGHT-1)*SCREEN_WIDTH + x] = ' ' | (terminal_color << 8);
    }
    
    terminal_row = SCREEN_HEIGHT - 1;
    terminal_column = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == SCREEN_HEIGHT) {
            terminal_scroll();
        }
    } else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
            terminal_buffer[terminal_row * SCREEN_WIDTH + terminal_column] = ' ' | (terminal_color << 8);
        } else if (terminal_row > 0) {
            terminal_row--;
            terminal_column = SCREEN_WIDTH - 1;
            terminal_buffer[terminal_row * SCREEN_WIDTH + terminal_column] = ' ' | (terminal_color << 8);
        }
    } else {
        terminal_buffer[terminal_row * SCREEN_WIDTH + terminal_column] = c | (terminal_color << 8);
        if (++terminal_column == SCREEN_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == SCREEN_HEIGHT) {
                terminal_scroll();
            }
        }
    }
    move_cursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, str_len(data));
}

void move_cursor(size_t x, size_t y) {
    uint16_t pos = y * SCREEN_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

uint8_t cmos_read(uint8_t reg) {
    outb(CMOS_ADDRESS, reg);
    return inb(CMOS_DATA);
}

int bcd_to_bin(int bcd) {
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}

char keyboard_getchar() {
    while (1) {
        if (inb(0x64) & 0x01) {
            uint8_t scancode = inb(0x60);
            
            if (scancode & 0x80) {
                uint8_t released_key = scancode & 0x7F;
                if (released_key == KEY_LSHIFT || released_key == KEY_RSHIFT) {
                    shift_pressed = false;
                } else if (released_key == KEY_CTRL) {
                    ctrl_pressed = false;
                } else if (released_key == KEY_ALT) {
                    alt_pressed = false;
                }
                continue;
            }
            
            if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
                shift_pressed = true;
                continue;
            } else if (scancode == KEY_CTRL) {
                ctrl_pressed = true;
                continue;
            } else if (scancode == KEY_ALT) {
                alt_pressed = true;
                continue;
            } else if (scancode == KEY_CAPSLOCK) {
                caps_lock = !caps_lock;
                continue;
            }
            
            if (scancode == 0xE0) {
                scancode = inb(0x60);
                switch(scancode) {
                    case KEY_UP:    return '\x11';
                    case KEY_DOWN:  return '\x12';
                    case KEY_LEFT:  return '\x13';
                    case KEY_RIGHT: return '\x14';
                }
            }
            
            if (scancode == KEY_ENTER) return '\n';
            if (scancode == KEY_BACKSPACE) return '\b';
            if (scancode == KEY_ESC) return '\x1B';
            
            if (scancode < 0x80) {
                const char* keyboard_map_lower = "\x00\x1B" "1234567890-=" "\x08"
                    "\x00" "qwertyuiop[]" "\x0D" "\x00" "asdfghjkl;'`" "\x00" 
                    "\\zxcvbnm,./" "\x00\x00\x00" " ";
                
                const char* keyboard_map_upper = "\x00\x1B" "!@#$%^&*()_+" "\x08"
                    "\x00" "QWERTYUIOP{}" "\x0D" "\x00" "ASDFGHJKL:\"~" "\x00" 
                    "|ZXCVBNM<>?" "\x00\x00\x00" " ";
                
                bool uppercase = (shift_pressed != caps_lock);
                
                if (ctrl_pressed) {
                    if (scancode >= 0x10 && scancode <= 0x1C) {
                        return scancode - 0x10 + 1;
                    }
                    continue;
                }
                
                if (alt_pressed) {
                    continue;
                }
                
                if (uppercase && keyboard_map_upper[scancode]) {
                    return keyboard_map_upper[scancode];
                } else if (!uppercase && keyboard_map_lower[scancode]) {
                    return keyboard_map_lower[scancode];
                }
            }
        }
    }
}

void init_timer() {
    uint32_t divisor = PIT_FREQUENCY / TIMER_HZ;
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
    boot_time = timer_ticks;
}

void mouse_wait(uint8_t type) {
    uint32_t timeout = 100000;
    if (type == 0) {
        while (timeout--) {
            if ((inb(MOUSE_CTRL) & 0x02) == 0) return;
        }
    } else {
        while (timeout--) {
            if ((inb(MOUSE_CTRL) & 0x01) == 1) return;
        }
    }
}

void mouse_write(uint8_t data) {
    mouse_wait(0);
    outb(MOUSE_CTRL, MOUSE_WRITE);
    mouse_wait(0);
    outb(MOUSE_PORT, data);
}

uint8_t mouse_read() {
    mouse_wait(1);
    return inb(MOUSE_PORT);
}

void mouse_init() {
    mouse_wait(0);
    outb(MOUSE_CTRL, MOUSE_ENABLE);
    mouse_wait(0);
    mouse_write(MOUSE_INIT);
    mouse_read();
}

void kernel_panic() {
    terminal_setcolor(COLOR_RED, COLOR_WHITE);
    terminal_clear();
    
    terminal_writestring("\n\nKERNEL PANIC!\n");
    terminal_writestring("System is crashed. Please restart your computer. лол русский не поддерживается влгсргшрсгшыврзщсргщ вгшпрвшгписшрпврпсриыврлсиывисроивысиорывисорывисорыивсиорвыисорыивсывписшывригсргшщвпрн\n");
    
    while (1) {
        asm volatile ("cli; hlt");
    }
}

void execute_fino() {
    terminal_clear();
    terminal_writestring("\nFikusOS Text Editor (fino) - ESC to exit\n");
    terminal_writestring("----------------------------------------\n");
    
    char buffer[1024] = {0};
    int pos = 0;
    int cursor_pos = 0;
    size_t saved_row = terminal_row;
    size_t saved_col = terminal_column;
    
    while (1) {
        char c = keyboard_getchar();
        
        if (c == '\x1B') {
            terminal_writestring("\nExit without saving\n");
            return;
        }
        else if (c == '\x11') {
            if (terminal_row > saved_row) {
                terminal_row--;
                if (cursor_pos > SCREEN_WIDTH) cursor_pos -= SCREEN_WIDTH;
                else cursor_pos = 0;
                move_cursor(cursor_pos, terminal_row);
            }
        }
        else if (c == '\x12') {
            if (terminal_row < SCREEN_HEIGHT - 1) {
                terminal_row++;
                cursor_pos += SCREEN_WIDTH;
                if (cursor_pos > pos) cursor_pos = pos;
                move_cursor(cursor_pos % SCREEN_WIDTH, terminal_row);
            }
        }
        else if (c == '\x13') {
            if (cursor_pos > 0) {
                cursor_pos--;
                if (terminal_column > 0) {
                    terminal_column--;
                } else if (terminal_row > saved_row) {
                    terminal_row--;
                    terminal_column = SCREEN_WIDTH - 1;
                }
                move_cursor(terminal_column, terminal_row);
            }
        }
        else if (c == '\x14') {
            if (cursor_pos < pos) {
                cursor_pos++;
                if (terminal_column < SCREEN_WIDTH - 1) {
                    terminal_column++;
                } else if (terminal_row < SCREEN_HEIGHT - 1) {
                    terminal_row++;
                    terminal_column = 0;
                }
                move_cursor(terminal_column, terminal_row);
            }
        }
        else if (c == '\n') {
            buffer[pos++] = c;
            terminal_putchar(c);
            cursor_pos = pos;
        } 
        else if (c == '\b') {
            if (pos > 0 && cursor_pos > 0) {
                for (int i = cursor_pos - 1; i < pos; i++) {
                    buffer[i] = buffer[i + 1];
                }
                pos--;
                cursor_pos--;
                
                terminal_row = saved_row;
                terminal_column = 0;
                move_cursor(terminal_column, terminal_row);
                terminal_writestring("                                        ");
                move_cursor(0, saved_row);
                terminal_write(buffer, pos);
                
                move_cursor(cursor_pos % SCREEN_WIDTH, saved_row + (cursor_pos / SCREEN_WIDTH));
            }
        }
        else if (pos < sizeof(buffer) - 1) {
            for (int i = pos; i > cursor_pos; i--) {
                buffer[i] = buffer[i - 1];
            }
            buffer[cursor_pos] = c;
            pos++;
            cursor_pos++;
            
            terminal_write(buffer + cursor_pos - 1, pos - cursor_pos + 1);
            move_cursor(cursor_pos % SCREEN_WIDTH, saved_row + (cursor_pos / SCREEN_WIDTH));
        }
    }
}

void execute_color(char* color) {
    uint8_t bg_color;
    
    char color_lower[32];
    str_cpy(color_lower, color);
    for (char* p = color_lower; *p; p++) {
        if (*p >= 'A' && *p <= 'Z') *p += 32;
    }
    
    if (str_cmp_case_insensitive(color_lower, "black") == 0) bg_color = COLOR_BLACK;
    else if (str_cmp_case_insensitive(color_lower, "blue") == 0) bg_color = COLOR_BLUE;
    else if (str_cmp_case_insensitive(color_lower, "green") == 0) bg_color = COLOR_GREEN;
    else if (str_cmp_case_insensitive(color_lower, "cyan") == 0) bg_color = COLOR_CYAN;
    else if (str_cmp_case_insensitive(color_lower, "red") == 0) bg_color = COLOR_RED;
    else if (str_cmp_case_insensitive(color_lower, "magenta") == 0) bg_color = COLOR_MAGENTA;
    else if (str_cmp_case_insensitive(color_lower, "brown") == 0) bg_color = COLOR_BROWN;
    else if (str_cmp_case_insensitive(color_lower, "gray") == 0) bg_color = COLOR_GRAY;
    else if (str_cmp_case_insensitive(color_lower, "white") == 0) bg_color = COLOR_WHITE;
    else {
        terminal_writestring("\nInvalid color. Available: black, blue, green, cyan, red, magenta, brown, gray, white\n");
        return;
    }
    
    terminal_setcolor(COLOR_WHITE, bg_color);
    terminal_clear();
    terminal_writestring("\nBackground color changed to ");
    terminal_writestring(color);
    terminal_writestring("\n");
}

void execute_memory() {
    uint32_t used_memory_kb = (next_node_addr - 0x100000) / 1024;
    uint32_t free_memory_kb = TOTAL_MEMORY_KB - used_memory_kb;
    
    char used_str[16], free_str[16], total_str[16];
    int_to_str(used_memory_kb, used_str);
    int_to_str(free_memory_kb, free_str);
    int_to_str(TOTAL_MEMORY_KB, total_str);
    
    terminal_writestring("Memory Usage:\n");
    terminal_writestring("Total: ");
    terminal_writestring(total_str);
    terminal_writestring(" KB\n");
    terminal_writestring("Used:  ");
    terminal_writestring(used_str);
    terminal_writestring(" KB\n");
    terminal_writestring("Free:  ");
    terminal_writestring(free_str);
    terminal_writestring(" KB\n");
}

void execute_disk() {
    terminal_writestring("Disk Information:\n");
    terminal_writestring("----------------------------------------\n");
    
    for (int i = 0; i < disk_count; i++) {
        char total_str[16], used_str[16], free_str[16];
        int_to_str(disks[i].total_bytes / (1024 * 1024), total_str);
        int_to_str((disks[i].total_bytes - disks[i].free_bytes) / (1024 * 1024), used_str);
        int_to_str(disks[i].free_bytes / (1024 * 1024), free_str);
        
        terminal_writestring(disks[i].name);
        terminal_writestring(":\n");
        terminal_writestring("  Total: ");
        terminal_writestring(total_str);
        terminal_writestring(" MB\n");
        terminal_writestring("  Used:  ");
        terminal_writestring(used_str);
        terminal_writestring(" MB\n");
        terminal_writestring("  Free:  ");
        terminal_writestring(free_str);
        terminal_writestring(" MB\n");
        
        if (i < disk_count - 1) {
            terminal_writestring("\n");
        }
    }
}

#define MAX_FILES 10
char* files[MAX_FILES] = {"file1.txt", "file2.doc", "notes"};
int file_count = 3;

void execute_ls() {
    terminal_writestring("\n");
    for (int i = 0; i < file_count; i++) {
        terminal_writestring(files[i]);
        terminal_writestring("  ");
    }
    terminal_writestring("\n");
}

void execute_pwd() {
    terminal_writestring("\n/root\n");
}

void execute_echo(char* args[], int arg_count) {
    for (int i = 1; i < arg_count; i++) {
        terminal_writestring(args[i]);
        terminal_writestring(" ");
    }
    terminal_writestring("\n");
}

void execute_date() {
    uint8_t second = cmos_read(0x00);
    uint8_t minute = cmos_read(0x02);
    uint8_t hour = cmos_read(0x04);
    uint8_t day = cmos_read(0x07);
    uint8_t month = cmos_read(0x08);
    uint8_t year = cmos_read(0x09);
    
    second = bcd_to_bin(second);
    minute = bcd_to_bin(minute);
    hour = bcd_to_bin(hour);
    day = bcd_to_bin(day);
    month = bcd_to_bin(month);
    year = bcd_to_bin(year);
    
    char hour_str[3], min_str[3], sec_str[3];
    int_to_str(hour, hour_str);
    int_to_str(minute, min_str);
    int_to_str(second, sec_str);
    
    char day_str[3], month_str[3], year_str[5];
    int_to_str(day, day_str);
    int_to_str(month, month_str);
    int_to_str(year + 2000, year_str);
    
    terminal_writestring("System Time: ");
    terminal_writestring(day_str);
    terminal_writestring("/");
    terminal_writestring(month_str);
    terminal_writestring("/");
    terminal_writestring(year_str);
    terminal_writestring(" ");
    terminal_writestring(hour_str);
    terminal_writestring(":");
    terminal_writestring(min_str);
    terminal_writestring(":");
    terminal_writestring(sec_str);
    terminal_writestring("\n");
}

void execute_whoami() {
    terminal_writestring("\nroot\n");
}

void execute_uptime() {
    uint32_t seconds = timer_ticks / TIMER_HZ;
    char sec_str[16];
    int_to_str(seconds, sec_str);
    terminal_writestring("\nUptime: ");
    terminal_writestring(sec_str);
    terminal_writestring(" seconds\n");
}

void execute_kptest() {
    kernel_panic();
}

void execute_poweroff() {
    terminal_writestring("\nSystem is shutting down...\n");
    outw(0x604, 0x2000);
    outw(0xB004, 0x2000);
    outw(0x4004, 0x3400);
    asm volatile ("cli");
    while (1) asm volatile ("hlt");
}

void execute_reboot() {
    terminal_writestring("\nSystem is rebooting...\n");
    outb(0x64, 0xFE);
    asm volatile ("cli");
    asm volatile ("int $0xFF");
    while (1);
}

int mouse_x = 40;
int mouse_y = 12;
bool mouse_enabled = false;

void execute_opengwm() {
    terminal_setcolor(COLOR_WHITE, COLOR_GRAY);
    terminal_clear();
   
    terminal_writestring("\n\n  FikusOS Graphical Window Manager\n");
    terminal_writestring("  ---------------------------------\n\n");
    terminal_writestring("  Press 'DEL' to exit\n");
    terminal_writestring("  Use mouse to move cursor (X)\n\n");
    
    mouse_init();
    mouse_enabled = true;
    
    terminal_buffer[mouse_y * SCREEN_WIDTH + mouse_x] = 'X' | (COLOR_BLACK << 8) | (COLOR_WHITE << 12);
    
    while (1) {
        if (inb(0x64) & 0x01) {
            uint8_t status = inb(0x64);
            if (status & 0x20) {
                uint8_t data = inb(0x60);
                
                if (mouse_enabled && (data & 0x08)) {
                    int8_t x_movement = (int8_t)inb(0x60);
                    int8_t y_movement = (int8_t)inb(0x60);
                    
                    terminal_buffer[mouse_y * SCREEN_WIDTH + mouse_x] = ' ' | (COLOR_GRAY << 8);
                    
                    mouse_x += x_movement;
                    mouse_y -= y_movement;
                    
                    if (mouse_x < 0) mouse_x = 0;
                    if (mouse_x >= SCREEN_WIDTH) mouse_x = SCREEN_WIDTH - 1;
                    if (mouse_y < 0) mouse_y = 0;
                    if (mouse_y >= SCREEN_HEIGHT) mouse_y = SCREEN_HEIGHT - 1;
                    
                    terminal_buffer[mouse_y * SCREEN_WIDTH + mouse_x] = 'X' | (COLOR_BLACK << 8) | (COLOR_WHITE << 12);
                }
            }
            
            uint8_t key = inb(0x60);
            if (key == 0x53) {
                mouse_enabled = false;
                terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
                terminal_clear();
                return;
            }
        }
        asm volatile ("hlt");
    }
}

void show_ascii_art() {
    terminal_writestring("              ______ _ _               ____   _____ \n");
    terminal_writestring("             |  ____(_) |             / __ \\ / ____|\n");
    terminal_writestring("             | |__   _| | ___   _ ___| |  | | (___  \n");
    terminal_writestring("             |  __| | | |/ / | | / __| |  | |\\___ \\ \n");
    terminal_writestring("             | |    | |   <| |_| \\__ \\ |__| |____) |\n");
    terminal_writestring("             |_|    |_|_|\\_\\\\__,_|___/\\____/|_____/ \n\n");
}

void execute_about() {
    show_ascii_art();
    terminal_writestring("OS: FikusOS\n");
    terminal_writestring("Version: Alpha 0.2\n");
    terminal_writestring("License: GNU GPL 2\n");
    terminal_writestring("Created for FikusPI\n");
    terminal_writestring("Shell: FKShell\n");
    terminal_writestring("Kernel: 0.0.1-Fikus\n");
    terminal_writestring("\n");
}

void execute_command(char* cmd) {
    char* args[10];
    int arg_count = 0;
    
    char cmd_copy[MAX_CMD_LEN];
    str_cpy(cmd_copy, cmd);
    
    for (char* p = cmd_copy; *p; p++) {
        if (*p >= 'A' && *p <= 'Z') *p += 32;
    }
    
    char* token = cmd_copy;
    while (*token && arg_count < 10 - 1) {
        while (*token == ' ') token++;
        if (*token == '\0') break;
        
        args[arg_count++] = token;
        while (*token && *token != ' ') token++;
        if (*token) *token++ = '\0';
    }
    args[arg_count] = 0;
    
    if (arg_count == 0) return;
    
    if (str_cmp_case_insensitive(args[0], "help") == 0) {
        terminal_writestring("\nAvailable commands:\n");
        terminal_writestring("help     - Show this help\n");
        terminal_writestring("cls      - Clear screen\n");
        terminal_writestring("about    - Show system info\n");
        terminal_writestring("color    - Change background color\n");
        terminal_writestring("fino     - Text editor\n");
        terminal_writestring("dir      - List files\n");
        terminal_writestring("pwd      - Print working directory\n");
        terminal_writestring("echo     - Display message\n");
        terminal_writestring("date     - Show current date/time\n");
        terminal_writestring("whoami   - Show current user\n");
        terminal_writestring("mem      - Show memory usage\n");
        terminal_writestring("disk     - Show disk information\n");
        terminal_writestring("poweroff - Shut down\n");
        terminal_writestring("reboot   - Reboot\n");
        terminal_writestring("kptest   - Test kernel panic\n");
    }
    else if (str_cmp_case_insensitive(args[0], "cls") == 0 || 
             str_cmp_case_insensitive(args[0], "clear") == 0) {
        terminal_clear();
    }
    else if (str_cmp_case_insensitive(args[0], "about") == 0) {
        execute_about();
    }
    else if (str_cmp_case_insensitive(args[0], "color") == 0) {
        if (arg_count > 1) execute_color(args[1]);
        else terminal_writestring("\nUsage: color <black|blue|gray|...>\n");
    }
    else if (str_cmp_case_insensitive(args[0], "fino") == 0) {
        execute_fino();
    }
    else if (str_cmp_case_insensitive(args[0], "dir") == 0 || 
             str_cmp_case_insensitive(args[0], "ls") == 0) {
        execute_ls();
    }
    else if (str_cmp_case_insensitive(args[0], "pwd") == 0) {
        execute_pwd();
    }
    else if (str_cmp_case_insensitive(args[0], "echo") == 0) {
        char* orig_args[10];
        int orig_arg_count = 0;
        char* orig_token = cmd;
        while (*orig_token && orig_arg_count < 10 - 1) {
            while (*orig_token == ' ') orig_token++;
            if (*orig_token == '\0') break;
            
            orig_args[orig_arg_count++] = orig_token;
            while (*orig_token && *orig_token != ' ') orig_token++;
            if (*orig_token) *orig_token++ = '\0';
        }
        orig_args[orig_arg_count] = 0;
        
        execute_echo(orig_args, orig_arg_count);
    }
    else if (str_cmp_case_insensitive(args[0], "date") == 0) {
        execute_date();
    }
    else if (str_cmp_case_insensitive(args[0], "whoami") == 0) {
        execute_whoami();
    }
    else if (str_cmp_case_insensitive(args[0], "uptime") == 0) {
        execute_uptime();
    }
    else if (str_cmp_case_insensitive(args[0], "mem") == 0 || 
             str_cmp_case_insensitive(args[0], "memory") == 0) {
        execute_memory();
    }
    else if (str_cmp_case_insensitive(args[0], "disk") == 0 || 
             str_cmp_case_insensitive(args[0], "data") == 0) {
        execute_disk();
    }
    else if (str_cmp_case_insensitive(args[0], "opengwm") == 0) {
        execute_opengwm();
    }
    else if (str_cmp_case_insensitive(args[0], "poweroff") == 0 || 
             str_cmp_case_insensitive(args[0], "shutdown") == 0) {
        execute_poweroff();
    }
    else if (str_cmp_case_insensitive(args[0], "reboot") == 0 || 
             str_cmp_case_insensitive(args[0], "restart") == 0) {
        execute_reboot();
    }
    else if (str_cmp_case_insensitive(args[0], "kptest") == 0) {
        execute_kptest();
    }
    else if (args[0][0] != '\0') {
        terminal_writestring("\nCommand not found: ");
        terminal_writestring(args[0]);
        terminal_writestring("\nType 'help' for available commands\n");
    }
}

void print_prompt() {
    terminal_setcolor(COLOR_GREEN, terminal_color >> 4);
    terminal_writestring("FIKUS~# ");
    terminal_setcolor(COLOR_WHITE, terminal_color >> 4);
    move_cursor(terminal_column, terminal_row);
}

void wait_for_enter() {
    terminal_setcolor(COLOR_WHITE, COLOR_BLACK);
    terminal_clear();
    
    terminal_writestring("\n\n\n");
    show_ascii_art();
    
    terminal_setcolor(COLOR_BROWN, COLOR_BLACK);
    terminal_writestring("                           FikusOS\n");
    terminal_writestring("                    GNU Operating System\n\n");
    terminal_writestring("                Copyright GPL (c) 2025 FikusOS\n\n\n");
    
    terminal_setcolor(COLOR_GREEN, COLOR_BLACK);
    terminal_writestring("Please press Enter to continue booting the system...");
    
    while (keyboard_getchar() != '\n');
    
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    terminal_clear();
}

void shell() {
    char cmd[MAX_CMD_LEN];
    int pos = 0;
    
    terminal_writestring("FikusOS Shell - Type 'help' for commands\n\n");
    
    while (1) {
        print_prompt();
        pos = 0;
        
        while (pos < MAX_CMD_LEN - 1) {
            char c = keyboard_getchar();
            
            if (c == '\n') {
                terminal_putchar('\n');
                cmd[pos] = '\0';
                execute_command(cmd);
                break;
            }
            else if (c == '\b') {
                if (pos > 0) {
                    pos--;
                    terminal_putchar('\b');
                }
            }
            else if (c == '\x11' || c == '\x12' || c == '\x13' || c == '\x14') {
            }
            else {
                cmd[pos++] = c;
                terminal_putchar(c);
            }
        }
    }
}

void kernel_main() {
    terminal_initialize();
    wait_for_enter();
    
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    show_ascii_art();
    
    terminal_writestring("Initializing kernel...\n");
    init_timer();
    
    terminal_writestring("Starting shell\n\n");
    shell();
    
    while (1) asm volatile ("hlt");
}
