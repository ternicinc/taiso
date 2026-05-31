#!/usr/bin/env bash
set -e

mkdir -p build/output
mkdir -p build/iso/boot/grub
mkdir -p build/images

gcc -m32 -ffreestanding -fno-pie -fno-stack-protector \
  -c kernel/main.c \
  -o build/output/main.o

ld -m elf_i386 \
  -T kernel/linker.ld \
  -o build/output/tawilos.bin \
  build/output/main.o

cp build/output/tawilos.bin build/iso/boot/tawilos.bin

cat > build/iso/boot/grub/grub.cfg <<EOF
set timeout=0
set default=0

menuentry "TawilOS Mobile" {
    multiboot /boot/tawilos.bin
    boot
}
EOF

grub-mkrescue -o build/images/tawilos.iso build/iso

echo "Built build/images/tawilos.iso"
