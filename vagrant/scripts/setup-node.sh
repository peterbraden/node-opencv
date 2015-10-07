apt-get install gcc make build-essential -y
apt-get install -y curl
apt-get install -y git
# curl --silent --location https://deb.nodesource.com/setup_4.x | sudo bash -
# apt-get install -y nodejs
su vagrant
wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.27.1/install.sh | bash
nvm install v0.10
nvm install v0.12
nvm install v4.1



