@echo off
if [%1]==[justcompile] goto :justcompile

echo Generating Pals
..\..\utils\mkts.exe mode=pals pals=..\gfx\palss1.png out=work\palfg1.h label=palss1 silent
..\..\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palbg0.h label=palts0 silent
..\..\utils\mkts.exe mode=pals pals=..\gfx\palts1.png out=work\palbg1.h label=palts1 silent
..\..\utils\mkts.exe mode=pals pals=..\gfx\palts2.png out=work\palbg2.h label=palts2 silent
..\..\utils\mkts.exe mode=pals pals=..\gfx\pal_ending.png out=work\palbg_ending.h label=pal_ending silent
copy /b work\palbg0.h + work\palbg1.h + work\palbg2.h + work\palfg1.h + work\palbg_ending.h assets\palettes.h > nul

echo Exporting chr
cd ..\gfx

echo bank 0 (tileset)
..\..\utils\mkts.exe mode=scripted in=cut_tiles.spt out=tschr.bin silent
..\..\utils\fillto.exe tschr.bin 4096

echo bank 1 (spriteset)
..\..\utils\mkts.exe mode=scripted in=cut_sprites.spt out=sschr.bin silent
..\..\utils\fillto.exe sschr.bin 4096

echo Juntando pichas
copy /b tschr.bin + sschr.bin ..\dev\tileset.chr > nul
del tschr.bin > nul
del sschr.bin > nul

cd ..\dev\

echo Exporting enems
cd ..\enems
..\..\utils\eneexp3c.exe level0.ene ..\dev\assets\enems0.h 0 1 nohotspots
..\..\utils\eneexp3c.exe level1.ene ..\dev\assets\enems1.h 1 1 nohotspots
..\..\utils\eneexp3c.exe level2.ene ..\dev\assets\enems2.h 2 1 nohotspots

echo Making map 
cd ..\map
..\..\utils\rle53map.exe in=level0.map out=..\dev\assets\map0.h size=5,2 prefix=0 scrsizes
..\..\utils\rle53map.exe in=level1.map out=..\dev\assets\map1.h size=6,5 prefix=1 scrsizes
..\..\utils\rle53map.exe in=level2.map out=..\dev\assets\map2.h size=5,6 prefix=2 scrsizes

rem echo Making texts
rem cd ..\texts
rem ..\..\utils\texts2array.exe dialogue.txt ..\dev\assets\texts.h texts0 19

cd ..\dev

..\..\utils\msc4nes.exe in=..\script\cave.spt out=..\dev\engine\mscnes.h maxpants=30 mapadjust=1

:justcompile

cc65 -Oi game.c --add-source
ca65 crt0.s -o crt0.o
ca65 game.s
ld65 -v -C nes.cfg -o cave.nes crt0.o game.o runtime.lib -Ln labels.txt

copy ..\..\tontis\dev\tileset.chr tileset_e.chr > nul

cc65 -Oi game.c --add-source -D CNROM
ca65 crt0-cnrom.s -o crt0.o
ca65 game.s
ld65 -v -C nes-cnrom.cfg -o  ..\..\..\standalone\cave-cnrom.nes crt0.o game.o runtime.lib -Ln labels.txt


cc65 -Oi game.c --add-source -D CNROM -D LANG_ES
ca65 crt0-cnrom.s -o crt0.o
ca65 game.s
ld65 -v -C nes-cnrom.cfg -o  ..\..\..\standalone\cave-cnrom_es.nes crt0.o game.o runtime.lib -Ln labels.txt

rem del game.s
del game.o
del crt0.o
