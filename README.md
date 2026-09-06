# LobsterCrypt

A lightweight, low-overhead command-line encryption utility implemented in C (POSIX standard), targeted for Linux systems and Android via the Native Development Kit (NDK).

Designed for secure file processing with minimal memory footprint, utilizing streaming buffer chunking to avoid loading entire binary payloads into RAM.

## Technical Architecture

* **Buffer Streaming Pipeline:** Processes arbitrary-sized files using fixed-size chunk buffers (64 KB paging) to ensure bounded memory consumption regardless of input file size.
* **I/O Subsystem:** Direct POSIX system calls (`read`, `write`, `open`) with explicit file descriptor control, bypassing standard library buffering overhead where applicable.
* **Cross-Compilation:** Clean target decoupling between glibc (Linux desktop/server) and Bionic libc (Android NDK standalone toolchain).
* **Deterministic Cleanup:** Explicit zeroization of sensitive key material in memory (`explicit_bzero` / secure memory scrubbing) prior to heap deallocation.

## CLI Usage

### Encryption Mode
```bash
./lobstercrypt -e -i input.bin -o encrypted.bin -k "YourSecretKey"
```

### Decryption Mode
```bash
./lobstercrypt -d -i encrypted.bin -o decrypted.bin -k "YourSecretKey"
```

### Options
* `-e` : Enable encryption mode.
* `-d` : Enable decryption mode.
* `-i <path>` : Path to input file.
* `-o <path>` : Path to output file.
* `-k <key>`  : Symmetric key passphrase.
* `-h` : Display usage information.

## Building and Compilation

### Native Linux (GCC / Clang)

#### Using CMake:
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

#### Manual Compilation:
```bash
gcc -O3 -Wall -Wextra -pedantic src/*.c -Iinclude -o lobstercrypt
```

### Android NDK (Cross-Compilation)
To compile for `aarch64-linux-android`:
```bash
$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android33-clang \
    -O3 -Wall -Wextra src/*.c -Iinclude -o lobstercrypt_arm64
```

## Security Design Principles

1. **Zero Heap Spills:** Key schedules and temporary transformation buffers reside on the stack where possible and are cleansed immediately post-operation.
2. **POSIX File Permissions:** Output files are created with restricted file masks (`0600`) to prevent unauthorized multi-user reading on shared hosts.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.ium and ImGui
