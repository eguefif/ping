# Ping

A minimal implementation of the `ping` utility written in C. This program sends ICMP echo requests to a given IP address or fully qualified domain name (FQDN) and displays the response time.

## Features

- Send ICMP echo requests (ping)
- Accepts both IP addresses and FQDNs
- Prints response time for each ping
- Statistics summary
- DNS reverse lookup

## Requirements

- GCC or another C compiler
- Root privileges (needed to send raw ICMP packets)
- Unix-like operating system (Linux, macOS, etc.)

## Build

```sh
gcc -o simple_ping simple_ping.c
```

## Usage
```sh
sudo ./simple_ping <IP or FQDN>
```

## Author
Emmanuel Guefif
