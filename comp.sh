gcc -o servidor.out servidor-tcp.c
./servidor.out &
gcc -o cliente.out cliente-tcp.c
# ./cliente.out TAM R IP|PORTA|NOME|
./cliente.out
kill -s INT $(pgrep servidor.out)