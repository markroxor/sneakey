scan()
{
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
            # nmap_dump=`sudo nmap -sn ${net_conf[$cmd]}/24`
            # custom UDP scan, more likely to hit
	    nmap_dump=`sudo nmap -sP -PU161 ${net_conf[$cmd]}/24`
            sc=`echo "$nmap_dump" | grep 'Nmap scan report'`
            ac=`echo "$nmap_dump" | grep 'MAC'`
            nd=`sudo netdiscover -i $cmd -r ${net_conf[$cmd]}/24 -PN | head -n -2`

            table="IP,MAC,Hostname\n$(paste -d ',' <(echo "$sc" | cut -d ' ' -f 5) <(echo "$ac" | cut -d ' ' -f3) <(echo "$ac" | cut -d ' ' -f4-))"
            printTable ',' "$table"
            table="IP,MAC,Hostname\n$(paste -d ',' <(echo "$nd" | tr -s '[:blank:]' | cut -d ' ' -f 2) <(echo "$nd" | tr -s '[:blank:]' | cut -d ' ' -f3) <(echo "$nd" | tr -s '[:blank:]' | cut -d ' ' -f6-))"
            printTable ',' "$table"

            break 
        fi
    done
    current_subshell=''
}
