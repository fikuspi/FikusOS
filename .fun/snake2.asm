section .data
    width equ 20
    height equ 20
    initial_length equ 3
    
    up equ 0
    right equ 1
    down equ 2
    left equ 3
    
    snake_char db 'O'
    food_char db '@'
    wall_char db '#'
    space_char db ' '
    newline db 10
    
    game_over_msg db 'Game Over! Score: '
    game_over_len equ $ - game_over_msg
    controls_msg db 'WASD to move, Q to quit'
    controls_len equ $ - controls_msg
    
section .bss
    snake resb width*height*2
    food resb 2
    direction resb 1
    length resb 1
    score resb 1
    input_buffer resb 1
    seed resd 1

section .text
global _start

_start:
    call init_game
    
game_loop:
    call draw
    call handle_input
    call update
    
    mov eax, 162
    mov ebx, timespec
    mov ecx, 0
    int 0x80
    
    jmp game_loop

init_game:
    mov byte [length], initial_length
    mov byte [direction], right
    
    mov esi, snake
    mov byte [esi], width/2
    mov byte [esi+1], height/2
    mov byte [esi+2], width/2-1
    mov byte [esi+3], height/2
    mov byte [esi+4], width/2-2
    mov byte [esi+5], height/2
    
    call spawn_food
    
    mov byte [score], 0
    ret

draw:
    mov ecx, 30
.clear_loop:
    push ecx
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    pop ecx
    loop .clear_loop
    
    mov ecx, width+2
.top_wall:
    push ecx
    mov eax, 4
    mov ebx, 1
    mov ecx, wall_char
    mov edx, 1
    int 0x80
    pop ecx
    loop .top_wall
    
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    
    mov ebx, 0
.y_loop:
    cmp ebx, height
    je .y_done
    
    push ebx
    mov eax, 4
    mov ebx, 1
    mov ecx, wall_char
    mov edx, 1
    int 0x80
    pop ebx
    
    mov ecx, 0
.x_loop:
    cmp ecx, width
    je .x_done
    
    call is_snake
    cmp al, 1
    je .draw_snake
    
    call is_food
    cmp al, 1
    je .draw_food
    
    push ebx
    push ecx
    mov eax, 4
    mov ebx, 1
    mov ecx, space_char
    mov edx, 1
    int 0x80
    pop ecx
    pop ebx
    jmp .next_x
    
.draw_snake:
    push ebx
    push ecx
    mov eax, 4
    mov ebx, 1
    mov ecx, snake_char
    mov edx, 1
    int 0x80
    pop ecx
    pop ebx
    jmp .next_x
    
.draw_food:
    push ebx
    push ecx
    mov eax, 4
    mov ebx, 1
    mov ecx, food_char
    mov edx, 1
    int 0x80
    pop ecx
    pop ebx
    
.next_x:
    inc ecx
    jmp .x_loop
    
.x_done:
    push ebx
    mov eax, 4
    mov ebx, 1
    mov ecx, wall_char
    mov edx, 1
    int 0x80
    
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    pop ebx
    
    inc ebx
    jmp .y_loop
    
.y_done:
    mov ecx, width+2
.bottom_wall:
    push ecx
    mov eax, 4
    mov ebx, 1
    mov ecx, wall_char
    mov edx, 1
    int 0x80
    pop ecx
    loop .bottom_wall
    
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    
    mov eax, 4
    mov ebx, 1
    mov ecx, controls_msg
    mov edx, controls_len
    int 0x80
    
    ret

handle_input:
    mov eax, 3
    mov ebx, 0
    mov ecx, input_buffer
    mov edx, 1
    int 0x80
    
    cmp eax, 0
    jle .no_input
    
    mov al, [input_buffer]
    cmp al, 'w'
    je .set_up
    cmp al, 'a'
    je .set_left
    cmp al, 's'
    je .set_down
    cmp al, 'd'
    je .set_right
    cmp al, 'q'
    je exit
    jmp .no_input
    
.set_up:
    mov byte [direction], up
    jmp .no_input
.set_left:
    mov byte [direction], left
    jmp .no_input
.set_down:
    mov byte [direction], down
    jmp .no_input
.set_right:
    mov byte [direction], right
    
.no_input:
    ret

update:
    mov esi, snake
    mov al, [esi]
    mov bl, [esi+1]
    
    cmp byte [direction], up
    je .move_up
    cmp byte [direction], right
    je .move_right
    cmp byte [direction], down
    je .move_down
    cmp byte [direction], left
    je .move_left
    
.move_up:
    dec bl
    jmp .move_done
.move_right:
    inc al
    jmp .move_done
.move_down:
    inc bl
    jmp .move_done
.move_left:
    dec al
    
.move_done:
    cmp al, 0
    jl game_over
    cmp al, width-1
    jge game_over
    cmp bl, 0
    jl game_over
    cmp bl, height-1
    jge game_over
    
    call is_snake
    cmp al, 1
    je game_over
    
    call is_food
    cmp al, 1
    je .eat_food
    
    movzx ecx, byte [length]
    dec ecx
    shl ecx, 1
    add ecx, snake
    mov byte [ecx], 0
    mov byte [ecx+1], 0
    jmp .move_snake
    
.eat_food:
    inc byte [length]
    inc byte [score]
    call spawn_food
    
.move_snake:
    movzx ecx, byte [length]
    dec ecx
    shl ecx, 1
    mov esi, snake
    add esi, ecx
    
.move_loop:
    cmp ecx, 0
    je .move_head
    mov dl, [esi-2]
    mov dh, [esi-1]
    mov [esi], dl
    mov [esi+1], dh
    sub esi, 2
    sub ecx, 2
    jmp .move_loop
    
.move_head:
    mov [esi], al
    mov [esi+1], bl
    
    ret

spawn_food:
    mov eax, [seed]
    mov ebx, 1103515245
    mul ebx
    add eax, 12345
    mov [seed], eax
    
    xor edx, edx
    mov ebx, width
    div ebx
    mov [food], dl
    
    mov eax, [seed]
    mov ebx, 1103515245
    mul ebx
    add eax, 12345
    mov [seed], eax
    xor edx, edx
    mov ebx, height
    div ebx
    mov [food+1], dl
    
    call is_snake
    cmp al, 1
    je spawn_food
    
    ret

is_snake:
    movzx ecx, byte [length]
    mov esi, snake
    
.check_loop:
    cmp ecx, 0
    je .not_found
    cmp al, [esi]
    jne .next
    cmp bl, [esi+1]
    jne .next
    mov al, 1
    ret
    
.next:
    add esi, 2
    dec ecx
    jmp .check_loop
    
.not_found:
    mov al, 0
    ret

is_food:
    cmp al, [food]
    jne .not_food
    cmp bl, [food+1]
    jne .not_food
    mov al, 1
    ret
    
.not_food:
    mov al, 0
    ret

game_over:
    mov eax, 4
    mov ebx, 1
    mov ecx, game_over_msg
    mov edx, game_over_len
    int 0x80
    
    movzx eax, byte [score]
    call print_number
    
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    
    jmp exit

print_number:
    mov ecx, 10
    xor ebx, ebx
    
.divide:
    xor edx, edx
    div ecx
    add dl, '0'
    push edx
    inc ebx
    test eax, eax
    jnz .divide
    
.print:
    pop ecx
    push ebx
    mov eax, 4
    mov ebx, 1
    mov edx, 1
    int 0x80
    pop ebx
    dec ebx
    jnz .print
    
    ret

exit:
    mov eax, 1
    xor ebx, ebx
    int 0x80

section .data
    timespec:
        tv_sec  dd 0
        tv_nsec dd 100000000
