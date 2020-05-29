#================
# COMPILER FLAGS
#================

CCFLAGS += -std=c++11 -Werror -Wall -O3 -mavx -pthread

#==========================
# INSTALLATION && CLEANING
#==========================

default : compile

install : 
	git clone https://github.com/rogersce/cnpy.git src/vendor/cnpy
	mkdir -p src/vendor/cnpy/build bin res
	cd src/vendor/cnpy/build && cmake ..
	cd src/vendor/cnpy/build && sudo make && sudo make install

clean : 
	- cd src/vendor/cnpy/build && make clean
	rm -rf src/vendor/cnpy bin res

#============
# CNPY STUFF 
#============

LINK_TO_CNPY_FLAGS = -L/usr/local -lcnpy -lz

#=============
# COMPILATION
#=============

MODEL_SRC = src/model.cpp
MODEL_EXE = bin/model

compile : ${MODEL_SRC}
	g++ ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}

#=======
# TESTS
#=======

RES_FOLDER = res

test_vacuum:
	g++ -D EXPERIMENT_VACUUM ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}
	${MODEL_EXE} ${RES_FOLDER}/vacuum.npy

test_amp_lattice:
	g++ -D EXPERIMENT_AMP_LATTICE ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}
	${MODEL_EXE} ${RES_FOLDER}/amp-lattice.npy

test_phase_lattice:
	g++ -D EXPERIMENT_PHASE_LATTICE ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}
	${MODEL_EXE} ${RES_FOLDER}/phase-lattice.npy

test_diffraction:
	g++ -D EXPERIMENT_DIFFRACTION ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}
	${MODEL_EXE} ${RES_FOLDER}/diffraction.npy

test_interference:
	g++ -D EXPERIMENT_INTEREFERENCE ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}
	${MODEL_EXE} ${RES_FOLDER}/interference.npy

test_gaussian_lens:
	g++ -D EXPERIMENT_GAUSSIAN_LENS ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}
	${MODEL_EXE} ${RES_FOLDER}/gaussian-lens.npy

test_canalisation:
	g++ -D EXPERIMENT_CANALISATION ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}
	${MODEL_EXE} ${RES_FOLDER}/canalisation.npy

test_turbulence:
	g++ -D EXPERIMENT_TURBULENCE ${CCFLAGS} ${MODEL_SRC} -o ${MODEL_EXE} ${LINK_TO_CNPY_FLAGS}
	${MODEL_EXE} ${RES_FOLDER}/turbulence.npy

