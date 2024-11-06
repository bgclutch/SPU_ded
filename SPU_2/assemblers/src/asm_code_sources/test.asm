draw
push 0
pop ax
pop [ ax ]

NEXT:

call BIGDRAW
pop [ ax ]

push ax
push 510
jb NEXT

draw
hlt



BIGDRAW:

push 5
push ax
add
pop ax
ret