#!/bin/bash
set -e

. utils.sh
. tools/scan.sh
. tools/find_open_ports.sh

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
    GR='\033[0;32m'
    NC='\033[0m'
    printf "${RED}annihilator${GR}$1${NC}»$2"
}

declare -A net_conf
network_config()
{
    printf 'Current network configuration is -\n'
    
    i=1
    while true; do
        interface=$(echo $interfaces | cut -d ' ' -f $i)
        ip=$(echo $ips | cut -d $' ' -f $i)

        if [ -z $interface ];
        then
            break
        else
            net_conf[$interface]=$ip

        fi
            ((i++))
        done
        table="Interface, IP\n$(for i in "${!net_conf[@]}"; do printf "%s,%s\n" $i ${net_conf[$i]}; done)"
        printTable ',' "$table"
}

declare -A help
help['scan']="Scans the victim machine in a given interface."
help['list_ports']='Lists open ports of victim machine. (Victim ip - vip must be set (see set) for this to work.)'
help['set']='Sets the environment variables. Eg. usage - set vip 192.168.1.2'
help['var_config']='Displays the current variables set for annihilation.'
help['net_conf']='Displays systems ip on different interfaces.'
help['help']='Displays this help message.'
help['exit']='Exits Annihilator.'

trap exit_msg INT

# for cmd in $(ls tools/);
# do
#     printf '%s\n' $cmd
#     # foo
# done

#--------------------------------------------------------------------------------------------------#
declare -A vars
vars[vip]=''
vars[port]='80'
#--------------------------------------------------------------------------------------------------#

interfaces=$(ifconfig | awk '$1=="inet"  {print f} {f=$1}' | sed 's/://g')
ips=$(ifconfig | awk '/inet /{print substr($2,1)}')

network_config
while true ; do
    prompt $current_subshell
    read cmd

    if [ -z "$cmd" ]; then
        # pass
        true

    elif [ "$cmd" = 'var_config' ]; then
        printf "Current variable configuratin.\n"
        printTable ',' "$(for i in "${!vars[@]}"; do printf "%s,%s\n" $i ${vars[$i]}; done)"


    elif [ "$cmd" = 'net_conf' ]; then
        network_config

    elif [ "$cmd" = 'scan' ]; then
        current_subshell='(scan)'
        printf "Choose interface to scan -\n"

        table="Interface, IP\n$(for i in "${!net_conf[@]}"; do printf "%s,%s\n" $i ${net_conf[$i]}; done)"
        printTable ',' "$table"

        while true; do
            prompt $current_subshell

            read cmd

            if [ -z "$cmd" ]; then
                true
            elif [ "$cmd" = 'exit' ]; then
                break
            elif [ -z ${net_conf[$cmd]} ]; then
                printf "%s is not a valid interface. \n" $cmd
            else
                nmap -snP ${net_conf[$cmd]}/24 | grep 'Nmap scan report'
                break 
            fi
        done
        current_subshell=''

    elif [ "$cmd" = 'list_ports' ]; then
        if [ -z ${vars[vip]} ]; then
            printf 'Victim IP (vip) not set.\n'
        else
            nmap -p- ${vars[vip]}
        fi

    elif [ $(echo $cmd | cut -d ' ' -f 1) = 'set' ]; then
        var=$(echo $cmd | cut -d ' ' -f 2)
        val=$(echo $cmd | cut -d ' ' -f 3)

        if [ -z $var ] || [ -z $val ]; then
            echo 'Invalid usage of set. See help'
        else
            vars[$var]=$val
        fi

    elif [ "$cmd" = 'help' ]; then
        printf "These are the available commands.\n"
        
        table="Command, Description\n""$(for i in "${!help[@]}";do  echo "$i,${help[$i]}";done)"
        printTable "," "$table"
        
    elif [ "$cmd" = 'exit' ]; then
        exit_msg

    else
        printf 'Unrecognised command. Try again..\n'
    fi

done

exit_msg
