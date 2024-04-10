Param (
	[Switch]$Run
)

Remove-Item -Path ./game.exe -Force -ErrorAction Ignore

gcc -g main.c -o game.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

if ($Run.IsPresent) {
	./game.exe
}