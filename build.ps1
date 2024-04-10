if ((Test-Path -Path ./game.exe) -eq $true) {
    Remove-Item -Path ./game.exe -Force
}
gcc -g main.c -o game.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
./game.exe