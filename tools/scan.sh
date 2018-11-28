_scan()
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
            nmap_dump=`sudo nmap -sn ${net_conf[$cmd]}/24`

            sc=`echo "$nmap_dump" | grep 'Nmap scan report'`
            ac=`echo "$nmap_dump" | grep 'MAC'`

            table="IP,Hostname\n$(paste -d ',' <(echo "$sc" | cut -d ' ' -f 5) <(echo "$ac" | cut -d ' ' -f4-))"
            printTable ',' "$table"

            break 
        fi
    done
    current_subshell=''
}