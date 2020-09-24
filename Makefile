#使用pwd命名确定当前路径
CUR_DIR = ${PWD}

#确定主目录下的其他目录的路径
MAIN_DIR = ${CUR_DIR}/main
LCD_DIR = ${CUR_DIR}/lcd
USR_DIR = ${CUR_DIR}/usr
DELAY_DIR = ${CUR_DIR}/delay
COMMON_DIR = ${CUR_DIR}/common
NETWORK_DIR = ${CUR_DIR}/network
FILE_DIR = ${CUR_DIR}/file

#确定头文件所在的文件夹的路径
INC_DIR = -I${MAIN_DIR} \
          -I${LCD_DIR} \
          -I${USR_DIR} \
		  -I${DELAY_DIR} \
		  -I${COMMON_DIR} \
		  -I${NETWORK_DIR} \
		  -I${FILE_DIR}

#查找所有的源文件夹
SRC = ${wildcard ${MAIN_DIR}/*.c} \
      ${wildcard ${LCD_DIR}/*.c} \
      ${wildcard ${USR_DIR}/*.c} \
      ${wildcard ${DELAY_DIR}/*.c} \
      ${wildcard ${COMMON_DIR}/*.c} \
      ${wildcard ${NETWORK_DIR}/*.c} \
      ${wildcard ${FILE_DIR}/*.c}

#确定所有的源文件对应的.o文件 patsubst函数为替换函数
OBJ = ${patsubst %.c,%.o,${SRC}}

#设置编译参数以及目标文件名称 使用gcc编译 使用-g参数便于使用gdb调试
CC = gcc
CCFLAGS = -g -Wall ${INC_DIR}
TARGET = raspi_iot

#将所有的.o文件链接成目标文件
${TARGET}:${OBJ}
	${CC} ${OBJ} -o $@ -lwiringPi -lpthread -lm
	@echo "Compile done."

#${OBJ}:${SRC}
#	${CC} ${CCFLAGS} -c $?

#将所有的源文件编译成.o文件 并打印编译过程
${OBJ}:%.o:%.c
	@echo "Compiling $< ==> $@"
	${CC} ${CCFLAGS} -c $< -o $@ -lwiringPi -lpthread -lm


.PHONY:clean
clean:
	@rm -f ${OBJ}
	@echo "Clean object files done."

	@rm -f *~
	@echo "Clean tempreator files done."

#	@rm -f ${TARGET}
#	@echo "Clean target files done."

	@echo "Clean done."
