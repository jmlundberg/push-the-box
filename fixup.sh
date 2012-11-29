mv -f x86-32/libCorradePluginManager.so.0{.2,}
mv -f x86-32/libCorradeUtility.so.0{.2,}
mv -f x86-64/libCorradePluginManager.so.0{.2,}
mv -f x86-64/libCorradeUtility.so.0{.2,}

/usr/nacl/toolchain/linux_x86_glibc/bin/x86_64-nacl-strip x86-64/* x86-32/* *.nexe
