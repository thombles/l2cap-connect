## Get the required info

Run the Rust cars app. While trying to connect it will show something like:

```
[DEBUG] 2021-09-20T05:11:16.673Z: L2CAP Server 58cdf443-0838-0000-0000-000000000000 PSM is 192. Attempting to connect
```

Thus the address is `58:CD:F4:43:08:38` and the PSM is `192`. This assumes an iOS peer. Note that it will rotate its BDADDR every 15 minutes or so.

## Run the C version

```
cd c
gcc connect.c -o connect -lbluetooth -g
./connect 58:CD:F4:43:08:38 192
```

## Run the Rust version

```
cd connect-rs
cargo run 58:CD:F4:43:08:38 192
```
