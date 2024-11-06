in
draw
pop ax

push 0
push 0
pop bx
pop cx

NEXT:

call SUM

push ax
push ax

mul

call FILL


call INSIDE

push bx
push cx

mul

push 512

jbe NEXT
draw
hlt


FILL:

je LOC
ret

LOC:
push bx
push 31
mul

push cx
add
pop dx
pop [ dx ]
ret


OUTSIDE:

push cx
push 16

jbe PTR
ret

PTR:
push 1
push cx
add
pop cx
ret

INSIDE:

push bx
push 32

ja HERE
push 0
pop bx
call OUTSIDE
ret

HERE:
push 1
push bx
add
pop bx
ret


SUM:
push cx
push 8
sub

pop dx
push dx
push dx

mul

push bx
push 16
sub

pop dx
push dx
push dx

mul

add

ret