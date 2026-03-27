# ipsentinel
## A lightweight smarthome network security monitor written in C. Tracks incoming IP connections, detects suspicious activity, and persists IP data to disk with integrity verification.

## Features

- Tracks IPv4 and IPv6 connections via an embedded HTTP server
- Flags suspicious IPs (Work in progress)
- Persists IP entries to a binary file with SHA-256 checksum verification (Work in progress)
- Dynamic hashmap for in-memory IP storage

## Dependencies

- [libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) - embedded HTTP server
- [OpenSSL](https://www.openssl.org/) (>= 3.0) - SHA-256 checksum via EVP API

## Building
``` sh
mkdir build && cd build
cmake -G Ninja -DCMAKE_C_COMPILER=clang ..
ninja
./svr
```

## Configuration
Edit the relevant config headers before building:
| Header | Purpose |
|---|---|
| `config_ip.h` | IP file path, enum flags |
| `server_config.h` | Port, homepage route |
| `hashmap.h` | FNV offset and prime, starting capacity |
## How it works

1. An HTTP server starts and listens on the configured port.
2. Each incoming connection's IP is recorded in a dynamic hash map.
3. On shutdown, the IP list is written to a binary file with a SHA-256 checksum.
4. On startup, the file is read back and the checksum is verified before use.

## File format
```
[magic number (8 bytes)] [entry count (8 bytes)] [ipEntry × count] [SHA-256 checksum (32 bytes)]
```

## TODO

- [ ] Automatic IP ban integration
- [x] Writing and reading IP list
- [ ] Rate limiting
- [ ] Anomaly detection
- [x] Replace the dynamic array with a hash map
- [ ] Finish and tidy up the project

## License

This project is licensed under the **MIT License**.

This means you are free to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of this software, with or without modification, as long as you include the original copyright notice and this permission notice in any copy or substantial portion of the software.

The software is provided **as-is**, without any warranty of any kind.

See [LICENSE](./LICENSE) for the full license text.
