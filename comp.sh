gcc -o servidor.out servidor-tcp.c
./servidor.out &
gcc -o tc.out tc.c
# ./tc.out 0.0.0.0 TAM R IP|PORTA|NOME|
./tc.out 0.0.0.0 22R0.0.0.0|8081|cliente1