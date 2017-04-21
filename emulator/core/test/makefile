all:
	verilator -Wall --cc test.v --exe sim_main.cpp
	cd obj_dir && make -j -f Vtest.mk Vtest
run:
	cd obj_dir && ./Vtest

