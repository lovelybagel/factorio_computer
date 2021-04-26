var a
var b
var c
peripheral top_display 769
peripheral low_display 770
entry
ST  1   a
ST  1   low_display
top
LD  0   a
ADD 0   b
ST  0   c
LD  0   b
ST  0   a
LD  0   c
ST  0   b
LD  0   a
ST  0   top_display
LD  0   b
ST  0   low_display
JMP top 0