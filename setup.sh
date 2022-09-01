#!/bin/sh
mkdir ~/Security && cd ~/Security

git clone https://github.com/wladislawmakowski/classical-cryptography.git
git clone https://github.com/wladislawmakowski/passt.git
cd passt

touch key.passt password.passt
chown root ~/Security

gcc main.c ../classical-cryptography/source/implementations/otp.c -o main

apt install xclip keepassxc
cd passt && chmod +x main passt
"export PATH="~/Security/passt:$PATH"" >> ~/.bashrc
"alias sudo='sudo -E env "PATH=$PATH"' >> ~/.bashrc

source ~/.bashrc
echo -e "\n[ PASST INSTALLED ]\n"
rm -r ~/passt
