# recipes-watchpointmem

The module give a possibility to set watch point on neccessary address and see backtrace when happens some changes there (read/write)

## Usage

To run module use command `modprobe watchpointmem watch_addr=<your_address>`  
To see backtrace use command `dmesg | awk '/Watchpointmem start dump/,/Watchpointmem finish dump/'`
