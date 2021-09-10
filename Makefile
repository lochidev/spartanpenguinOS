BUILD=build/

C_SOURCES=$(wildcard kernel/*.c drivers/*.c helpers/*.c)
OBJ=${C_SOURCES:.c=.o}

C_FLAGS=-fno-PIC -m32 -ffreestanding
LD_FLAGS=-no-PIE -m elf_i386 --oformat binary -Ttext 0x1000
NASMFLAGS=-f elf32

QEMU=qemu-system-i386 --curses
all: dir kernel.bin boot.bin os-image os

run: all
	$(QEMU) -hda ${BUILD}os.img

os-image:
	cat ${BUILD}boot.bin ${BUILD}kernel.bin > ${BUILD}os-image

os: $(BUILD)os-image
	dd if=/dev/zero bs=1024 count=1440 > ${BUILD}$@.img
	dd if=$< of=${BUILD}$@.img conv=notrunc

boot.bin: boot/boot_sect.asm
	nasm $(NASM_FLAGS) $< -f bin -I 'boot/' -o ${BUILD}$@

%kernel_entry.o: kernel/kernel_entry.asm
	nasm $< $(NASMFLAGS) -o kernel/kernel_entry.o

kernel.bin: kernel/kernel_entry.o $(OBJ)
	ld $(LD_FLAGS) -o ${BUILD}$@ $^

%.o: %.c
	gcc $(C_FLAGS) -c $< -o $@

dir:
	mkdir -p $(BUILD)

clean:
	@rm -f kernel/*.o drivers/*.o helpers/*.o
	@rm -r $(BUILD)