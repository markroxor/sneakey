#!/bin/bash
set -e

. utils.sh

# import tools
for tool in $(ls tools/)
do
    eval '. tools/$tool'
done

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

#--------------------------------------------------------------------------------------------------#
declare -A global_vars
global_vars[vip]=''
global_vars[port]='80'
#--------------------------------------------------------------------------------------------------#

interfaces=$(ifconfig | awk '$1=="inet"  {print f} {f=$1}' | sed 's/://g')
ips=$(ifconfig | awk '/inet /{print substr($2,1)}')


printWelcome
printf "Type 'help' to get a list of commands.\n"
network_config


#------------------------------------------Project-------------------------------------------------#
printf "Type 'list' to get list of existing projects or type a new project name.\n"

mkdir -p projects/
while true; do
    current_subshell="(Enter project name.)"
    prompt "$current_subshell"
    
    read cmd
    if [ -z "$cmd" ]; then
        continue
    elif [ "$cmd" = 'list' ]; then
        printf "Existing projects are -\n"
        for project in $(ls projects/)
        do
            printf "%s\n" $project
        done
    else
        if [ -d "projects/$cmd" ]; then
            printf "Using existing project %s\n" $cmd
        else
            mkdir -p projects/$cmd
            printf "Created new project %s\n" projects/$cmd
        fi 

        break
    fi
done

current_subshell=''
#--------------------------------------------------------------------------------------------------#
while true ; do
    prompt $current_subshell
    read cmd

    if [ -z "$cmd" ]; then
        # continue, on receiving an empty command
        continue

    elif [ "$cmd" = 'var_config' ]; then
        printf "Current variable configuratin.\n"
        printTable ',' "$(for i in "${!global_vars[@]}"; do printf "%s,%s\n" $i ${global_vars[$i]}; done)"

    elif [ "$cmd" = 'net_conf' ]; then
        network_config

    elif [ "$cmd" = 'list_ports' ]; then
        if [ -z ${global_vars[vip]} ]; then
            printf 'Victim IP (vip) not set.\n'
        else
            nmap -p- ${global_vars[vip]}
        fi

    elif [ $(echo $cmd | cut -d ' ' -f 1) = 'set' ]; then
        # _set global_vars $cmd
        var=$(echo $cmd | cut -d ' ' -f 2)
        val=$(echo $cmd | cut -d ' ' -f 3)

        if [ -z $var ] || [ -z $val ]; then
            echo 'Invalid usage of set. See help'
        else
            global_vars[$var]=$val
        fi

    elif [ "$cmd" = 'help' ]; then
        printf "These are the available commands.\n"
        
        table="Command, Description\n""$(for i in "${!help[@]}";do  echo "$i,${help[$i]}";done)"
        printTable "," "$table"
        
    elif [ "$cmd" = 'exit' ]; then
        exit_msg
    
    # test if _$cmd is part of tools.
    elif ! type "_$cmd" >/dev/null 2>&1; then
        printf 'Unrecognised command. Type "help" for list of commands.\n'
    else
        _$cmd
    fi

done

exit_msg
