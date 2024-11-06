        in
        dump
        pop     ax
        dump
        call    FACT
        out
        hlt

FACT:
        push    ax
        push    0
        ja      NEXT
        dump
        push    1
        ret
NEXT:
        push    ax
        push    ax
        push    1
        dump
        sub
        pop     ax
        dump
        call    FACT
        mul
        dump
        ret
