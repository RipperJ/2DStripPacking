# 2DFloorplanningAlgorithms

## Environment
* System Checking
    * Ubuntu OS: 20.04.5 LTS
    * Linux: 5.14.0-1054-oem
    * g++ 9.4.0
* To install **Gurobi** on Linux
    * Obtain an academic license at [https://www.gurobi.com/downloads/end-user-license-agreement-academic/](https://www.gurobi.com/downloads/end-user-license-agreement-academic/)
    * Set the environment variables:
    ```bash
    # Gurobi Basics
    export GUROBI_HOME=<path-to-gurobi-home-directory>
    export PATH="${GUROBI_HOME}/bin:${PATH}"
    export LD_LIBRARY_PATH="${GUROBI_HOME}/lib:${LD_LIBRARY_PATH}"
    # For '#include "gurobi_c++.h"' in C++ source code
    export CPATH="${GUROBI_HOME}/include:${CPATH}"
    # License
    export GRB_LICENSE_FILE=<path-to-license-file-.lic>
    ```
* Python packages (for visualization)
    * matplotlib 3.5.1
* To enable `#include <graphics.h>` in C++ (poor visualization quality, ***Optional***)
    1. `sudo apt-get install build-essential`
    2. `sudo apt-get install libsdl-image1.2 libsdl-image1.2-dev guile-2.0 guile-2.0-dev libsdl1.2debian libart-2.0-dev libaudiofile-dev libdirectfb-dev libdirectfb-extra libfreetype6-dev libxext-dev x11proto-xext-dev libfreetype6 libaa1 libaa1-dev libslang2-dev libasound2 libasound2-dev`
    3. `sudo vim /etc/apt/sources.list`, and paste the following sources
        ```text
        deb http://us.archive.ubuntu.com/ubuntu/ xenial main universe
        deb-src http://us.archive.ubuntu.com/ubuntu/ xenial main universe
        ```
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
* To compile the C++ code:
    ```bash
    make
    ```
* To run all the testcases one-and-for-all
    * `python run_all.py` is an integrated script calling the C++ executable repetitively
    * Note that we only run MILP for 10-block case in this script because others lead to timeout (>2 hours)
* To generate floorplan for specific cases:
    ```bash
    ./floorplan ./testcase/50.txt 0 7000
    # 0 for MILP, 7000 means the program will terminate in 7000 seconds if not optimally solved yet

    # ./floorplan ./testcase/<testcase_name> <alg_type> <sub_alg_type> <sorting_type>, e.g.,
    ./floorplan ./testcase/500.txt 1 0 0
    ```
    * Standard output: the required floorplan report in terminal
    * Log file for visualization: *.log (without "verbose" keyword) in [./log](./log) folder
    * Detailed log (only for MILP): *.log with "verbose" keyword in [./log](./log) folder
* To visualize all logs and save images in the directory [./img](./img):
    ```bash
    python vis_all.py
    ```
* To visualize certain log:
    ```bash
    python visualize.py ./log/<xxx>.log
    # output in base directory as "floorplan.png"
    ```
* To see the best solutions under each algorithm:
    ```bash
    python find_best.py
    # standard output in the terminal for each testcase, and for each type of algorithm
    ```

## Reference

## Useful and Interesting Links
* [How to compile C++ with Gurobi on Linux](https://www.leandro-coelho.com/how-to-compile-c-with-gurobi-in-linux)
* Gurobi Documents
    * [Gurobi Optimizer Reference Manual](https://www.gurobi.com/wp-content/plugins/hd_documentations/documentation/10.0/refman.pdf)
    * [C++ API Example -- mip1_c++.cpp](https://www.gurobi.com/documentation/10.0/quickstart_windows/cpp_example_mip1_cpp_cpp.html)
* [How to install "graphics.h" on Linux](https://urwithajit9.medium.com/adding-graphic-h-for-graphic-programming-using-c-c-in-ubuntu-20-4-65840fa6347b)