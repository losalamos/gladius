# Copyright 2016 Stanford University
# Copyright 2016 Triad National Security, LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


ifndef LG_RT_DIR
$(error LG_RT_DIR variable is not defined, aborting build)
endif

ifndef MRNET_HOME
$(error MRNET_HOME variable is not defined, aborting build)
endif

ifndef GASNET_ROOT
$(error GASNET_ROOT variable is not defined, aborting build)
endif

# Flags for directing the runtime makefile what to include
DEBUG           ?= 1		# Include debugging symbols
OUTPUT_LEVEL    ?= LEVEL_DEBUG	# Compile time logging level
SHARED_LOWLEVEL ?= 0		# Use shared-memory runtime (not recommended)
USE_CUDA        ?= 0		# Include CUDA support (requires CUDA)
USE_GASNET      ?= 1		# Include GASNet support (requires GASNet)
CONDUIT         ?= udp
USE_HDF         ?= 0		# Include HDF5 support (requires HDF5)
ALT_MAPPERS     ?= 0		# Include alternative mappers (not recommended)

# Put the binary file name here
OUTFILE		?= mapper-mrnet-interop
# List all the application source files here
GEN_SRC		?= mapper-mrnet-interop.cc
GEN_GPU_SRC	?= # .cu files

# You can modify these variables, some will be appended to by the runtime makefile
xplat_version := 5.0.1
INC_FLAGS	?= \
-I$(MRNET_HOME)/include \
-I$(MRNET_HOME)/lib/xplat-$(xplat_version)/include

CC_FLAGS	?= -g -pthread -std=c++11
NVCC_FLAGS	?=
GASNET_FLAGS?=

LD_FLAGS	?= \
-L$(MRNET_HOME)/lib \
-L/usr/lib/x86_64-linux-gnu \
-lmrnet -lxplat -lboost_system -lboost_timer -ldl

###########################################################################
#
#   Don't change anything below here
#   
###########################################################################

include $(LG_RT_DIR)/runtime.mk
