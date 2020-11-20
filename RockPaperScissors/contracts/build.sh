rm -rf rps
mkdir rps
cd rps
eosio-cpp -abigen ../rps.cpp -o rps.wasm -I ./
