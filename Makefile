KernelSourceFile := $(shell find Source -name Kernel.c)
KernelObjectFile := $(patsubst Source/Kernel.c, Build/Kernel.o, $(KernelSourceFile))

BootSourceFiles := $(shell find Source/Boot -name *.asm)
BootObjectFiles := $(patsubst Source/Boot/%.asm, Build/Boot/%.o, $(BootSourceFiles))

CpuSourceFiles := $(shell find Source/System/Cpu -name *.c)
CpuObjectFiles := $(patsubst Source/System/Cpu/%.c, Build/System/Cpu/%.o, $(CpuSourceFiles))

DriverSourceFiles := $(shell find Source/System/Drivers -name *.c)
DriverObjectFiles := $(patsubst Source/System/Drivers/%.c, Build/System/Drivers/%.o, $(DriverSourceFiles))

ObjectFiles := $(BootObjectFiles) $(CpuObjectFiles) $(DriverObjectFiles)

$(KernelObjectFile): Build/%.o : Source/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I Headers -ffreestanding $(patsubst Build/Kernel.o, Source/Kernel.c, $@) -o $@

$(BootObjectFiles): Build/Boot/%.o : Source/Boot/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst Build/Boot/%.o, Source/Boot/%.asm, $@) -o $@

$(CpuObjectFiles): Build/System/Cpu/%.o : Source/System/Cpu/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I Headers -ffreestanding $(patsubst Build/System/Cpu/%.o, Source/System/Cpu/%.c, $@) -o $@

$(DriverObjectFiles): Build/System/Drivers/%.o : Source/System/Drivers/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I Headers -ffreestanding $(patsubst Build/System/Drivers/%.o, Source/System/Drivers/%.c, $@) -o $@

.PHONY: build
build: $(KernelObjectFile) $(ObjectFiles)
	mkdir -p Output && \
	x86_64-elf-ld -n -o Output/Kernel64.bin -T Linker.ld $(KernelObjectFile) $(ObjectFiles) && \
	cp Output/Kernel64.bin Grub/boot/Kernel64.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o Output/SimpleOS.iso Grub