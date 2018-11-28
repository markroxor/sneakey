#--------------------------------------------------------------------------------------------------#
declare -A wfuzz_vars
wfuzz_vars[wordlist]='/usr/share/wfuzz/wordlist/general/big.txt'
wfuzz_vars[hc]='403,404'
#--------------------------------------------------------------------------------------------------#

_wfuzz()
{
    current_subshell='(wfuzz)'

    # table="Interface, IP\n$(for i in "${!wfuzz_vars[@]}"; do printf "%s,%s\n" $i ${wfuzz_vars[$i]}; done)"
    # printTable ',' "$table"
    # return
    _get wfuzz_vars

    while true; do
        prompt $current_subshell
        read cmd

        if [ -z "$cmd" ]; then
            true
        elif [ "$cmd" = 'exit' ]; then
            break
        elif [ "$cmd" = 'var_config' ]; then
            _get wfuzz_vars
        elif [ $(echo $cmd | cut -d ' ' -f 1) = 'set' ]; then
            _set wfuzz_vars $cmd
        fi
    done
    current_subshell=''
    # wfuzz -w /usr/share/wfuzz/wordlist/general/big.txt -w /usr/share/wfuzz/wordlist/general/extensions_common.txt --hc '403,404' http://192.168.15.168/FUZZFUZ2Z
}