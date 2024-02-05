.include "hdr.asm"

.section ".rodata1" superfree

Player1:
.incbin "Player1.pic"
Player1_end:

palPlayer1:
.incbin "Player1.pal"
palPlayer1_end:

background:
.incbin "background.map"
background_end:

pbackground:
.incbin "background.pal"
pbackground_end:

tbackground:
.incbin "background.pic"
tbackground_end:

.ends

.section ".rodata2" superfree

tileset:
.incbin "Tiled.pic"
tilesetend:

tilesetpal:
.incbin "Tiled.pal"

map:
.incbin "Map.m16"

tilesetatt:
.incbin "MapT.b16"

tilesetdef:
.incbin "Map0M.t16"

.ends
