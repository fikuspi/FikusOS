.global _start
_start:
    # Код инициализации
    call kernel_main
    cli
    hlt
