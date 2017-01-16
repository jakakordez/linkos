export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

rm -rf bin
mkdir bin
i686-elf-gcc -c src/ACPI.c -o bin/ACPI.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/keyboard.c -o bin/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/io.c -o bin/io.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/windows.c -o bin/windows.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/mem.c -o bin/mem.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/memory.c -o bin/memory.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/string.c -o bin/string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/cli.c -o bin/cli.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/kernel.c -o bin/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i686-elf-gcc -c src/programs/programs.c -o bin/programs.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/programs/terminal.c -o bin/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/programs/charmap.c -o bin/charmap.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/programs/colors.c -o bin/colors.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i686-elf-as src/boot.s -o bin/boot.o

i686-elf-gcc -T src/linker.ld -o bin/linkos.bin -ffreestanding -O2 -nostdlib bin/*.o -lgcc
