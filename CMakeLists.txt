cmake_minimum_required(VERSION 3.10)
project(init9009 VERSION 0.9)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_C_COMPILER "/usr/local/arm/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")
set(tools "/usr/local/arm/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin")
#add_subdirectory(ad9528_driver)
#add_subdirectory(Talise_API_ARM_v6_0_6_0_2)
add_executable(init9009_guochan main.c k7_spi.c
    YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_plat_hal.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_init.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_jesd204b.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_radio.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_cals.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_error.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_riscv.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_init.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_agc.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_cals.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_rx.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_tx.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_gpio.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_hal.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_info.c
	YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_config.c

                )
add_executable(set9009 set9009.c)
#set(CMAKE_C_COMPILER "/opt/ftools/FMQL-Linux-SDK_20210816/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")
#set(tools "/opt/ftools/FMQL-Linux-SDK_20210816/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf/bin")
#add_executable(bit2bin_lnx bit2bin.cpp)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -static -static-libgcc -static-libstdc++")
#set(CMAKE_CXX_COMPILER "/usr/bin/x86_64-w64-mingw32-g++")
#set(tools "/usr/bin/")
#find_package(Threads)
#add_subdirectory(io_ctrl)
#add_executable(bit2bin_win bit2bin.cpp)
#target_link_libraries(test_oc PUBLIC io_ctrl)
