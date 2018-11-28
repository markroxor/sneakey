#!/bin/sh

heading=("
   _____                .__    .__.__          __                
  /  _  \   ____   ____ |  |__ |__|  | _____ _/  |_  ___________ 
 /  /_\  \ /    \ /    \|  |  \|  |  | \__  \\   __\/  _ \_  __  \\ 
/    |    \   |  \   |  \   Y  \  |  |__/ __ \|  | (  <_> )  | \/ 
\____|__  /___|  /___|  /___|  /__|____(____  /__|  \____/|__|   
        \/     \/     \/     \/             \/                   

" " 
 _______  __    _  __    _  __   __  ___  ___      _______  _______  _______  ______   
|   _   ||  |  | ||  |  | ||  | |  ||   ||   |    |   _   ||       ||       ||    _ |  
|  |_|  ||   |_| ||   |_| ||  |_|  ||   ||   |    |  |_|  ||_     _||   _   ||   | ||  
|       ||       ||       ||       ||   ||   |    |       |  |   |  |  | |  ||   |_||_ 
|       ||  _    ||  _    ||       ||   ||   |___ |       |  |   |  |  |_|  ||    __  |
|   _   || | |   || | |   ||   _   ||   ||       ||   _   |  |   |  |       ||   |  | |
|__| |__||_|  |__||_|  |__||__| |__||___||_______||__| |__|  |___|  |_______||___|  |_|

" "
 ▄▄▄       ███▄    █  ███▄    █  ██░ ██  ██▓ ██▓    ▄▄▄     ▄▄▄█████▓ ▒█████   ██▀███  
▒████▄     ██ ▀█   █  ██ ▀█   █ ▓██░ ██▒▓██▒▓██▒   ▒████▄   ▓  ██▒ ▓▒▒██▒  ██▒▓██ ▒ ██▒
▒██  ▀█▄  ▓██  ▀█ ██▒▓██  ▀█ ██▒▒██▀▀██░▒██▒▒██░   ▒██  ▀█▄ ▒ ▓██░ ▒░▒██░  ██▒▓██ ░▄█ ▒
░██▄▄▄▄██ ▓██▒  ▐▌██▒▓██▒  ▐▌██▒░▓█ ░██ ░██░▒██░   ░██▄▄▄▄██░ ▓██▓ ░ ▒██   ██░▒██▀▀█▄  
 ▓█   ▓██▒▒██░   ▓██░▒██░   ▓██░░▓█▒░██▓░██░░██████▒▓█   ▓██▒ ▒██▒ ░ ░ ████▓▒░░██▓ ▒██▒
 ▒▒   ▓▒█░░ ▒░   ▒ ▒ ░ ▒░   ▒ ▒  ▒ ░░▒░▒░▓  ░ ▒░▓  ░▒▒   ▓▒█░ ▒ ░░   ░ ▒░▒░▒░ ░ ▒▓ ░▒▓░
  ▒   ▒▒ ░░ ░░   ░ ▒░░ ░░   ░ ▒░ ▒ ░▒░ ░ ▒ ░░ ░ ▒  ░ ▒   ▒▒ ░   ░      ░ ▒ ▒░   ░▒ ░ ▒░
  ░   ▒      ░   ░ ░    ░   ░ ░  ░  ░░ ░ ▒ ░  ░ ░    ░   ▒    ░      ░ ░ ░ ▒    ░░   ░ 
      ░  ░         ░          ░  ░  ░  ░ ░      ░  ░     ░  ░            ░ ░     ░                                                                                            
" "
 +-++-++-++-++-++-++-++-++-++-+
 |A||n||n||h||i||l||a||t||o||r|
 +-++-++-++-++-++-++-++-++-++-+
" "
     \                   |     _)  |         |                
    _ \    __ \   __ \   __ \   |  |   _\` \|  __\|   _ \    __\| 
   ___ \   \|   \|  \|   \|  \| \| \|  \|  \|  (   \|  \|    (   \|  \|    
 _/    _\ _\|  _\| _\|  _\| _\| \|_\| _\| _\| \__,_\| \__\| \___/  _\|    
                                                              
" "
 ____  ____  ____  ____  ____  ____  ____  ____  ____  ____ 
||A ||||n ||||n ||||h ||||i ||||l ||||a ||||t ||||o ||||r ||
||__||||__||||__||||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\||/__\||/__\||/__\|
")



function printWelcome()
{
    len=$((${#heading[@]}-1))
    random=$(shuf -i 0-$len -n 1)

    printf "%s" "${heading[$random]}"
}

# https://unix.stackexchange.com/a/429094
function printTable()
{
    local -r delimiter="${1}"
    local -r data="$(removeEmptyLines "${2}")"

    if [[ "${delimiter}" != '' && "$(isEmptyString "${data}")" = 'false' ]]
    then
        local -r numberOfLines="$(wc -l <<< "${data}")"

        if [[ "${numberOfLines}" -gt '0' ]]
        then
            local table=''
            local i=1

            for ((i = 1; i <= "${numberOfLines}"; i = i + 1))
            do
                local line=''
                line="$(sed "${i}q;d" <<< "${data}")"

                local numberOfColumns='0'
                numberOfColumns="$(awk -F "${delimiter}" '{print NF}' <<< "${line}")"

                # Add Line Delimiter

                if [[ "${i}" -eq '1' ]]
                then
                    table="${table}$(printf '%s#+' "$(repeatString '#+' "${numberOfColumns}")")"
                fi

                # Add Header Or Body

                table="${table}\n"

                local j=1

                for ((j = 1; j <= "${numberOfColumns}"; j = j + 1))
                do
                    table="${table}$(printf '#| %s' "$(cut -d "${delimiter}" -f "${j}" <<< "${line}")")"
                done

                table="${table}#|\n"

                # Add Line Delimiter

                if [[ "${i}" -eq '1' ]] || [[ "${numberOfLines}" -gt '1' && "${i}" -eq "${numberOfLines}" ]]
                then
                    table="${table}$(printf '%s#+' "$(repeatString '#+' "${numberOfColumns}")")"
                fi
            done
            if [[ "$(isEmptyString "${table}")" = 'false' ]]
            then
                echo -e "${table}" | column -s '#' -t | awk '/^\+/{gsub(" ", "-", $0)}1'
            fi
        fi
    fi
}

function removeEmptyLines()
{
    local -r content="${1}"

    echo -e "${content}" | sed '/^\s*$/d'
}

function repeatString()
{
    local -r string="${1}"
    local -r numberToRepeat="${2}"

    if [[ "${string}" != '' && "${numberToRepeat}" =~ ^[1-9][0-9]*$ ]]
    then
        local -r result="$(printf "%${numberToRepeat}s")"
        echo -e "${result// /${string}}"
    fi
}

function isEmptyString()
{
    local -r string="${1}"

    if [[ "$(trimString "${string}")" = '' ]]
    then
        echo 'true' && return 0
    fi

    echo 'false' && return 1
}

function trimString()
{
    local -r string="${1}"

    sed 's,^[[:blank:]]*,,' <<< "${string}" | sed 's,[[:blank:]]*$,,'
}

function _set()
{
    dic=$1
    var=$3
    val=$4
    
    if [ -z $var ] || [ -z $val ]; then
        echo 'Invalid usage of set. See help'
    else
        dic[$var]=$val
    fi    
}

function _get()
{
    # echo 'in get'
    dic=$1
    # echo $1
    printf "Current variable configuration in get.\n"
    printTable ',' "$(for i in "${!dic[@]}"; do printf "%s,%s\n" $i ${dic[$i]}; done)"
}