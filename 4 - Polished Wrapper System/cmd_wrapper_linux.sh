g++ -fPIC -shared panda_wrapper.cpp arg_buffer.cpp -o ./modules/libpanda_wrapper.so -I panda3d/built/include/ -L panda3d-pike/built/lib/ -lp3framework -lpanda -lpandaexpress -lp3dtoolconfig -lp3dtool
