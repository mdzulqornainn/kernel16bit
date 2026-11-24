# Milestone 1 — Sistem Operasi 16-bit

Sistem operasi 16-bit sederhana yang berjalan di emulator **Bochs**. Fitur:

- Bootloader yang memuat kernel dari floppy ke memori `0x1000:0000`
- `printString` — cetak string ke layar (BIOS `INT 0x10`, `AH=0x0E`)
- `readString` — baca input keyboard sampai Enter, mendukung Backspace (BIOS `INT 0x16`)
- `clearScreen` — bersihkan layar, warna putih (`0x07`), kursor ke kiri atas
- System call lewat `INT 0x21` (`AX=0` → printString, `AX=1` → readString)

---

## 1. Kebutuhan

- Ubuntu (native, VMware/VirtualBox, atau WSL2 dengan WSLg)
- Tools:

```bash
sudo apt update
sudo apt install -y nasm bcc bin86 bochs bochs-x bochsbios vgabios make git
```

| Tool | Fungsi |
|------|--------|
| `nasm` | Assembler untuk bootloader dan `kernel.asm` |
| `bcc` | Compiler C 16-bit |
| `ld86` (paket `bin86`) | Linker 16-bit |
| `bochs`, `bochs-x` | Emulator x86 + tampilan GUI |
| `make` | Otomatisasi build |

Cek instalasi:

```bash
nasm -v
bcc -v
ld86 -v
ls /usr/share/bochs/   # harus ada BIOS-bochs-latest dan VGABIOS-lgpl-latest
```

---

## 2. Struktur Folder

```
.
├── makefile
├── README.md
├── .gitignore
├── src/
│   ├── asm/
│   │   ├── bootloader.asm   # boot sector (sektor 0)
│   │   └── kernel.asm       # putInMemory, interrupt, makeInterrupt21
│   ├── c/
│   │   ├── kernel.c         # main, printString, readString, clearScreen, handleInterrupt21
│   │   ├── kernel.h
│   │   ├── std_lib.c        # div, mod, strlen, strcmp, strcpy, memcpy, clear
│   │   ├── std_lib.h
│   │   └── std_type.h       # byte, bool, true, false
│   └── config/
│       ├── if2230.config    # konfigurasi Bochs
│       └── debug.rc         # perintah otomatis debugger Bochs ("c" = continue)
└── out/                     # hasil build (dibuat otomatis, tidak di-commit)
```

---

## 3. Cara Menjalankan

```bash
git clone <url-repo>
cd <nama-repo>
make build-run
```

Hasil yang diharapkan:

1. Layar bersih, muncul `Selamat datang di OS Milestone 1!`
2. Muncul prompt `> ` — ketik teks (Backspace bisa dipakai), tekan Enter
3. Muncul `Kamu mengetik: <teks>` lalu prompt baru

Untuk keluar: tutup jendela Bochs, atau tekan `Ctrl+C` di terminal lalu ketik `q`.

### Target Makefile

| Perintah | Fungsi |
|----------|--------|
| `make diskimage` | Buat `out/system.img` (floppy 1.44 MB) |
| `make bootloader` | Assemble bootloader, tulis ke sektor 0 |
| `make stdlib` | Compile `std_lib.c` |
| `make kernel` | Compile + link kernel, tulis mulai sektor 1 |
| `make build` | `diskimage` + `bootloader` + `kernel` |
| `make run` | Jalankan Bochs |
| `make build-run` | Build lalu jalankan |
| `make clean` | Hapus folder `out/` |

---

## 4. Alur Boot

```
CPU reset (F000:FFF0)
   → BIOS
   → baca sektor 0 floppy ke 0x0000:7C00  (bootloader)
   → bootloader baca 15 sektor (mulai sektor 1) ke 0x1000:0000
   → jmp 0x1000:0000  → main() di kernel.c
```

Catatan teknis:

- `ld86 -d` membuang header, jadi **`main()` harus fungsi pertama** di `kernel.c`.
- `bcc -ansi` hanya mengizinkan deklarasi variabel di awal blok.
- Alamat fisik = `segment * 0x10 + offset` (contoh: `0xB000:0x8000` = `0xB8000`, memori video teks).

---

## 5. Troubleshooting

| Gejala | Penyebab & Solusi |
|--------|-------------------|
| Muncul `<bochs:1>` dan berhenti | Debugger Bochs menunggu perintah. Ketik `c` + Enter. Dengan `debug.rc` ini otomatis. |
| Setelah `c`, kembali ke `f000:fff0` dan log berisi `3rd (13) exception` | ROM BIOS/VGA tidak cocok. Pastikan config memakai `/usr/share/bochs/BIOS-bochs-latest` dan `/usr/share/bochs/VGABIOS-lgpl-latest`. Jika masih gagal, ganti ke `BIOS-bochs-legacy`. |
| Macet di "Booting from floppy" | Cek `ls -l out/kernel` (harus > 1000 byte) dan pastikan `make build` sukses. Debug: `Ctrl+C` di terminal → `r` → lihat `CS:IP`. |
| `Can't open display` | GUI tidak tersedia. Di WSL: `export DISPLAY=:0`. Di VM: jalankan dari terminal desktop, bukan SSH. |
| `display library 'x' not available` | `sudo apt install bochs-x` |
| `missing separator` saat `make` | Indentasi makefile harus TAB, bukan spasi. |
| Pesan `ALSA lib ...` | Hanya soal audio, aman diabaikan. |

Log lengkap Bochs ada di `out/bochsout.txt`.

---

## 6. Pengumpulan

- Jangan commit file binary (`out/`, `*.o`, `*.img`) — sudah diatur di `.gitignore`.
- Release dengan tag `v.1.0.0`:

```bash
git tag v.1.0.0
git push origin v.1.0.0
```
