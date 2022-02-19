# annihilator [WIP]
A commandline utility for pentesting.

[![asciicast](https://asciinema.org/a/240645.svg)](https://asciinema.org/a/240645)

cpp [codebase](https://github.com/markroxor/sneakey/tree/master/tools)

To use cpp tools install the libs using -
```
sudo apt install libnet-pcap-perl  # in ubuntu
```

Supply the appropriate flags using -  
`g++ $(libnet-config --defines)  unwrap_pack.cpp -lnet -lpcap && ./a.out`
