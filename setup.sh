#!/bin/sh
mkdir /home/$USER/Security && cd /home/$USER/Security

git clone https://github.com/wladislawmakowski/classical-cryptography.git
git clone https://github.com/wladislawmakowski/passt.git

apt install xclip keepassxc
cd passt && chmod +x main passt
"export PATH="/home/$USER/Security/passt:$PATH"" >> ~/.bashrc
"alias sudo='sudo -E env "PATH=$PATH"' << ~/.bashrc

source ~/.bashrc
echo -e "\n[ PASST INSTALLED ]\n"
rm -r ~/passt
