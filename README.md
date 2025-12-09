# My Custom Userspace VPN
still working on it
I don't like the idea of the local network admin seeing every DNS lookup I make.
I decided to build my own VPN from scratch in C to learn how this stuff actually works under the hood.

## Why i made this?
**Privacy**

## How it works
It's a custom daemon written in C that:
1.  Creates a virtual network interface (`tun0`) on Linux
2.  Hijacks IP packets from the kernel before they hit the WiFi card
3.  Encrypts them using **Libsodium** (ChaCha20-Poly1305)
4.  Yeets them over UDP to a server so the local router just sees random garbage data

### Dependencies
Just install `libsodium-dev`

### Compile
```Bash
make clean
make
```

### Test it
Terminal 1:
```Bash
sudo ./bin/vpn
```
Terminal 2:
Once it's running, open a new terminal and force traffic through the tunnel:
```Bash
sudo ip route add 8.8.8.8 dev tun0
ping 8.8.8.8
```
![2025-12-08 21-16-07-VEED](https://github.com/user-attachments/assets/87accba0-8094-4d82-b2f4-98aa1ce8c47d)
