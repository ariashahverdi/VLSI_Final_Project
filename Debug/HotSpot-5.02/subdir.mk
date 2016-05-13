################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HotSpot-5.02/RCutil.c \
../HotSpot-5.02/flp.c \
../HotSpot-5.02/flp_desc.c \
../HotSpot-5.02/hotspot.c \
../HotSpot-5.02/npe.c \
../HotSpot-5.02/package.c \
../HotSpot-5.02/shape.c \
../HotSpot-5.02/sim-template.c \
../HotSpot-5.02/temperature.c \
../HotSpot-5.02/temperature_block.c \
../HotSpot-5.02/temperature_grid.c \
../HotSpot-5.02/util.c \
../HotSpot-5.02/wire.c 

OBJS += \
./HotSpot-5.02/RCutil.o \
./HotSpot-5.02/flp.o \
./HotSpot-5.02/flp_desc.o \
./HotSpot-5.02/hotspot.o \
./HotSpot-5.02/npe.o \
./HotSpot-5.02/package.o \
./HotSpot-5.02/shape.o \
./HotSpot-5.02/sim-template.o \
./HotSpot-5.02/temperature.o \
./HotSpot-5.02/temperature_block.o \
./HotSpot-5.02/temperature_grid.o \
./HotSpot-5.02/util.o \
./HotSpot-5.02/wire.o 

C_DEPS += \
./HotSpot-5.02/RCutil.d \
./HotSpot-5.02/flp.d \
./HotSpot-5.02/flp_desc.d \
./HotSpot-5.02/hotspot.d \
./HotSpot-5.02/npe.d \
./HotSpot-5.02/package.d \
./HotSpot-5.02/shape.d \
./HotSpot-5.02/sim-template.d \
./HotSpot-5.02/temperature.d \
./HotSpot-5.02/temperature_block.d \
./HotSpot-5.02/temperature_grid.d \
./HotSpot-5.02/util.d \
./HotSpot-5.02/wire.d 


# Each subdirectory must supply rules for building sources it contributes
HotSpot-5.02/%.o: ../HotSpot-5.02/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


