SCTP Basic Example
==================
This is a simple example that shows how to build a client/server application using SCTP transport protocol.

The server sends to the client two packets on two different stream: 
- on the stream 0 it sends the LOCAL time hour. 
- on the stream 1 it sends the GMT time hour.

This example shows a simple usage of the SCTP's multi-streaming capability.

Usage
==================

After cloning the repo, supposing you've cloned it in your $HOME folder:

```
cd $HOME/sctp_basic_example
mkdir build
cd build
cmake ..
make
```

The executables are in the build/ folder.

Run the server:

```
./sctp_server $SERVER_ADDR
```
Run the client:

```
./sctp_client $SERVER_ADDR
```



