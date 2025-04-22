#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

void* memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
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

int atoi(const char* str) {
    int res = 0;
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
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

#define FILE_REGULAR 0
#define FILE_DIR     1
#define FILE_SYMLINK 2

#define FS_SUCCESS 0
#define FS_ERROR   1

typedef struct {
    char name[32];
    uint64_t total_bytes;
    uint64_t free_bytes;
} Disk;

typedef struct {
    char name[32];
    uint32_t size;
    uint8_t type;
    uint32_t inode;
    uint32_t parent_inode;
} File;

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

int mouse_x = 40;
int mouse_y = 12;
bool mouse_enabled = false;
bool mouse_left_button = false;
bool mouse_right_button = false;
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

void kernel_main();
void kernel_panic(const char* message);

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
    while (*data) {
        terminal_putchar(*data++);
    }
}

static size_t str_len(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

static int str_cmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
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

void execute_ls() {
    terminal_writestring("\n");
    for (int i = 0; i < file_count; i++) {
        terminal_writestring(files[i].name);
        terminal_writestring(files[i].type ? "/ " : "  ");
    }
    terminal_writestring("\n");
}

void execute_pwd() {
    terminal_writestring("\n/\n");
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
    terminal_writestring("\nFikusOS version 0.3 Alpha\n");
    terminal_writestring("Build 2025-04-22\n");
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
    terminal_setcolor(COLOR_WHITE, COLOR_GRAY);
    terminal_clear();
   
    terminal_writestring("\n\n  FikusOS Mouse Test\n");
    terminal_writestring("  ---------------------------------\n\n");
    terminal_writestring("  Reboot pc to exit\n");
    terminal_writestring("  Use mouse to move cursor (X)\n");
    terminal_writestring("  Left click to select text\n\n");
    
    mouse_init();
    mouse_enabled = true;
    
    int prev_x = mouse_x;
    int prev_y = mouse_y;
    bool selecting = false;
    
    while (1) {
        if (inb(0x64) & 0x01) {
            uint8_t status = inb(0x64);
            if (status & 0x20) {
                uint8_t data = inb(0x60);
                
                if (data == 0x9B) {
                    mouse_left_button = true;
                    mouse_start_x = mouse_x;
                    mouse_start_y = mouse_y;
                    selecting = true;
                } else if (data == 0x1B) {
                    mouse_left_button = false;
                    selecting = false;
                    
                    for (int y = 0; y < SCREEN_HEIGHT; y++) {
                        for (int x = 0; x < SCREEN_WIDTH; x++) {
                            if (terminal_buffer[y * SCREEN_WIDTH + x] & 0x7000) {
                                terminal_buffer[y * SCREEN_WIDTH + x] &= 0x8FFF;
                            }
                        }
                    }
                }
                
                if (mouse_enabled) {
                    terminal_buffer[prev_y * SCREEN_WIDTH + prev_x] = ' ' | (COLOR_GRAY << 8);
                    
                    int8_t x_movement = (int8_t)inb(0x60);
                    int8_t y_movement = (int8_t)inb(0x60);
                    
                    mouse_x += x_movement;
                    mouse_y -= y_movement;
                    
                    if (mouse_x < 0) mouse_x = 0;
                    if (mouse_x >= SCREEN_WIDTH) mouse_x = SCREEN_WIDTH - 1;
                    if (mouse_y < 0) mouse_y = 0;
                    if (mouse_y >= SCREEN_HEIGHT) mouse_y = SCREEN_HEIGHT - 1;
                    
                    if (selecting) {
                        int start_x = mouse_start_x < mouse_x ? mouse_start_x : mouse_x;
                        int end_x = mouse_start_x < mouse_x ? mouse_x : mouse_start_x;
                        int start_y = mouse_start_y < mouse_y ? mouse_start_y : mouse_y;
                        int end_y = mouse_start_y < mouse_y ? mouse_y : mouse_start_y;
                        
                        for (int y = 0; y < SCREEN_HEIGHT; y++) {
                            for (int x = 0; x < SCREEN_WIDTH; x++) {
                                if (y >= start_y && y <= end_y && x >= start_x && x <= end_x) {
                                    terminal_buffer[y * SCREEN_WIDTH + x] |= 0x7000;
                                } else {
                                    terminal_buffer[y * SCREEN_WIDTH + x] &= 0x8FFF;
                                }
                            }
                        }
                    }
                    
                    terminal_buffer[mouse_y * SCREEN_WIDTH + mouse_x] = 'X' | (COLOR_BLACK << 8) | (COLOR_WHITE << 12);
                    prev_x = mouse_x;
                    prev_y = mouse_y;
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
    terminal_writestring("Version: Alpha 0.3\n");
    terminal_writestring("License: GNU GPL 2\n");
    terminal_writestring("Created for FikusPI\n");
    terminal_writestring("Shell: FKShell\n");
    terminal_writestring("Kernel: 0.0.8-Fikus\n");
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
                
                if (str_cmp(input, "q") == 0) {
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

void execute_help(int page) {
    switch(page) {
        case 1:
            terminal_writestring("\nAvailable commands (page 1/2):\n");
            terminal_writestring("help [page] - Show this help\n");
            terminal_writestring("cls         - Clear screen\n");
            terminal_writestring("about       - Show system info\n");
            terminal_writestring("color       - Change background color\n");
            terminal_writestring("fino        - Text editor\n");
            terminal_writestring("dir         - List files\n");
            terminal_writestring("pwd         - Print working directory\n");
            terminal_writestring("echo        - Display message\n");
            terminal_writestring("calc        - Simple calculator\n");
            terminal_writestring("date        - Show current date/time\n");
            terminal_writestring("time        - Show current time\n");
            terminal_writestring("ver         - Show version\n");
            terminal_writestring("Type 'help 2' for more commands\n");
            break;
        case 2:
            terminal_writestring("\nAvailable commands (page 2/2):\n");
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
            terminal_writestring("Type 'help 1' for previous commands\n");
            break;
        default:
            terminal_writestring("\nInvalid help page. Use 'help 1' or 'help 2'\n");
            break;
    }
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
        int page = 1;
        if (arg_count > 1) {
            page = atoi(args[1]);
        }
        execute_help(page);
    }
    else if (str_cmp_case_insensitive(args[0], "cls") == 0 || 
             str_cmp_case_insensitive(args[0], "clear") == 0) {
        execute_cls();
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
    else if (str_cmp_case_insensitive(args[0], "time") == 0) {
        execute_time();
    }
    else if (str_cmp_case_insensitive(args[0], "ver") == 0) {
        execute_ver();
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
    else if (str_cmp_case_insensitive(args[0], "pause") == 0) {
        execute_pause();
    }
    else if (str_cmp_case_insensitive(args[0], "calc") == 0) {
        execute_calc();
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
    else if (str_cmp_case_insensitive(args[0], "exit") == 0) {
        execute_exit();
    }
    else if (str_cmp_case_insensitive(args[0], "kptest") == 0) {
        execute_kptest();
    }
    else if (str_cmp_case_insensitive(args[0], "ping") == 0) {
        if (arg_count > 1) execute_ping(args[1]);
        else terminal_writestring("\nUsage: ping <ip>\n");
    }
    else if (str_cmp_case_insensitive(args[0], "ifconfig") == 0) {
        execute_ifconfig();
    }
    else if (str_cmp_case_insensitive(args[0], "netstat") == 0) {
        execute_netstat();
    }
    else if (args[0][0] != '\0') {
        terminal_writestring("\nCommand not found: ");
        terminal_writestring(args[0]);
        terminal_writestring("\nType 'help' for available commands\n");
    }
}

void print_prompt() {
    terminal_setcolor(COLOR_GREEN, terminal_color >> 4);
    terminal_writestring("Fikus~# ");
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
    wait_for_enter();
    
    terminal_setcolor(COLOR_WHITE, COLOR_BLUE);
    show_ascii_art();
    
    terminal_writestring("Initializing kernel...\n");
    init_timer();
    net_init();
    
    terminal_writestring("Starting shell...\n\n");
    shell();
    
    while (1) asm volatile ("hlt");
}
