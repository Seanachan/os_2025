savedcmd_osfs.o := ld -EL  -maarch64elf -z noexecstack --no-warn-rwx-segments   -r -o osfs.o @osfs.mod 
