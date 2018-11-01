#!/bin/bash
. utils.sh

printWelcome

printf "Type 'help' to get a list of commands.\n"

exit_msg()
{
    printf '\nending annihator..\n'
    exit
}

prompt()
{
    # https://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux
    RED='\033[0;31m'
    NC='\033[0m' # No Color
    printf "${RED}annihilator${NC} »$1"
}

trap exit_msg INT

while true ;
do
    prompt
    read cmd

    if [ $cmd = 'help' ]
    then
        printf 'Available list of commands are -\n'
    else
        printf 'Unknown command try again..\n'
    fi

done
