rm -rf build
mkdir build
cd build
eosio-cpp -abigen ../aoswap.cpp ../token_functions.cpp -o aoswap.wasm -I ./
