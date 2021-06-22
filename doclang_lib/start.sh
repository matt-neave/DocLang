#! /bin/bash

echo -e "\033[1;32mMaking the extension project...."
echo -e "\033[0m"

make clean
make

echo -e "\033[1;32mMoving lexer to testserver/"
mv lexer dlc
mv dlc testserver/
echo -e ""

echo -e "\033[1;32mCleaning"
echo -e "\033[0m"
make clean

echo ""
echo ""
echo -e "\033[1;32mMaking the assembler"
echo -e "\033[0m"
cd ../assembler
make clean
make

echo -e "\033[1;32mMoving assemble to testserver/"
mv assemble ../extension/testserver
echo -e ""

echo -e "\033[1;32mCleaning"
echo -e "\033[0m"
make clean
echo ""
echo ""
echo -e "\033[1;32mMaking the emulator"
echo -e "\033[0m"
cd ../emulator
make clean
make

echo -e "\033[1;32mMoving emulate to testserver/"
mv emulate ../extension/testserver
echo -e "\033[0m"

echo -e "\033[1;32mCleaning"
echo -e "\033[0m"
make clean
echo -e ""
echo -e "\033[1;33mPreparing to start server"
cd ../extension/testserver

echo -e "\033[1;33mStarting Server"
echo -e "\033[0m"
read -p "Enter port number [18000]: " PORT
PORT=${PORT:-18000}
ruby testserver.rb ./ $PORT