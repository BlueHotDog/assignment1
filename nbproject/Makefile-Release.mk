#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/ydThreads.o \
	${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/tests.o \
	${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/linkedList.o \
	${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/jobs.o \
	${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/main.o \
	${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/fileHandler.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/assignment1

dist/Release/GNU-Linux-x86/assignment1: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/assignment1 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/ydThreads.o: nbproject/Makefile-${CND_CONF}.mk ../Assignment1-backup/ydThreads.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/ydThreads.o ../Assignment1-backup/ydThreads.c

${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/tests.o: nbproject/Makefile-${CND_CONF}.mk ../Assignment1-backup/tests.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/tests.o ../Assignment1-backup/tests.c

${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/linkedList.o: nbproject/Makefile-${CND_CONF}.mk ../Assignment1-backup/linkedList.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/linkedList.o ../Assignment1-backup/linkedList.c

${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/jobs.o: nbproject/Makefile-${CND_CONF}.mk ../Assignment1-backup/jobs.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/jobs.o ../Assignment1-backup/jobs.c

${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/main.o: nbproject/Makefile-${CND_CONF}.mk ../Assignment1-backup/main.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/main.o ../Assignment1-backup/main.c

${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/fileHandler.o: nbproject/Makefile-${CND_CONF}.mk ../Assignment1-backup/fileHandler.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/Assignment1-backup/fileHandler.o ../Assignment1-backup/fileHandler.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/assignment1

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
