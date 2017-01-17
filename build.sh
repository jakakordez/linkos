./compile.sh

#qemu-system-i386 -kernel myos.bin

rm -rf out
mkdir out
mkdir out/iso
mkdir out/iso/boot
mkdir out/iso/boot/grub
cp bin/linkos.bin out/iso/boot/linkos.bin
cp src/grub.cfg out/iso/boot/grub/grub.cfg
grub-mkrescue -o out/linkos.iso out/iso

#qemu-system-i386 -cdrom out/linkos.iso

VBoxManage startvm "LinkOS"
