SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER avr-gcc)

SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-g")
SET(CWARN "-Wall")
SET(CTUNING "-mcall-prologues -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT "-Os")
SET(CINCS "-I../../VirtualSerial")
SET(CMCU "-mmcu=atmega32u4")
SET(CDEFS "-DF_CPU=16000000")
SET(LDFLAGS "-Wl,-gc-sections -Wl,-relax")
SET(LDINCS CINCS)
SET(LDPARAMS "-lVirtualSerial")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")

SET(CMAKE_C_FLAGS ${CFLAGS})

project(Test)
add_executable(Test src/main.c) 
