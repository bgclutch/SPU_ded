        in
        pop     ax
        in
        pop     bx
        in
        pop     cx

        push    ax
        push    0
        jne     TWOROOTS

        push    bx
        push    0
        jne     ONEROOT

        push    cx
        push    0
        jne     NOROOTS
        push    9999
        out
        hlt

NOROOTS:
        push    0
        out
        hlt

ONEROOT:
        push    cx
        push    0
        jne     ROOTBX
        hlt
ROOTBX:
        push    cx
        push    bx
        div
        out
        hlt



TWOROOTS:

        call    DISCR

        push    dx
        push    0

        jae     ABOVEEQUZERO

        push    999999
        out
        hlt



ABOVEEQUZERO:
        push    dx
        push    0
        je      ZERODISCR

        push    dx
        sqrt
        pop     dx


        push    bx
        push    -1
        mul

        push    dx
        sub

        push    2
        push    ax
        mul

        div
        out

        push    bx
        push    -1
        mul

        push    dx
        add

        push    2
        push    ax
        mul

        div
        out
        hlt



ZERODISCR:

        push    -1
        push    bx
        mul

        push    ax
        push    2
        mul
        div
        out
        hlt



DISCR:

        push    bx
        push    bx
        mul

        push    -4
        push    ax
        mul
        push    cx
        mul

        add

        pop     dx

        ret