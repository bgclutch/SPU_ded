            in
            pop ax

            push 0
            pop bx
            push 0
            pop cx

NEXT:

            call FILL

            call INCREMENT

            push bx
            push cx
            mul

            push 2040

            jb NEXT
            draw
            hlt


FILL:

    push cx
    push -16
    add
    push cx
    push -16
    add
    mul

    push bx
    push -30
    add
    push bx
    push -30
    add
    mul

    add

    push ax
    push ax
    mul

    jae QUIT

    push cx
    push 64
    mul

    push bx
    add

    pop dx
    pop [ dx ]
    ret

QUIT:
    ret


INCREMENT:

    push bx
    push 64

    jae ADDRBX
    push 1
    push bx
    add
    pop bx
    ret

ADDRBX:
    push 0
    pop bx

    push cx
    push 32
    jae STOP
    push 1
    push cx
    add
    pop cx
    ret

STOP:
    ret