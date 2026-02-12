# *********************************************************
#
#                  gPLUTO makefile
#
# *********************************************************

pluto:                              # Default target

ARCH         = linux.g++.defs
#PLUTO_DIR    =                      # define PLUTO main directory here
SRC          = $(PLUTO_DIR)/Src
INCLUDE_DIRS = -I. -I$(SRC)
VPATH        = ./:$(SRC)/New:$(SRC):$(SRC)/Time_Stepping:$(SRC)/States

include $(PLUTO_DIR)/Config/$(ARCH)

# ---------------------------------------------------------
#         Set headers and object files
# ---------------------------------------------------------

OBJ     = arrays.o array_reconstruct.o bin_io.o boundary.o \
          cmd_line_opt.o debug_tools.o \
          entropy_switch.o failsafe.o flag_shock.o \
          get_nghost.o globals.o init.o            \
          initialize.o input_data.o internal_boundary.o \
          jet_domain.o main.o mapped_grid_tools.o  mappers3D.o             \
          math_misc.o  math_lu_decomp.o mean_mole_weight.o mem_usage.o \
          next_time_step.o nvtx.o output_log.o    \
          parallel_exchange_async.o parallel_exchange_sync.o parse_file.o  \
          rbox.o ring_average.o restart.o rotate.o runtime_setup.o source.o      \
          set_geometry.o set_grid.o set_output.o set_var_names.o  \
          show_config.o split_source.o startup.o sweep_alloc.o    \
          timing.o tools.o write_data.o write_tab.o write_vtk.o   \
          vector_curl.o  userdef_output.o 

HEADERS = Array.hpp definitions.hpp eos.hpp gpluto.hpp \
          macros.hpp metric.hpp                        \
          plm_coeffs.hpp prototypes.hpp structs.hpp  

include $(SRC)/Math_Tools/makefile

# ---------------------------------------------------------
#  Define macros by adding -D<name> where <name> has been
#  set to TRUE in the system configuration file (.defs)
# ---------------------------------------------------------

ifeq ($(strip $(PARALLEL)), TRUE)
 CFLAGS += -I$(SRC)/Parallel -DPARALLEL
 include $(SRC)/Parallel/makefile
 ifeq ($(strip $(USE_ASYNC_IO)), TRUE)
  CFLAGS += -DUSE_ASYNC_IO
 endif
endif

ifeq ($(strip $(USE_HDF5)), TRUE)
 CFLAGS += -DUSE_HDF5
 OBJ    += hdf5_io.o
endif

ifeq ($($strip $(USE_PNG)), TRUE)
 CFLAGS += -DUSE_PNG
endif

-include local_make

# ---------------------------------------------------------
#   Additional_CFLAGS_here   ! dont change this line
# ---------------------------------------------------------

# ---------------------------------------------------------
#   Additional_header_files_here   ! dont change this line
# ---------------------------------------------------------

# ---------------------------------------------------------
#   Additional_object_files_here   ! dont change this line
# ---------------------------------------------------------

OBJ += flat_states.o
OBJ += rk_step.o
OBJ += update_stage.o
include $(SRC)/MHD/makefile

# ---------------------------------------------------------
#    PLUTO target rule
# ---------------------------------------------------------

pluto: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# ---------------------------------------------------------
#                    Suffix rule
# ---------------------------------------------------------

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $<

clean:
	@rm -f	*.o
	@echo make clean: done

# ---------------------------------------------------------
#          Dependencies for object files
# ---------------------------------------------------------

$(OBJ):  $(HEADERS)
