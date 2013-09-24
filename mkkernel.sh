make -C /home/engle/android/ME722-JB/kernel/moto/milestone2 O=/home/engle/android/ME722-JB/out/target/product/milestone2/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE="/home/engle/android/ME722-JB/prebuilts/misc/linux-x86/ccache/ccache /home/engle/android/ME722-JB/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi-" mapphone_defconfig

make -C /home/engle/android/ME722-JB/kernel/moto/milestone2 O=/home/engle/android/ME722-JB/out/target/product/milestone2/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE="/home/engle/android/ME722-JB/prebuilts/misc/linux-x86/ccache/ccache /home/engle/android/ME722-JB/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi-" headers_install

make -C /home/engle/android/ME722-JB/kernel/moto/milestone2 O=/home/engle/android/ME722-JB/out/target/product/milestone2/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE="/home/engle/android/ME722-JB/prebuilts/misc/linux-x86/ccache/ccache /home/engle/android/ME722-JB/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi-" zImage

make -C /home/engle/android/ME722-JB/kernel/moto/milestone2 O=/home/engle/android/ME722-JB/out/target/product/milestone2/obj/KERNEL_OBJ ARCH=arm CROSS_COMPILE="/home/engle/android/ME722-JB/prebuilts/misc/linux-x86/ccache/ccache /home/engle/android/ME722-JB/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi-" modules

make -C /home/engle/android/ME722-JB/kernel/moto/milestone2 O=/home/engle/android/ME722-JB/out/target/product/milestone2/obj/KERNEL_OBJ INSTALL_MOD_PATH=../../system ARCH=arm CROSS_COMPILE="/home/engle/android/ME722-JB/prebuilts/misc/linux-x86/ccache/ccache /home/engle/android/ME722-JB/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi-" modules_install

#make PREFIX=/home/engle/android/ME722-JB//home/engle/android/ME722-JB/out/target/product/milestone2/obj/kernel_intermediates/build ARCH=arm CROSS_COMPILE=/home/engle/android/ME722-JB/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi- HOST_PLATFORM=zoom2 KERNEL_DIR=/home/engle/android/ME722-JB/out/target/product/milestone2/obj/KERNEL_OBJ  clean -C /home/engle/android/ME722-JB/system/wlan/ti/wilink_6_1/platforms/os/linux

