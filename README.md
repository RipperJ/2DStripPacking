# 2DFloorplanningAlgorithms

## Environment
* System Checking
    * Ubuntu OS: 20.04.5 LTS
    * Linux: 5.14.0-1054-oem
    * g++ 9.4.0
* To install **Gurobi** on Linux
* Python packages (for visualization)
    * matplotlib 3.5.1
* To enable `#include <graphics.h>` in C++
    1. `sudo apt-get install build-essential`
    2. `sudo apt-get install libsdl-image1.2 libsdl-image1.2-dev guile-2.0 guile-2.0-dev libsdl1.2debian libart-2.0-dev libaudiofile-dev libdirectfb-dev libdirectfb-extra libfreetype6-dev libxext-dev x11proto-xext-dev libfreetype6 libaa1 libaa1-dev libslang2-dev libasound2 libasound2-dev`
    3. `sudo vim /etc/apt/sources.list`, and paste the following sources
        * deb http://us.archive.ubuntu.com/ubuntu/ xenial main universe
        * deb-src http://us.archive.ubuntu.com/ubuntu/ xenial main universe
    4. `sudo apt update`
    5. `sudo apt install libesd0-dev`
    6. `wget http://download.savannah.gnu.org/releases/libgraph/libgraph-1.0.2.tar.gz`
    7. `tar zxvf libgraph-1.0.2.tar.gz`
    8. `cd libgraph-1.0.2/`
    9. `CPPFLAGS="$CPPFLAGS $(pkg-config --cflags-only-I guile-2.0)" CFLAGS="$CFLAGS $(pkg-config --cflags-only-other guile-2.0)" LDFLAGS="$LDFLAGS $(pkg-config --libs guile-2.0)" ./configure`
    10. `make`
    11. `sudo make install`
    12. `sudo cp /usr/local/lib/libgraph.* /usr/lib`

## How to Run
* To generate floorplan:
    * Standard output: required floorplan report in terminal
    * Log file for visualization: *.log without "verbose" keyword in [./log](./log) folder
    * Detailed log: *.log with "verbose" keyword in [./log](./log) folder
```bash
make
./floorplan ./testcase/500.txt
```
* To visualize certain log:
```bash
python visualize.py ./log/<xxx>.log
```

## Reference

## Useful and Interesting Links
* [How to compile C++ with Gurobi on Linux](https://www.leandro-coelho.com/how-to-compile-c-with-gurobi-in-linux)
* Gurobi Documents
    * [Gurobi Optimizer Reference Manual](https://www.gurobi.com/wp-content/plugins/hd_documentations/documentation/10.0/refman.pdf)
    * [C++ API Example -- mip1_c++.cpp](https://www.gurobi.com/documentation/10.0/quickstart_windows/cpp_example_mip1_cpp_cpp.html)
* [How to install "graphics.h" on Linux](https://urwithajit9.medium.com/adding-graphic-h-for-graphic-programming-using-c-c-in-ubuntu-20-4-65840fa6347b)