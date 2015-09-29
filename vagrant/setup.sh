#!/bin/bash
apt-get update
apt-get upgrade -y

#INSTALL BASE
chmod +x "/vagrant/scripts/base.sh"
$include "/vagrant/scripts/base.sh"

#COMMENT OUT OR IN LINES AS APPROPRIATE
INCLUDED_TECH=()
INCLUDED_TECH+=("node")
#INCLUDED_TECH+=("mongodb")
#INCLUDED_TECH+=("meteor")
#INCLUDED_TECH+=("redis")
#INCLUDED_TECH+=("apache")
#INCLUDED_TECH+=("nginx")
#INCLUDED_TECH+=("mysql")
#INCLUDED_TECH+=("php")

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