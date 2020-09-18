C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o sprites.o "sprites/sprites.c"
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o bkgSprites.o "sprites/bkgSprites.c"
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o main.o main.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -Wl-yp0x143=0x80 -o main.gb sprites.o bkgSprites.o main.o
copy main.gb "C:\Users\jonathan\Documents\BGB\main.gb" /y 

del /f /q /s *.lst
del /f /q /s *.o
del /f /q /s *.map
del /f /q /s *.sym