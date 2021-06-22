#! /bin/bash

echo -e "\e[1;31mMaking the extension project...."
echo -e "\033[0m"

make
echo ""
echo ""
read -p "Enter name of source file to compile: " SOURCE

echo ""
echo -e "\033[0;33mConverting source code in to assembly"
echo -e "\033[0m"

./lexer "sample_src/$SOURCE"
make clean

echo ""
echo ""
echo -e "\033[0;32mCompleted conversion of source code into assembly.."

echo -e "\033[0m"
echo -e "\033[0;33mMoving assembly file to be converted into binary"

echo -e "\033[0m"
echo ""
mv out.s ../assembler
rm registerMap.txt
cd ../assembler
make

./assemble out.s out.out
make clean

rm out.s  #Removing the assembly file as it is no longer needed
echo ""
echo -e "\033[0m"
echo -e "\033[0;32mCompleted conversion into binary..."

echo -e "\033[0m"
echo -e "\033[1;33mRunning Binary file on the arm11 emulator!"

echo -e "\033[0m"
echo ""
mv out.out ../emulator
cd ../emulator
make

./emulate out.out
make clean

rm out.out #Removing Binary file as it is no longer needed


echo ""
echo ""
echo -e "\033[1;32mALL DONE!!"