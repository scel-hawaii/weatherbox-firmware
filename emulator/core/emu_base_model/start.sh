BUILD=ga_production
cd ../../../
platformio run --environment $BUILD
cp .pioenvs/$BUILD/firmware.hex emulator/core/emu_base_model
cd emulator/core
python run_emu.py $BUILD
