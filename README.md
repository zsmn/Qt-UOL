# Qt-MSN

To run this application you must have QT5 installed. Run the command bellow to install.

```bash
sudo apt install qt5-default
```
## Compiling and running Server

```bash
cd Qt-UOL
cd server
sh protobuf.sh
mkdir build
cd build
qmake ..
make
./server
```


## Compiling and running Client

```bash
cd Qt-UOL
cd client
sh protobuf.sh
mkdir build
cd build
qmake ..
make
./client
```


