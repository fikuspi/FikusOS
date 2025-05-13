section .data
    board times 9 db ' '
    newline db 10
    prompt db 'Enter move (1-9): ',0
    x_wins db 'X wins!',10,0
    o_wins db 'O wins!',10,0
    draw_msg db 'Draw!',10,0
    x_turn db 'X turn',10,0
    o_turn db 'O turn',10,0
    invalid db 'Invalid move!',10,0

section .bss
    input resb 2
    current_player resb 1

section .text
global _start

_start:
    mov byte [current_player], 'X'

game_loop:
    call draw_board
    call print_turn
    call get_move
    call update_board
    call check_winner
    jmp game_loop

draw_board:
    mov ecx, 3
    mov esi, 0

.draw_row:
    push ecx
    mov ecx, 3
    mov edi, 0

.draw_cell:
    push ecx
    mov eax, 4
    mov ebx, 1
    lea ecx, [board + esi + edi]
    mov edx, 1
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, space_char
    mov edx, 1
    int 0x80

    pop ecx
    inc edi
    loop .draw_cell

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    pop ecx
    add esi, 3
    loop .draw_row
    ret

print_turn:
    cmp byte [current_player], 'X'
    je .print_x_turn

    mov eax, 4
    mov ebx, 1
    mov ecx, o_turn
    mov edx, 7
    int 0x80
    ret

.print_x_turn:
    mov eax, 4
    mov ebx, 1
    mov ecx, x_turn
    mov edx, 7
    int 0x80
    ret

get_move:
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt
    mov edx, 18
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, input
    mov edx, 2
    int 0x80

    ret

update_board:
    mov al, [input]
    sub al, '1'
    cmp al, 0
    jl .invalid
    cmp al, 8
    jg .invalid

    movzx ebx, al
    cmp byte [board + ebx], ' '
    jne .invalid

    mov al, [current_player]
    mov [board + ebx], al
    ret

.invalid:
    mov eax, 4
    mov ebx, 1
    mov ecx, invalid
    mov edx, 14
    int 0x80
    jmp get_move

check_winner:
    call check_rows
    call check_columns
    call check_diagonals
    call check_draw
    ret

check_rows:
    mov ecx, 0

.row_loop:
    mov eax, ecx
    imul eax, 3
    mov al, [board + eax]
    cmp al, ' '
    je .next_row

    mov bl, [board + eax + 1]
    cmp al, bl
    jne .next_row

    mov bl, [board + eax + 2]
    cmp al, bl
    jne .next_row

    call declare_winner
    ret

.next_row:
    inc ecx
    cmp ecx, 3
    jl .row_loop
    ret

check_columns:
    mov ecx, 0

.col_loop:
    mov al, [board + ecx]
    cmp al, ' '
    je .next_col

    mov bl, [board + ecx + 3]
    cmp al, bl
    jne .next_col

    mov bl, [board + ecx + 6]
    cmp al, bl
    jne .next_col

    call declare_winner
    ret

.next_col:
    inc ecx
    cmp ecx, 3
    jl .col_loop
    ret

check_diagonals:
    mov al, [board]
    cmp al, ' '
    je .check_second

    mov bl, [board + 4]
    cmp al, bl
    jne .check_second

    mov bl, [board + 8]
    cmp al, bl
    jne .check_second

    call declare_winner
    ret

.check_second:
    mov al, [board + 2]
    cmp al, ' '
    je .diag_done

    mov bl, [board + 4]
    cmp al, bl
    jne .diag_done

    mov bl, [board + 6]
    cmp al, bl
    jne .diag_done

    call declare_winner
    ret

.diag_done:
    ret

check_draw:
    mov ecx, 0

.draw_loop:
    cmp byte [board + ecx], ' '
    je .not_draw
    inc ecx
    cmp ecx, 9
    jl .draw_loop

    mov eax, 4
    mov ebx, 1
    mov ecx, draw_msg
    mov edx, 6
    int 0x80
    call exit
    ret

.not_draw:
    ret

declare_winner:
    cmp byte [current_player], 'X'
    je .x_wins

    mov eax, 4
    mov ebx, 1
    mov ecx, o_wins
    mov edx, 8
    int 0x80
    jmp .game_end

.x_wins:
    mov eax, 4
    mov ebx, 1
    mov ecx, x_wins
    mov edx, 8
    int 0x80

.game_end:
    call draw_board
    call exit
    ret

switch_player:
    cmp byte [current_player], 'X'
    je .switch_to_o

    mov byte [current_player], 'X'
    ret

.switch_to_o:
    mov byte [current_player], 'O'
    ret

exit:
    mov eax, 1
    xor ebx, ebx
    int 0x80

section .data
    space_char db '|'
