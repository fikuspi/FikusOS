#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
size_t strlen(const char* s);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
char* strchr(const char* s, int c);
char* strstr(const char* haystack, const char* needle);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
int strncmp(const char* s1, const char* s2, size_t n);

void* malloc(size_t size);
void free(void* ptr);
void* calloc(size_t nmemb, size_t size);
void* realloc(void* ptr, size_t size);

void* memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void* memset(void* s, int c, size_t n) {
    unsigned char* p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* p1 = s1, *p2 = s2;
    while (n--) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}

char* strchr(const char* s, int c) {
    while (*s != '\0') {
        if (*s == c) {
            return (char*)s;
        }
        s++;
    }
    return NULL;
}

char* strstr(const char* haystack, const char* needle) {
    size_t needle_len = strlen(needle);
    if (needle_len == 0) {
        return (char*)haystack;
    }
    
    for (; *haystack; haystack++) {
        if (*haystack == *needle && !memcmp(haystack, needle, needle_len)) {
            return (char*)haystack;
        }
    }
    return NULL;
}

int atoi(const char* str) {
    int res = 0;
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}

static inline int abs(int n) {
    return (n < 0) ? -n : n;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

char* strcpy(char* dest, const char* src) {
    char* ret = dest;
    while ((*dest++ = *src++));
    return ret;
}

char* strcat(char* dest, const char* src) {
    char* ret = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return ret;
}

char* strncpy(char* dest, const char* src, size_t n) {
    char* ret = dest;
    while (n-- && (*dest++ = *src++));
    while (n-- > 0) *dest++ = '\0';
    return ret;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char* strdup(const char* s) {
    size_t len = strlen(s) + 1;
    char* new = malloc(len);
    if (new == NULL) return NULL;
    memcpy(new, s, len);
    return new;
}


#define COM1 0x3F8
#define VIDEO_MEMORY ((volatile uint16_t*)0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define MAX_CMD_LEN 256
#define MOUSE_PORT 0x60
#define MOUSE_CTRL 0x64
#define PIT_FREQUENCY 1193182
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND 0x43
#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71
#define NET_IO_BASE 0x1000
#define NET_IRQ 11
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define MAX_FILES 128
#define MAX_PROCESSES 16
#define MAX_NET_CONNECTIONS 8
#define SECTOR_SIZE 512
#define MAX_PATH_LEN 256
#define TOTAL_MEMORY_KB 640
#define MAX_WINDOWS 10
#define MAX_PROGRAMS 10
#define MAX_BLOCKS 1024
#define MAX_INODES 128
#define BLOCK_SIZE 1024
#define INODE_DIRECT_BLOCKS 12

#define COLOR_BLACK   0x0
#define COLOR_BLUE    0x1
#define COLOR_GREEN   0x2
#define COLOR_CYAN    0x3
#define COLOR_RED     0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN   0x6
#define COLOR_GRAY    0x7
#define COLOR_DARK_GRAY 0x8
#define COLOR_BRIGHT_BLUE 0x9
#define COLOR_BRIGHT_GREEN 0xA
#define COLOR_BRIGHT_CYAN 0xB
#define COLOR_BRIGHT_RED 0xC
#define COLOR_BRIGHT_MAGENTA 0xD
#define COLOR_YELLOW 0xE
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
#define KEY_SPACE     0x39
#define KEY_F1        0x3B
#define KEY_F2        0x3C
#define KEY_F3        0x3D
#define KEY_F4        0x3E
#define KEY_F5        0x3F
#define KEY_F6        0x40
#define KEY_F7        0x41
#define KEY_F8        0x42
#define KEY_F9        0x43
#define KEY_F10       0x44
#define KEY_W         0x11

#define MOUSE_ENABLE  0xA8
#define MOUSE_WRITE   0xD4
#define MOUSE_INIT    0xF4

#define FILE_REGULAR 0
#define FILE_DIR     1
#define FILE_SYMLINK 2

#define FS_SUCCESS 0
#define FS_ERROR   1

#define WINDOW_NORMAL 0
#define WINDOW_MINIMIZED 1
#define WINDOW_MAXIMIZED 2

typedef struct {
    char name[32];
    uint64_t total_bytes;
    uint64_t free_bytes;
} Disk;

typedef struct {
    uint32_t mode;       
    uint32_t uid;       
    uint32_t gid;       
    uint32_t size;      
    uint32_t atime;     
    uint32_t ctime;     
    uint32_t mtime;     
    uint32_t dtime;     
    uint32_t blocks;     
    uint32_t block[INODE_DIRECT_BLOCKS]; 
} Inode;

typedef struct {
    char name[32];
    uint32_t inode;
    uint32_t parent_inode;
} FileEntry;

typedef struct {
    uint8_t mac[6];
    uint8_t ip[4];
    uint8_t subnet[4];
    uint8_t gateway[4];
    uint8_t dns[4];
    bool dhcp_enabled;
} NetworkConfig;

typedef struct {
    uint8_t state;
    uint16_t local_port;
    uint16_t remote_port;
    uint8_t remote_ip[4];
    uint32_t seq_num;
    uint32_t ack_num;
} TCPConnection;

typedef struct {
    uint32_t pid;
    uint32_t ppid;
    char name[32];
    uint32_t priority;
    uint32_t state;
    uintptr_t stack_ptr;
    uintptr_t entry_point;
} Process;

typedef struct {
    uint32_t magic;
    uint32_t block_size;
    uint32_t inodes_count;
    uint32_t free_inodes;
    uint32_t blocks_count;
    uint32_t free_blocks;
    uint32_t first_data_block;
    uint32_t inodes_per_group;
    uint32_t mtime;
    uint32_t wtime;
    uint16_t mounts_count;
    uint16_t max_mounts;
    uint16_t magic_signature;
    uint16_t state;
    uint16_t errors;
    uint16_t minor_rev_level;
    uint32_t lastcheck;
    uint32_t checkinterval;
    uint32_t creator_os;
    uint32_t rev_level;
    uint16_t def_resuid;
    uint16_t def_resgid;
} Superblock;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    char title[32];
    uint8_t color;
    bool active;
    uint8_t state;
    void (*draw_func)();
    void (*handle_input)(char c);
} Window;

typedef struct {
    char name[32];
    void (*open_func)();
} Program;

typedef struct {
    uint32_t inode;
    char name[32];
    uint32_t parent_inode;
    uint8_t type;
    uint32_t size;
} File;

volatile uint32_t timer_ticks = 0;
#define TIMER_HZ 18.2
uint32_t boot_time = 0;

static uintptr_t next_node_addr = 0x100000;
static uintptr_t heap_start = 0x200000;
static uintptr_t heap_end = 0x400000;

Disk disks[] = {
    {"Primary HDD", 500ULL * 1024 * 1024, 250ULL * 1024 * 1024},
    {"Secondary HDD", 1024ULL * 1024 * 1024, 768ULL * 1024 * 1024},
    {"USB Drive", 16ULL * 1024 * 1024 * 1024, 12ULL * 1024 * 1024 * 1024}
};
int disk_count = 3;

File files[MAX_FILES];
int file_count = 5;
uint32_t current_inode = 1;

Inode inodes[MAX_INODES];
uint8_t blocks[MAX_BLOCKS][BLOCK_SIZE];
bool block_used[MAX_BLOCKS];
uint32_t free_blocks = MAX_BLOCKS;
uint32_t free_inodes = MAX_INODES;

int mouse_x = 40;
int mouse_y = 12;
bool mouse_enabled = false;
bool mouse_left_button = false;
bool mouse_right_button = false;
bool mouse_middle_button = false;
int mouse_start_x = 0;
int mouse_start_y = 0;

size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color = 0;
volatile uint16_t* terminal_buffer = 0;

bool shift_pressed = false;
bool ctrl_pressed = false;
bool alt_pressed = false;
bool caps_lock = false;

NetworkConfig net_config = {
    .mac = {0x52, 0x54, 0x00, 0x12, 0x34, 0x56},
    .ip = {192, 168, 1, 100},
    .subnet = {255, 255, 255, 0},
    .gateway = {192, 168, 1, 1},
    .dns = {8, 8, 8, 8},
    .dhcp_enabled = false
};

TCPConnection tcp_connections[MAX_NET_CONNECTIONS];
Process processes[MAX_PROCESSES];
int process_count = 0;

Window windows[MAX_WINDOWS];
int window_count = 0;
int active_window = -1;
bool gwm_mode = false;

Program programs[MAX_PROGRAMS];
int program_count = 0;
bool show_start_menu = false;
int selected_menu_item = -1;

void kernel_main();
void kernel_panic(const char* message);
void terminal_initialize();
void move_cursor(size_t x, size_t y);
void terminal_setcolor(uint8_t fg, uint8_t bg);
void terminal_clear();
void terminal_scroll();
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_printf(const char* format, ...);
void draw_window(Window* win);
void draw_gwm_desktop();
void draw_gwm_taskbar();
void update_clock();
void draw_about_window();
void draw_calc_window();
void draw_fte_window();
void draw_file_manager();
void draw_system_info();
void draw_paint();
void handle_calc_input(char c);
void handle_fte_input(char c);
void handle_file_manager_input(char c);
void handle_paint_input(char c);
void init_gwm();
void open_calc();
void open_text_editor();
void open_file_manager();
void open_system_info();
void open_paint();
void open_terminal();
void shutdown_pc();
void draw_start_menu();
void shell();

static int strcmp_case_insensitive(const char* s1, const char* s2) {
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

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outl(uint16_t port, uint32_t val) {
    asm volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait() {
    outb(0x80, 0);
}

void terminal_initialize() {
    terminal_buffer = VIDEO_MEMORY;
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    terminal_clear();
}

void move_cursor(size_t x, size_t y) {
    uint16_t pos = y * SCREEN_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

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
    while (*data) {
        terminal_putchar(*data++);
    }
}

void terminal_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    char buffer[256];
    char *ptr = buffer;
    
    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    int num = va_arg(args, int);
                    char num_str[16];
                    int_to_str(num, num_str);
                    char *tmp = num_str;
                    while (*tmp) {
                        *ptr++ = *tmp++;
                    }
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char*);
                    while (*str) {
                        *ptr++ = *str++;
                    }
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    *ptr++ = c;
                    break;
                }
                case 'x': {
                    uint32_t num = va_arg(args, uint32_t);
                    const char* hex = "0123456789ABCDEF";
                    for (int i = 7; i >= 0; i--) {
                        *ptr++ = hex[(num >> (i * 4)) & 0xF];
                    }
                    break;
                }
                case '%': {
                    *ptr++ = '%';
                    break;
                }
            }
        } else {
            *ptr++ = *format;
        }
        format++;
    }
    *ptr = '\0';
    
    terminal_writestring(buffer);
    va_end(args);
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
            if (scancode == KEY_SPACE) return ' ';
            if (scancode == KEY_F1) return '\xF1';
            if (scancode == KEY_F2) return '\xF2';
            if (scancode == KEY_F3) return '\xF3';
            if (scancode == KEY_F4) return '\xF4';
            if (scancode == KEY_F5) return '\xF5';
            if (scancode == KEY_F6) return '\xF6';
            if (scancode == KEY_F7) return '\xF7';
            if (scancode == KEY_F8) return '\xF8';
            if (scancode == KEY_F9) return '\xF9';
            if (scancode == KEY_F10) return '\xFA';
            if (scancode == KEY_W) return 'w';
            
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
    outb(MOUSE_CTRL, 0xA8);
    mouse_wait(0);
    outb(MOUSE_CTRL, 0x20);
    mouse_wait(1);
    uint8_t status = inb(MOUSE_PORT) | 2;
    mouse_wait(0);
    outb(MOUSE_CTRL, 0x60);
    mouse_wait(0);
    outb(MOUSE_PORT, status);
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
}

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

void pci_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    outl(PCI_CONFIG_ADDRESS, address);
    outl(PCI_CONFIG_DATA, value);
}

bool detect_network_card() {
    for (uint8_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t vendor_device = pci_read(bus, slot, 0, 0);
            uint16_t vendor = vendor_device & 0xFFFF;
            uint16_t device = (vendor_device >> 16) & 0xFFFF;
            
            if (vendor != 0xFFFF) {
                uint8_t class = pci_read(bus, slot, 0, 0x08) >> 24;
                uint8_t subclass = pci_read(bus, slot, 0, 0x08) >> 16 & 0xFF;
                
                if (class == 0x02 && subclass == 0x00) {
                    return true;
                }
            }
        }
    }
    return false;
}

void net_send_packet(uint8_t* data, uint16_t len) {
    outw(NET_IO_BASE + 0, len);
    for (uint16_t i = 0; i < len; i++) {
        outb(NET_IO_BASE + 4, data[i]);
    }
    outb(NET_IO_BASE + 6, 1);
}

uint16_t net_receive_packet(uint8_t* buffer) {
    uint16_t len = inw(NET_IO_BASE + 0);
    if (len == 0 || len > 2048) return 0;
    
    for (uint16_t i = 0; i < len; i++) {
        buffer[i] = inb(NET_IO_BASE + 4);
    }
    outb(NET_IO_BASE + 6, 0);
    return len;
}

void net_init() {
    if (!detect_network_card()) {
        terminal_writestring("No network card found\n");
        return;
    }
    
    terminal_writestring("Initializing network...\n");
    
    for (int i = 0; i < MAX_NET_CONNECTIONS; i++) {
        tcp_connections[i].state = 0;
    }
}

void net_handle_packet(uint8_t* packet, uint16_t len) {
    uint8_t* ip_header = packet + 14;
    uint8_t protocol = ip_header[9];
    
    if (protocol == 6) {
        uint8_t* tcp_header = ip_header + 20;
        uint16_t src_port = (tcp_header[0] << 8) | tcp_header[1];
        uint16_t dst_port = (tcp_header[2] << 8) | tcp_header[3];
        
        for (int i = 0; i < MAX_NET_CONNECTIONS; i++) {
            if (tcp_connections[i].state == 1 && 
                tcp_connections[i].local_port == dst_port) {
                uint8_t flags = tcp_header[13];
                if (flags & 0x01) {
                    tcp_connections[i].state = 0;
                }
                break;
            }
        }
    }
}

void net_process() {
    uint8_t packet[2048];
    uint16_t len = net_receive_packet(packet);
    if (len > 0) {
        net_handle_packet(packet, len);
    }
}

int net_connect(uint8_t* ip, uint16_t port) {
    for (int i = 0; i < MAX_NET_CONNECTIONS; i++) {
        if (tcp_connections[i].state == 0) {
            tcp_connections[i].state = 1;
            tcp_connections[i].local_port = 49152 + i;
            tcp_connections[i].remote_port = port;
            memcpy(tcp_connections[i].remote_ip, ip, 4);
            tcp_connections[i].seq_num = 0;
            tcp_connections[i].ack_num = 0;
            return i;
        }
    }
    return -1;
}

void net_disconnect(int conn_id) {
    if (conn_id >= 0 && conn_id < MAX_NET_CONNECTIONS) {
        tcp_connections[conn_id].state = 0;
    }
}

int net_send(int conn_id, uint8_t* data, uint16_t len) {
    if (conn_id < 0 || conn_id >= MAX_NET_CONNECTIONS || 
        tcp_connections[conn_id].state != 1) {
        return -1;
    }
    
    uint8_t packet[2048];
    uint8_t* eth_header = packet;
    uint8_t* ip_header = packet + 14;
    uint8_t* tcp_header = packet + 34;
    uint8_t* payload = packet + 54;
    
    memcpy(payload, data, len);
    
    uint16_t total_len = 54 + len;
    net_send_packet(packet, total_len);
    
    return len;
}

int net_receive(int conn_id, uint8_t* buffer, uint16_t max_len) {
    if (conn_id < 0 || conn_id >= MAX_NET_CONNECTIONS || 
        tcp_connections[conn_id].state != 1) {
        return -1;
    }
    
    uint8_t packet[2048];
    uint16_t len = net_receive_packet(packet);
    if (len > 0) {
        uint8_t* ip_header = packet + 14;
        uint8_t* tcp_header = ip_header + 20;
        uint8_t* payload = tcp_header + 20;
        uint16_t payload_len = len - 54;
        
        if (payload_len > max_len) payload_len = max_len;
        memcpy(buffer, payload, payload_len);
        return payload_len;
    }
    return 0;
}

void* malloc(size_t size) {
    if (heap_start + size > heap_end) {
        return NULL;
    }
    void* ptr = (void*)heap_start;
    heap_start += size;
    return ptr;
}

void free(void* ptr) {
}

void* calloc(size_t num, size_t size) {
    void* ptr = malloc(num * size);
    if (ptr) {
        memset(ptr, 0, num * size);
    }
    return ptr;
}

void* realloc(void* ptr, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }
    
    void* new_ptr = malloc(new_size);
    if (new_ptr && ptr) {
        memcpy(new_ptr, ptr, new_size);
        free(ptr);
    }
    return new_ptr;
}

void draw_pixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        terminal_buffer[y * SCREEN_WIDTH + x] = ' ' | (color << 8);
    }
}

void draw_char(int x, int y, char c, uint8_t fg, uint8_t bg) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        terminal_buffer[y * SCREEN_WIDTH + x] = c | (fg << 8) | (bg << 12);
    }
}

void draw_string(int x, int y, const char* str, uint8_t fg, uint8_t bg) {
    while (*str) {
        draw_char(x++, y, *str++, fg, bg);
    }
}

void draw_rect(int x, int y, int width, int height, uint8_t color) {
    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            draw_pixel(j, i, color);
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2, uint8_t color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        draw_pixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_window(Window* win) {
    draw_rect(win->x, win->y, win->width, win->height, COLOR_GRAY);
    
    draw_rect(win->x, win->y, win->width, 1, COLOR_BLUE);
    draw_string(win->x + 2, win->y, win->title, COLOR_WHITE, COLOR_BLUE);
    
    draw_char(win->x + win->width - 3, win->y, '-', COLOR_WHITE, COLOR_BLUE);
    draw_char(win->x + win->width - 2, win->y, 'x', COLOR_WHITE, COLOR_BLUE);
    
    if (win->draw_func) {
        win->draw_func();
    }
}

void draw_start_menu() {
    if (!show_start_menu) return;
    
    draw_rect(2, SCREEN_HEIGHT - 6 - program_count, 20, program_count + 1, COLOR_WHITE);
    draw_string(3, SCREEN_HEIGHT - 6 - program_count, " Programs ", COLOR_BLUE, COLOR_WHITE);
    
    for (int i = 0; i < program_count; i++) {
        if (i == selected_menu_item) {
            draw_rect(3, SCREEN_HEIGHT - 5 - program_count + i, 18, 1, COLOR_BLUE);
            draw_string(4, SCREEN_HEIGHT - 5 - program_count + i, programs[i].name, COLOR_WHITE, COLOR_BLUE);
        } else {
            draw_string(4, SCREEN_HEIGHT - 5 - program_count + i, programs[i].name, COLOR_BLACK, COLOR_WHITE);
        }
    }
}

void draw_gwm_desktop() {
    for (int y = 0; y < SCREEN_HEIGHT - 1; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint8_t color = COLOR_BLUE;
            if ((x + y) % 4 == 0) color = COLOR_BRIGHT_BLUE;
            if ((x - y) % 5 == 0) color = COLOR_BRIGHT_CYAN;
            draw_pixel(x, y, color);
        }
    }
    
    draw_string(5, 2, "ooo", COLOR_WHITE, COLOR_BLUE);
    draw_string(5, 3, "000", COLOR_WHITE, COLOR_BLUE);
    draw_string(5, 4, "***", COLOR_WHITE, COLOR_BLUE);
    
    for (int i = 0; i < window_count; i++) {
        if (windows[i].state != WINDOW_MINIMIZED) {
            draw_window(&windows[i]);
        }
    }
    
    draw_gwm_taskbar();
    
    draw_start_menu();
}

void draw_gwm_taskbar() {
    draw_rect(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, 1, COLOR_WHITE);
    
    draw_rect(2, SCREEN_HEIGHT - 1, 8, 1, COLOR_BLUE);
    draw_string(3, SCREEN_HEIGHT - 1, " Start ", COLOR_WHITE, COLOR_BLUE);
    
    update_clock();
}

void update_clock() {
    uint8_t hour = cmos_read(0x04);
    uint8_t minute = cmos_read(0x02);
    uint8_t second = cmos_read(0x00);
    
    hour = bcd_to_bin(hour);
    minute = bcd_to_bin(minute);
    second = bcd_to_bin(second);
    
    char time_str[9];
    int_to_str(hour, time_str);
    time_str[2] = ':';
    int_to_str(minute, time_str + 3);
    time_str[5] = ':';
    int_to_str(second, time_str + 6);
    time_str[8] = '\0';
    
    if (hour < 10) {
        time_str[0] = '0';
        time_str[1] = hour + '0';
    }
    if (minute < 10) {
        time_str[3] = '0';
        time_str[4] = minute + '0';
    }
    if (second < 10) {
        time_str[6] = '0';
        time_str[7] = second + '0';
    }
    
    draw_string(SCREEN_WIDTH - 9, SCREEN_HEIGHT - 1, time_str, COLOR_BLACK, COLOR_WHITE);
}

void draw_about_window() {
    Window* win = &windows[active_window];
    
    draw_string(win->x + 2, win->y + 2, "FikusOS version 0.4 Alpha", COLOR_BLACK, COLOR_GRAY);
    draw_string(win->x + 2, win->y + 3, "Build 2025-05-09", COLOR_BLACK, COLOR_GRAY);
    draw_string(win->x + 2, win->y + 4, "License: GNU GPL 2", COLOR_BLACK, COLOR_GRAY);
    draw_string(win->x + 2, win->y + 5, "Created for FikusPI", COLOR_BLACK, COLOR_GRAY);
    draw_string(win->x + 2, win->y + 6, "Shell: FKShell", COLOR_BLACK, COLOR_GRAY);
    draw_string(win->x + 2, win->y + 7, "Kernel: 0.0.9-Fikus", COLOR_BLACK, COLOR_GRAY);
    
    draw_rect(win->x + win->width - 10, win->y + win->height - 2, 8, 1, COLOR_BLUE);
    draw_string(win->x + win->width - 9, win->y + win->height - 2, " OK ", COLOR_WHITE, COLOR_BLUE);
}

void draw_calc_window() {
    Window* win = &windows[active_window];
    
    draw_rect(win->x + 2, win->y + 2, win->width - 4, 1, COLOR_WHITE);
    draw_string(win->x + 3, win->y + 2, "0", COLOR_BLACK, COLOR_WHITE);
    
    const char* buttons[] = {"7", "8", "9", "/",
                             "4", "5", "6", "*",
                             "1", "2", "3", "-",
                             "0", "C", "=", "+"};
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x = win->x + 2 + j * 4;
            int y = win->y + 4 + i * 2;
            draw_rect(x, y, 3, 1, COLOR_GRAY);
            draw_string(x + 1, y, buttons[i * 4 + j], COLOR_BLUE, COLOR_GRAY);
        }
    }
}

void draw_fte_window() {
    Window* win = &windows[active_window];
    
    draw_rect(win->x + 1, win->y + 1, win->width - 2, win->height - 2, COLOR_WHITE);
    
    draw_rect(win->x + 1, win->y + 1, win->width - 2, 1, COLOR_BLUE);
    draw_string(win->x + 2, win->y + 1, "File Edit View Help", COLOR_WHITE, COLOR_BLUE);
    
    draw_rect(win->x + 1, win->y + 2, win->width - 2, win->height - 4, COLOR_WHITE);
    draw_string(win->x + 2, win->y + 3, "Type here...", COLOR_BLACK, COLOR_WHITE);
}

void draw_file_manager() {
    Window* win = &windows[active_window];
    
    draw_rect(win->x + 1, win->y + 1, win->width - 2, win->height - 2, COLOR_WHITE);
    
    draw_rect(win->x + 1, win->y + 1, win->width - 2, 1, COLOR_BLUE);
    draw_string(win->x + 2, win->y + 1, "File Edit View Go Help", COLOR_WHITE, COLOR_BLUE);
    
    for (int i = 0; i < file_count; i++) {
        if (files[i].parent_inode == current_inode) {
            draw_string(win->x + 2, win->y + 3 + i, files[i].name, COLOR_BLACK, COLOR_WHITE);
            if (files[i].type == FILE_DIR) {
                draw_char(win->x + 2 + strlen(files[i].name), win->y + 3 + i, '/', COLOR_BLUE, COLOR_WHITE);
            }
        }
    }
}

void draw_system_info() {
    Window* win = &windows[active_window];
    
    draw_rect(win->x + 1, win->y + 1, win->width - 2, win->height - 2, COLOR_WHITE);
    
    draw_string(win->x + 2, win->y + 2, "=== System Information ===", COLOR_BLUE, COLOR_WHITE);
    
    uint32_t used_memory_kb = (next_node_addr - 0x100000) / 1024;
    uint32_t free_memory_kb = TOTAL_MEMORY_KB - used_memory_kb;
    char used_str[16], free_str[16];
    int_to_str(used_memory_kb, used_str);
    int_to_str(free_memory_kb, free_str);
    
    draw_string(win->x + 2, win->y + 4, "Memory:", COLOR_BLACK, COLOR_WHITE);
    draw_string(win->x + 10, win->y + 4, used_str, COLOR_BLACK, COLOR_WHITE);
    draw_string(win->x + 16, win->y + 4, "KB used,", COLOR_BLACK, COLOR_WHITE);
    draw_string(win->x + 25, win->y + 4, free_str, COLOR_BLACK, COLOR_WHITE);
    draw_string(win->x + 31, win->y + 4, "KB free", COLOR_BLACK, COLOR_WHITE);
    
    uint32_t seconds = timer_ticks / TIMER_HZ;
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;
    char uptime_str[32];
    int_to_str(hours, uptime_str);
    strcat(uptime_str, " hours, ");
    int_to_str(minutes % 60, uptime_str + strlen(uptime_str));
    strcat(uptime_str, " minutes, ");
    int_to_str(seconds % 60, uptime_str + strlen(uptime_str));
    strcat(uptime_str, " seconds");
    
    draw_string(win->x + 2, win->y + 6, "Uptime:", COLOR_BLACK, COLOR_WHITE);
    draw_string(win->x + 10, win->y + 6, uptime_str, COLOR_BLACK, COLOR_WHITE);
}

void draw_paint() {
    Window* win = &windows[active_window];
    
    draw_rect(win->x + 1, win->y + 1, win->width - 2, win->height - 2, COLOR_WHITE);
    
    draw_rect(win->x + 1, win->y + 1, win->width - 2, 1, COLOR_BLUE);
    draw_string(win->x + 2, win->y + 1, "File Edit View Colors Help", COLOR_WHITE, COLOR_BLUE);
    
    for (int i = 0; i < 16; i++) {
        draw_rect(win->x + 2 + i*2, win->y + 3, 2, 1, i);
    }
    
    draw_rect(win->x + 2, win->y + 5, win->width - 4, win->height - 7, COLOR_WHITE);
}

void handle_calc_input(char c) {
}

void handle_fte_input(char c) {
}

void handle_file_manager_input(char c) {
}

void handle_paint_input(char c) {
}

void create_window(int x, int y, int width, int height, const char* title, uint8_t color, 
                  void (*draw_func)(), void (*handle_input)(char)) {
    if (window_count >= MAX_WINDOWS) return;
    
    windows[window_count].x = x;
    windows[window_count].y = y;
    windows[window_count].width = width;
    windows[window_count].height = height;
    strcpy(windows[window_count].title, title);
    windows[window_count].color = color;
    windows[window_count].active = false;
    windows[window_count].state = WINDOW_NORMAL;
    windows[window_count].draw_func = draw_func;
    windows[window_count].handle_input = handle_input;
    
    window_count++;
}

void activate_window(int index) {
    if (index >= 0 && index < window_count) {
        for (int i = 0; i < window_count; i++) {
            windows[i].active = (i == index);
        }
        active_window = index;
    }
}

void close_window(int index) {
    if (index >= 0 && index < window_count) {
        for (int i = index; i < window_count - 1; i++) {
            windows[i] = windows[i + 1];
        }
        window_count--;
        if (active_window >= index) active_window--;
    }
}

void init_gwm() {
    program_count = 0;
    
    strcpy(programs[program_count].name, "Calculator");
    programs[program_count++].open_func = open_calc;
    
    strcpy(programs[program_count].name, "Text Editor");
    programs[program_count++].open_func = open_text_editor;
    
    strcpy(programs[program_count].name, "File Manager");
    programs[program_count++].open_func = open_file_manager;
    
    strcpy(programs[program_count].name, "System Info");
    programs[program_count++].open_func = open_system_info;
    
    strcpy(programs[program_count].name, "Draw");
    programs[program_count++].open_func = open_paint;
    
    strcpy(programs[program_count].name, "Terminal");
    programs[program_count++].open_func = open_terminal;
    
    strcpy(programs[program_count].name, "Shutdown");
    programs[program_count++].open_func = shutdown_pc;
}

void open_calc() {
    create_window(20, 8, 30, 12, "Calculator", COLOR_GRAY, draw_calc_window, handle_calc_input);
    activate_window(window_count - 1);
    show_start_menu = false;
}

void open_text_editor() {
    create_window(15, 5, 50, 18, "Text Editor", COLOR_GRAY, draw_fte_window, handle_fte_input);
    activate_window(window_count - 1);
    show_start_menu = false;
}

void open_file_manager() {
    create_window(10, 5, 40, 15, "File Manager", COLOR_GRAY, draw_file_manager, handle_file_manager_input);
    activate_window(window_count - 1);
    show_start_menu = false;
}

void open_system_info() {
    create_window(10, 5, 40, 15, "System Info", COLOR_GRAY, draw_system_info, NULL);
    activate_window(window_count - 1);
    show_start_menu = false;
}

void open_paint() {
    create_window(10, 5, 50, 20, "Draw", COLOR_GRAY, draw_paint, handle_paint_input);
    activate_window(window_count - 1);
    show_start_menu = false;
}

void open_terminal() {
    gwm_mode = false;
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    terminal_clear();
    shell();
}

void shutdown_pc() {
    terminal_writestring("\nSystem is shutting down...\n");
    outw(0x604, 0x2000);
    outw(0xB004, 0x2000);
    outw(0x4004, 0x3400);
    asm volatile ("cli");
    while (1) asm volatile ("hlt");
}

int fs_alloc_inode() {
    if (free_inodes == 0) return -1;
    
    for (int i = 0; i < MAX_INODES; i++) {
        if (inodes[i].mode == 0) {
            memset(&inodes[i], 0, sizeof(Inode));
            inodes[i].mode = 1; 
            free_inodes--;
            return i + 1; 
        }
    }
    return -1;
}

void fs_free_inode(uint32_t inode_num) {
    if (inode_num == 0 || inode_num > MAX_INODES) return;
    
    memset(&inodes[inode_num - 1], 0, sizeof(Inode));
    free_inodes++;
}

int fs_alloc_block() {
    if (free_blocks == 0) return -1;
    
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (!block_used[i]) {
            block_used[i] = true;
            free_blocks--;
            return i;
        }
    }
    return -1;
}

void fs_free_block(uint32_t block_num) {
    if (block_num >= MAX_BLOCKS) return;
    
    block_used[block_num] = false;
    free_blocks++;
}

int fs_create_file(const char* name, uint32_t parent_inode, uint8_t type) {
    if (file_count >= MAX_FILES) return FS_ERROR;
    
    int inode_num = fs_alloc_inode();
    if (inode_num == -1) return FS_ERROR;
    
    strncpy(files[file_count].name, name, 31);
    files[file_count].inode = inode_num;
    files[file_count].parent_inode = parent_inode;
    files[file_count].type = type;
    files[file_count].size = 0;
    
    Inode* inode = &inodes[inode_num - 1];
    inode->mode = type == FILE_DIR ? 0x4000 : 0x8000;
    inode->uid = 0; 
    inode->gid = 0; 
    inode->size = 0;
    inode->atime = timer_ticks;
    inode->ctime = timer_ticks;
    inode->mtime = timer_ticks;
    inode->blocks = 0;
    
    file_count++;
    return FS_SUCCESS;
}

int fs_delete_file(uint32_t inode_num) {
    if (inode_num == 0 || inode_num > MAX_INODES) return FS_ERROR;
    
    Inode* inode = &inodes[inode_num - 1];
    
    for (uint32_t i = 0; i < inode->blocks && i < INODE_DIRECT_BLOCKS; i++) {
        if (inode->block[i] != 0) {
            fs_free_block(inode->block[i]);
        }
    }
    
    for (int i = 0; i < file_count; i++) {
        if (files[i].inode == inode_num) {
            for (int j = i; j < file_count - 1; j++) {
                files[j] = files[j + 1];
            }
            file_count--;
            break;
        }
    }
    
    fs_free_inode(inode_num);
    return FS_SUCCESS;
}

int fs_write_file(uint32_t inode_num, const void* data, uint32_t size) {
    if (inode_num == 0 || inode_num > MAX_INODES) return FS_ERROR;
    
    Inode* inode = &inodes[inode_num - 1];
    uint32_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    
    if (blocks_needed > INODE_DIRECT_BLOCKS) return FS_ERROR;
    
    for (uint32_t i = 0; i < inode->blocks; i++) {
        if (inode->block[i] != 0) {
            fs_free_block(inode->block[i]);
        }
    }
    
    for (uint32_t i = 0; i < blocks_needed; i++) {
        int block_num = fs_alloc_block();
        if (block_num == -1) {
            for (uint32_t j = 0; j < i; j++) {
                fs_free_block(inode->block[j]);
            }
            return FS_ERROR;
        }
        inode->block[i] = block_num;
    }
    
    inode->blocks = blocks_needed;
    inode->size = size;
    inode->mtime = timer_ticks;
    
    uint32_t remaining = size;
    for (uint32_t i = 0; i < blocks_needed; i++) {
        uint32_t to_copy = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        memcpy(blocks[inode->block[i]], (char*)data + i * BLOCK_SIZE, to_copy);
        remaining -= to_copy;
    }
    
    return FS_SUCCESS;
}

int fs_read_file(uint32_t inode_num, void* buffer, uint32_t size) {
    if (inode_num == 0 || inode_num > MAX_INODES) return FS_ERROR;
    
    Inode* inode = &inodes[inode_num - 1];
    if (size > inode->size) size = inode->size;
    
    uint32_t remaining = size;
    for (uint32_t i = 0; i < inode->blocks && remaining > 0; i++) {
        uint32_t to_copy = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        memcpy((char*)buffer + i * BLOCK_SIZE, blocks[inode->block[i]], to_copy);
        remaining -= to_copy;
    }
    
    inode->atime = timer_ticks;
    return size;
}

int fs_change_dir(uint32_t inode_num) {
    for (int i = 0; i < file_count; i++) {
        if (files[i].inode == inode_num && files[i].type == FILE_DIR) {
            current_inode = inode_num;
            return FS_SUCCESS;
        }
    }
    return FS_ERROR;
}

void beep(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0xB6);
    outb(0x42, (uint8_t)(divisor & 0xFF));
    outb(0x42, (uint8_t)((divisor >> 8) & 0xFF));
    
    uint8_t tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

void no_sound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

void execute_fino() {
    terminal_clear();
    terminal_writestring("\nFikusOS Text Editor (fino) - ESC to exit\n");
    terminal_writestring("__________________________________________\n");
    
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
    strcpy(color_lower, color);
    for (char* p = color_lower; *p; p++) {
        if (*p >= 'A' && *p <= 'Z') *p += 32;
    }
    
    if (strcmp_case_insensitive(color_lower, "black") == 0) bg_color = COLOR_BLACK;
    else if (strcmp_case_insensitive(color_lower, "blue") == 0) bg_color = COLOR_BLUE;
    else if (strcmp_case_insensitive(color_lower, "green") == 0) bg_color = COLOR_GREEN;
    else if (strcmp_case_insensitive(color_lower, "cyan") == 0) bg_color = COLOR_CYAN;
    else if (strcmp_case_insensitive(color_lower, "red") == 0) bg_color = COLOR_RED;
    else if (strcmp_case_insensitive(color_lower, "magenta") == 0) bg_color = COLOR_MAGENTA;
    else if (strcmp_case_insensitive(color_lower, "brown") == 0) bg_color = COLOR_BROWN;
    else if (strcmp_case_insensitive(color_lower, "gray") == 0) bg_color = COLOR_GRAY;
    else if (strcmp_case_insensitive(color_lower, "white") == 0) bg_color = COLOR_WHITE;
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

void execute_ls() {
    terminal_writestring("\n");
    for (int i = 0; i < file_count; i++) {
        if (files[i].parent_inode == current_inode) {
            terminal_writestring(files[i].name);
            terminal_writestring(files[i].type == FILE_DIR ? "/ " : "  ");
        }
    }
    terminal_writestring("\n");
}

void execute_pwd() {
    char path[MAX_PATH_LEN] = "/";
    uint32_t inode = current_inode;
    
    while (inode != 1) { 
        for (int i = 0; i < file_count; i++) {
            if (files[i].inode == inode) {
                char temp[MAX_PATH_LEN];
                strcpy(temp, "/");
                strcat(temp, files[i].name);
                strcat(temp, path);
                strcpy(path, temp);
                
                inode = files[i].parent_inode;
                break;
            }
        }
    }
    
    terminal_writestring("\n");
    terminal_writestring(path);
    terminal_writestring("\n");
}

void execute_cd(char* dirname) {
    if (dirname == NULL || strlen(dirname) == 0) {
        current_inode = 1;
        return;
    }
    
    if (strcmp(dirname, "..") == 0) {
        for (int i = 0; i < file_count; i++) {
            if (files[i].inode == current_inode) {
                current_inode = files[i].parent_inode;
                return;
            }
        }
        terminal_writestring("\nAlready at root directory\n");
        return;
    }
    
    for (int i = 0; i < file_count; i++) {
        if (files[i].parent_inode == current_inode && 
            strcmp(files[i].name, dirname) == 0 && 
            files[i].type == FILE_DIR) {
            current_inode = files[i].inode;
            return;
        }
    }
    
    terminal_writestring("\nDirectory not found: ");
    terminal_writestring(dirname);
    terminal_writestring("\n");
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

void execute_time() {
    uint8_t hour = cmos_read(0x04);
    uint8_t minute = cmos_read(0x02);
    uint8_t second = cmos_read(0x00);
    
    hour = bcd_to_bin(hour);
    minute = bcd_to_bin(minute);
    second = bcd_to_bin(second);
    
    char hour_str[3], min_str[3], sec_str[3];
    int_to_str(hour, hour_str);
    int_to_str(minute, min_str);
    int_to_str(second, sec_str);
    
    terminal_writestring("Current time: ");
    terminal_writestring(hour_str);
    terminal_writestring(":");
    terminal_writestring(min_str);
    terminal_writestring(":");
    terminal_writestring(sec_str);
    terminal_writestring("\n");
}

void execute_whoami() {
    terminal_writestring("\nusr\n");
}

void execute_uptime() {
    uint32_t seconds = timer_ticks / TIMER_HZ;
    char sec_str[16];
    int_to_str(seconds, sec_str);
    terminal_writestring("\nUptime: ");
    terminal_writestring(sec_str);
    terminal_writestring(" seconds\n");
}

void execute_ver() {
    terminal_writestring("\nFikusOS version 0.4 Alpha\n");
    terminal_writestring("Build 2025-05-09\n");
}

void execute_pause() {
    terminal_writestring("\nPress any key to continue...");
    keyboard_getchar();
    terminal_writestring("\n");
}

void execute_cls() {
    terminal_clear();
}

void execute_exit() {
    terminal_writestring("\nLogging out...\n");
    asm volatile ("cli");
    asm volatile ("int $0xFF");
}

void execute_kptest() {
    kernel_panic("kernel panic");
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
    uint8_t temp = inb(0x64);
    while (temp & 0x02)
        temp = inb(0x64);
    outb(0x64, 0xFE);
    asm volatile ("cli");
    asm volatile ("int $0xFF");
    while (1);
}

void execute_opengwm() {
    gwm_mode = true;
    mouse_init();
    mouse_enabled = true;
    init_gwm();
    
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    terminal_clear();
    
    while (gwm_mode) {
        draw_gwm_desktop();
        
        if (inb(0x64) & 0x01) {
            uint8_t key = inb(0x60);
            
            if (key == KEY_ESC) {
                gwm_mode = false;
                terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
                terminal_clear();
                return;
            }
            
            if (key == 'w') {
                show_start_menu = !show_start_menu;
                selected_menu_item = -1;
            }
            
            if (show_start_menu) {
                if (key == KEY_UP) {
                    if (selected_menu_item > 0) selected_menu_item--;
                    else if (selected_menu_item == -1) selected_menu_item = program_count - 1;
                }
                else if (key == KEY_DOWN) {
                    if (selected_menu_item < program_count - 1) selected_menu_item++;
                    else if (selected_menu_item == -1) selected_menu_item = 0;
                }
                else if (key == KEY_ENTER && selected_menu_item >= 0) {
                    programs[selected_menu_item].open_func();
                }
            }
            
            if (active_window >= 0 && windows[active_window].handle_input) {
                windows[active_window].handle_input(key);
            }
        }
        
        if (mouse_enabled) {
            uint8_t status = inb(0x64);
            if (status & 0x20) {
                uint8_t data = inb(0x60);
                
                if (data == 0x9B) {
                    mouse_left_button = true;
                    
                    if (mouse_x >= 2 && mouse_x < 8 && mouse_y == SCREEN_HEIGHT - 1) {
                        show_start_menu = !show_start_menu;
                        selected_menu_item = -1;
                    }
                    else if (show_start_menu && mouse_x >= 3 && mouse_x < 21 && 
                             mouse_y >= SCREEN_HEIGHT - 6 - program_count && mouse_y < SCREEN_HEIGHT - 5) {
                        int item = mouse_y - (SCREEN_HEIGHT - 6 - program_count);
                        if (item >= 0 && item < program_count) {
                            programs[item].open_func();
                        }
                    }
                    else {
                        for (int i = 0; i < window_count; i++) {
                            if (mouse_x >= windows[i].x && mouse_x < windows[i].x + windows[i].width &&
                                mouse_y >= windows[i].y && mouse_y < windows[i].y + windows[i].height) {
                                activate_window(i);
                                break;
                            }
                        }
                    }
                } else if (data == 0x1B) {
                    mouse_left_button = false;
                }
                
                int8_t x_movement = (int8_t)inb(0x60);
                int8_t y_movement = (int8_t)inb(0x60);
                
                mouse_x += x_movement;
                mouse_y -= y_movement;
                
                if (mouse_x < 0) mouse_x = 0;
                if (mouse_x >= SCREEN_WIDTH) mouse_x = SCREEN_WIDTH - 1;
                if (mouse_y < 0) mouse_y = 0;
                if (mouse_y >= SCREEN_HEIGHT - 1) mouse_y = SCREEN_HEIGHT - 2;
                
                draw_pixel(mouse_x, mouse_y, COLOR_WHITE);
            }
        }
        
        asm volatile ("hlt");
    }
}

void show_ascii_art() {
    terminal_writestring("  ______ _ _               ____   _____ \n");
    terminal_writestring(" |  ____(_) |             / __ \\ / ____|\n");
    terminal_writestring(" | |__   _| | ___   _ ___| |  | | (___  \n");
    terminal_writestring(" |  __| | | |/ / | | / __| |  | |\\___ \\ \n");
    terminal_writestring(" | |    | |   <| |_| \\__ \\ |__| |____) |\n");
    terminal_writestring(" |_|    |_|_|\\_\\\\__,_|___/\\____/|_____/ \n");
    terminal_writestring("\n");
}

void execute_about() {
    show_ascii_art();
    terminal_writestring("OS: FikusOS\n");
    terminal_writestring("Version: Alpha 0.4\n");
    terminal_writestring("License: GNU GPL 2\n");
    terminal_writestring("Created for FikusPI\n");
    terminal_writestring("Shell: FKShell\n");
    terminal_writestring("Kernel: 0.0.9-Fikus (srunix fork)\n");
    terminal_writestring("\n");
}

void execute_calc() {
    terminal_writestring("\nFikusOS Calculator - Type 'q' to quit\n");
    terminal_writestring("Simple calc\n\n");
    
    char input[MAX_CMD_LEN];
    int pos = 0;
    
    while (1) {
        terminal_writestring("fkcalc> ");
        pos = 0;
        
        while (pos < MAX_CMD_LEN - 1) {
            char c = keyboard_getchar();
            
            if (c == '\n') {
                terminal_putchar('\n');
                input[pos] = '\0';
                
                if (strcmp(input, "q") == 0) {
                    return;
                }
                
                int a = 0, b = 0, result = 0;
                char op = '+';
                int i = 0;
                bool has_error = false;
                
                while (input[i] >= '0' && input[i] <= '9') {
                    a = a * 10 + (input[i] - '0');
                    i++;
                }
                
                if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
                    op = input[i];
                    i++;
                } else {
                    has_error = true;
                }
                
                while (input[i] >= '0' && input[i] <= '9') {
                    b = b * 10 + (input[i] - '0');
                    i++;
                }
                
                if (input[i] != '\0' || has_error) {
                    terminal_writestring("Error: Invalid expression format\n");
                    break;
                }
                
                switch(op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': 
                        if (b != 0) result = a / b;
                        else {
                            terminal_writestring("Error: Division by zero\n");
                            break;
                        }
                        break;
                    default:
                        terminal_writestring("Error: Invalid operator\n");
                        break;
                }
                
                char result_str[16];
                int_to_str(result, result_str);
                terminal_writestring("= ");
                terminal_writestring(result_str);
                terminal_writestring("\n");
                
                break;
            }
            else if (c == '\b') {
                if (pos > 0) {
                    pos--;
                    terminal_putchar('\b');
                }
            }
            else {
                input[pos++] = c;
                terminal_putchar(c);
            }
        }
    }
}

void execute_ping(char* ip_str) {
    uint8_t ip[4] = {0};
    char* token = ip_str;
    int i = 0;
    
    while (token && i < 4) {
        ip[i++] = atoi(token);
        token = strchr(token, '.');
        if (token) token++;
    }
    
    if (i != 4) {
        terminal_writestring("Invalid IP address format. Use x.x.x.x\n");
        return;
    }
    
    terminal_printf("Pinging %d.%d.%d.%d...\n", ip[0], ip[1], ip[2], ip[3]);
    
    int conn = net_connect(ip, 7);
    if (conn < 0) {
        terminal_writestring("Failed to create connection\n");
        return;
    }
    
    uint8_t packet[32] = "PING";
    if (net_send(conn, packet, 4) < 0) {
        terminal_writestring("Failed to send ping\n");
        net_disconnect(conn);
        return;
    }
    
    uint32_t start_time = timer_ticks;
    uint8_t reply[32];
    
    while (timer_ticks - start_time < TIMER_HZ * 2) {
        int reply_len = net_receive(conn, reply, sizeof(reply));
        if (reply_len > 0) {
            uint32_t ping_time = (timer_ticks - start_time) * 1000 / TIMER_HZ;
            terminal_printf("Reply from %d.%d.%d.%d: time=%dms\n", 
                          ip[0], ip[1], ip[2], ip[3], ping_time);
            net_disconnect(conn);
            return;
        }
    }
    
    terminal_writestring("Request timed out\n");
    net_disconnect(conn);
}

void execute_ifconfig() {
    terminal_printf("eth0: flags=UP BROADCAST RUNNING\n");
    terminal_printf("    MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", 
                   net_config.mac[0], net_config.mac[1], net_config.mac[2],
                   net_config.mac[3], net_config.mac[4], net_config.mac[5]);
    terminal_printf("    IP: %d.%d.%d.%d\n", 
                   net_config.ip[0], net_config.ip[1], 
                   net_config.ip[2], net_config.ip[3]);
    terminal_printf("    Subnet: %d.%d.%d.%d\n", 
                   net_config.subnet[0], net_config.subnet[1],
                   net_config.subnet[2], net_config.subnet[3]);
    terminal_printf("    Gateway: %d.%d.%d.%d\n", 
                   net_config.gateway[0], net_config.gateway[1],
                   net_config.gateway[2], net_config.gateway[3]);
    terminal_printf("    DNS: %d.%d.%d.%d\n", 
                   net_config.dns[0], net_config.dns[1],
                   net_config.dns[2], net_config.dns[3]);
}

void execute_netstat() {
    terminal_writestring("Active TCP connections:\n");
    terminal_writestring("Local Address          Foreign Address        State\n");
    
    for (int i = 0; i < MAX_NET_CONNECTIONS; i++) {
        if (tcp_connections[i].state == 1) {
            terminal_printf("%d.%d.%d.%d:%d    %d.%d.%d.%d:%d    ESTABLISHED\n",
                          net_config.ip[0], net_config.ip[1], net_config.ip[2], net_config.ip[3],
                          tcp_connections[i].local_port,
                          tcp_connections[i].remote_ip[0], tcp_connections[i].remote_ip[1],
                          tcp_connections[i].remote_ip[2], tcp_connections[i].remote_ip[3],
                          tcp_connections[i].remote_port);
        }
    }
}

void execute_sysinfo() {
    terminal_writestring("\n=== System Information ===\n");
    
    uint32_t used_memory_kb = (next_node_addr - 0x100000) / 1024;
    uint32_t free_memory_kb = TOTAL_MEMORY_KB - used_memory_kb;
    terminal_printf("Memory: %d KB used, %d KB free\n", used_memory_kb, free_memory_kb);
    
    uint32_t seconds = timer_ticks / TIMER_HZ;
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;
    terminal_printf("Uptime: %d hours, %d minutes, %d seconds\n", hours, minutes % 60, seconds % 60);
    
    terminal_writestring("\nDisks:\n");
    for (int i = 0; i < disk_count; i++) {
        terminal_printf("- %s: %d MB total, %d MB free\n", 
                      disks[i].name, 
                      (int)(disks[i].total_bytes / (1024 * 1024)),
                      (int)(disks[i].free_bytes / (1024 * 1024)));
    }
    
    terminal_writestring("\nNetwork:\n");
    terminal_printf("IP: %d.%d.%d.%d\n", 
                  net_config.ip[0], net_config.ip[1], 
                  net_config.ip[2], net_config.ip[3]);
    terminal_printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  net_config.mac[0], net_config.mac[1], net_config.mac[2],
                  net_config.mac[3], net_config.mac[4], net_config.mac[5]);
}

void execute_mkfile(char* filename) {
    if (fs_create_file(filename, current_inode, FILE_REGULAR) == FS_SUCCESS) {
        terminal_printf("\nFile '%s' created successfully\n", filename);
    } else {
        terminal_writestring("\nFailed to create file\n");
    }
}

void execute_mkdir(char* dirname) {
    if (fs_create_file(dirname, current_inode, FILE_DIR) == FS_SUCCESS) {
        terminal_printf("\nDirectory '%s' created successfully\n", dirname);
    } else {
        terminal_writestring("\nFailed to create directory\n");
    }
}

void execute_rm(char* name) {
    for (int i = 0; i < file_count; i++) {
        if (files[i].parent_inode == current_inode && strcmp(files[i].name, name) == 0) {
            if (fs_delete_file(files[i].inode) == FS_SUCCESS) {
                terminal_printf("\n'%s' deleted successfully\n", name);
            } else {
                terminal_writestring("\nFailed to delete\n");
            }
            return;
        }
    }
    terminal_printf("\nFile not found: %s\n", name);
}

void execute_beep() {
    beep(1000);
    for (volatile int i = 0; i < 1000000; i++);
    no_sound();
}

void execute_draw() {
    terminal_clear();
    terminal_writestring("Drawing test - Press any key to exit\n");
    
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        draw_pixel(i, 10, COLOR_RED);
    }
    
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        draw_pixel(40, i, COLOR_GREEN);
    }
    
    draw_line(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, COLOR_CYAN);
    draw_line(SCREEN_WIDTH-1, 0, 0, SCREEN_HEIGHT-1, COLOR_MAGENTA);
    
    keyboard_getchar();
    terminal_clear();
}

void execute_help(int page) {
    switch(page) {
        case 1:
            terminal_writestring("\nAvailable commands (page 1/3):\n");
            terminal_writestring("help [page] - Show this help\n");
            terminal_writestring("cls         - Clear screen\n");
            terminal_writestring("about       - Show system info\n");
            terminal_writestring("color       - Change background color\n");
            terminal_writestring("fino        - Text editor\n");
            terminal_writestring("dir         - List files\n");
            terminal_writestring("pwd         - Print working directory\n");
            terminal_writestring("cd          - Change directory\n");
            terminal_writestring("echo        - Display message\n");
            terminal_writestring("calc        - Simple calculator\n");
            terminal_writestring("date        - Show current date/time\n");
            terminal_writestring("time        - Show current time\n");
            terminal_writestring("ver         - Show version\n");
            terminal_writestring("Type 'help 2' for more commands\n");
            break;
        case 2:
            terminal_writestring("\nAvailable commands (page 2/3):\n");
            terminal_writestring("whoami      - Show current user\n");
            terminal_writestring("mem         - Show memory usage\n");
            terminal_writestring("disk        - Show disk information\n");
            terminal_writestring("pause       - Wait for keypress\n");
            terminal_writestring("poweroff    - Shut down\n");
            terminal_writestring("reboot      - Reboot\n");
            terminal_writestring("exit        - Log out\n");
            terminal_writestring("kptest      - Test kernel panic\n");
            terminal_writestring("ping        - Ping a network host\n");
            terminal_writestring("ifconfig    - Show network config\n");
            terminal_writestring("netstat     - Show network stats\n");
            terminal_writestring("Type 'help 3' for more commands\n");
            break;
        case 3:
            terminal_writestring("\nAvailable commands (page 3/3):\n");
            terminal_writestring("sysinfo     - Detailed system info\n");
            terminal_writestring("mkfile      - Create file\n");
            terminal_writestring("mkdir       - Create directory\n");
            terminal_writestring("rm          - Delete file\n");
            terminal_writestring("beep        - Play test sound\n");
            terminal_writestring("draw        - Drawing test\n");
            terminal_writestring("cd          - change directory\n");
            terminal_writestring("Type 'help 1' for previous commands\n");
            break;
        default:
            terminal_writestring("\nInvalid help page. Use 'help 1', 'help 2' or 'help 3'\n");
            break;
    }
}

void execute_command(char* cmd) {
    char* args[10];
    int arg_count = 0;
    
    char cmd_copy[MAX_CMD_LEN];
    strcpy(cmd_copy, cmd);
    
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
    
    if (strcmp_case_insensitive(args[0], "help") == 0) {
        int page = 1;
        if (arg_count > 1) {
            page = atoi(args[1]);
        }
        execute_help(page);
    }
    else if (strcmp_case_insensitive(args[0], "cls") == 0 || 
             strcmp_case_insensitive(args[0], "clear") == 0) {
        execute_cls();
    }
    else if (strcmp_case_insensitive(args[0], "about") == 0) {
        execute_about();
    }
    else if (strcmp_case_insensitive(args[0], "color") == 0) {
        if (arg_count > 1) execute_color(args[1]);
        else terminal_writestring("\nUsage: color <black|blue|gray|...>\n");
    }
    else if (strcmp_case_insensitive(args[0], "fino") == 0) {
        execute_fino();
    }
    else if (strcmp_case_insensitive(args[0], "dir") == 0 || 
             strcmp_case_insensitive(args[0], "ls") == 0) {
        execute_ls();
    }
    else if (strcmp_case_insensitive(args[0], "pwd") == 0) {
        execute_pwd();
    }
    else if (strcmp_case_insensitive(args[0], "cd") == 0) {
        if (arg_count > 1) execute_cd(args[1]);
        else execute_cd(NULL);
    }
    else if (strcmp_case_insensitive(args[0], "echo") == 0) {
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
    else if (strcmp_case_insensitive(args[0], "date") == 0) {
        execute_date();
    }
    else if (strcmp_case_insensitive(args[0], "time") == 0) {
        execute_time();
    }
    else if (strcmp_case_insensitive(args[0], "ver") == 0) {
        execute_ver();
    }
    else if (strcmp_case_insensitive(args[0], "whoami") == 0) {
        execute_whoami();
    }
    else if (strcmp_case_insensitive(args[0], "uptime") == 0) {
        execute_uptime();
    }
    else if (strcmp_case_insensitive(args[0], "mem") == 0 || 
             strcmp_case_insensitive(args[0], "memory") == 0) {
        execute_memory();
    }
    else if (strcmp_case_insensitive(args[0], "disk") == 0 || 
             strcmp_case_insensitive(args[0], "data") == 0) {
        execute_disk();
    }
    else if (strcmp_case_insensitive(args[0], "pause") == 0) {
        execute_pause();
    }
    else if (strcmp_case_insensitive(args[0], "calc") == 0) {
        execute_calc();
    }
    else if (strcmp_case_insensitive(args[0], "opengwm") == 0) {
        execute_opengwm();
    }
    else if (strcmp_case_insensitive(args[0], "poweroff") == 0 || 
             strcmp_case_insensitive(args[0], "shutdown") == 0) {
        execute_poweroff();
    }
    else if (strcmp_case_insensitive(args[0], "reboot") == 0 || 
             strcmp_case_insensitive(args[0], "restart") == 0) {
        execute_reboot();
    }
    else if (strcmp_case_insensitive(args[0], "exit") == 0) {
        execute_exit();
    }
    else if (strcmp_case_insensitive(args[0], "kptest") == 0) {
        execute_kptest();
    }
    else if (strcmp_case_insensitive(args[0], "ping") == 0) {
        if (arg_count > 1) execute_ping(args[1]);
        else terminal_writestring("\nUsage: ping <ip>\n");
    }
    else if (strcmp_case_insensitive(args[0], "ifconfig") == 0) {
        execute_ifconfig();
    }
    else if (strcmp_case_insensitive(args[0], "netstat") == 0) {
        execute_netstat();
    }
    else if (strcmp_case_insensitive(args[0], "sysinfo") == 0) {
        execute_sysinfo();
    }
    else if (strcmp_case_insensitive(args[0], "mkfile") == 0) {
        if (arg_count > 1) execute_mkfile(args[1]);
        else terminal_writestring("\nUsage: mkfile <filename>\n");
    }
    else if (strcmp_case_insensitive(args[0], "mkdir") == 0) {
        if (arg_count > 1) execute_mkdir(args[1]);
        else terminal_writestring("\nUsage: mkdir <dirname>\n");
    }
    else if (strcmp_case_insensitive(args[0], "rm") == 0) {
        if (arg_count > 1) execute_rm(args[1]);
        else terminal_writestring("\nUsage: rm <filename>\n");
    }
    else if (strcmp_case_insensitive(args[0], "beep") == 0) {
        execute_beep();
    }
    else if (strcmp_case_insensitive(args[0], "draw") == 0) {
        execute_draw();
    }
    else if (args[0][0] != '\0') {
        terminal_writestring("\nCommand not found: ");
        terminal_writestring(args[0]);
        terminal_writestring("\nType 'help' for available commands\n");
    }
}

void print_prompt() {
    terminal_setcolor(COLOR_GREEN, terminal_color >> 4);
    terminal_writestring("FikusOS-> ");
    terminal_setcolor(COLOR_WHITE, terminal_color >> 4);
    move_cursor(terminal_column, terminal_row);
}

void show_boot_screen() {
    terminal_setcolor(COLOR_WHITE, COLOR_BLACK);
    terminal_clear();
    
    terminal_writestring("\n\n\n");
    terminal_writestring("FikusOS\n");
    terminal_writestring("GNU Operating System\n\n");
    terminal_writestring("Copyright GPL (c) 2025 FikusOS\n\n\n");
    
    terminal_setcolor(COLOR_GREEN, COLOR_BLACK);
    terminal_writestring("Starting system...\n\n");
    
    const char* boot_messages[] = {
        "Loading kernel...",
        "Initializing memory...",
        "Detecting hardware...",
        "Initializing keyboard...",
        "Initializing mouse...",
        "Starting filesystem...",
        "Loading drivers...",
        "Starting network...",
        "Preparing shell..."
    };
    
    for (int i = 0; i < 9; i++) {
        terminal_writestring("[ OK ] ");
        terminal_writestring(boot_messages[i]);
        terminal_writestring("\n");
        
        for (volatile int j = 0; j < 1000000; j++);
    }
    
    terminal_writestring("\nSystem ready. Press any key to continue...");
    keyboard_getchar();
    
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    terminal_clear();
}

void shell() {
    char cmd[MAX_CMD_LEN];
    int pos = 0;
    
    show_ascii_art();
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

void kernel_panic(const char* message) {
    terminal_setcolor(COLOR_RED, COLOR_WHITE);
    terminal_clear();
    
    terminal_writestring("\n\nKERNEL PANIC!\n");
    terminal_writestring(message);
    terminal_writestring("\nSystem is crashed. Please restart your computer.\n");
    
    while (1) {
        asm volatile ("cli; hlt");
    }
}

void kernel_main() {
    terminal_initialize();
    show_boot_screen();
    
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    terminal_clear();
    
    terminal_writestring("Initializing kernel...\n");
    init_timer();
    net_init();
    
    memset(inodes, 0, sizeof(inodes));
    memset(block_used, 0, sizeof(block_used));
    free_blocks = MAX_BLOCKS;
    free_inodes = MAX_INODES;
    
    fs_create_file("root", 1, FILE_DIR);
    current_inode = 1;
    
    fs_create_file("bin", 1, FILE_DIR);
    fs_create_file("etc", 1, FILE_DIR);
    fs_create_file("home", 1, FILE_DIR);
    
    fs_create_file("readme", 1, FILE_REGULAR);
    fs_create_file("fino", 1, FILE_REGULAR);
    
    terminal_writestring("Starting shell...\n\n");
    shell();
    
    while (1) asm volatile ("hlt");
}
