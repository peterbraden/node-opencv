#!/bin/bash
apt-get update
apt-get upgrade -y

#INSTALL BASE
chmod +x "/vagrant/scripts/base.sh"
$include "/vagrant/scripts/base.sh"

#COMMENT OUT OR IN LINES AS APPROPRIATE
INCLUDED_TECH=()
INCLUDED_TECH+=("node")

SCRIPT_LOCATION="/vagrant/scripts/setup-"

for i in ${INCLUDED_TECH[@]}; do
	chmod +x "${SCRIPT_LOCATION}${i}.sh"
	$include "${SCRIPT_LOCATION}${i}.sh"
done

echo "Base Setup Done!"
ls
#INSTALL USER
chmod +x "/vagrant/user.sh"
$include "/vagrant/user.sh"