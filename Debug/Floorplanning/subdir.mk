################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Floorplanning/SA_helper.c 

OBJS += \
./Floorplanning/SA_helper.o 

C_DEPS += \
./Floorplanning/SA_helper.d 


# Each subdirectory must supply rules for building sources it contributes
Floorplanning/%.o: ../Floorplanning/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


