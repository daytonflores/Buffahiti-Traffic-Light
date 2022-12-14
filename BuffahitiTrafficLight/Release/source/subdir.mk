################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/fsm_trafficlight.c \
../source/led.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/systick.c \
../source/touch.c \
../source/tpm.c 

C_DEPS += \
./source/fsm_trafficlight.d \
./source/led.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/systick.d \
./source/touch.d \
./source/tpm.d 

OBJS += \
./source/fsm_trafficlight.o \
./source/led.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/systick.o \
./source/touch.o \
./source/tpm.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BuffahitiTrafficLight\board" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BuffahitiTrafficLight\source" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BuffahitiTrafficLight" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BuffahitiTrafficLight\drivers" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BuffahitiTrafficLight\CMSIS" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BuffahitiTrafficLight\utilities" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\BuffahitiTrafficLight\startup" -O0 -fno-common -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/fsm_trafficlight.d ./source/fsm_trafficlight.o ./source/led.d ./source/led.o ./source/main.d ./source/main.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/systick.d ./source/systick.o ./source/touch.d ./source/touch.o ./source/tpm.d ./source/tpm.o

.PHONY: clean-source

