# MC833
Equipe:
- Enrico Piovesana Fernandes RA: 233895
- João Vitor Baptista Moreira RA: 237833
## Projeto 1 - Socket TCP
O código foi desenvolvido em 2 arquivos, o [server.c](/server.c) e o [client.c](/client.c). Além disso, o arquivo [test.c](/test.c) permite testar 
localmente as funções implementadas no servidor sem que haja a comunicação entre as partes, facilitando o debug. Finalmente, o banco de dados com
o registro de usuários pode ser acessado pela pasta [data](/data).
Para compilar, as intruções são básicas, são elas:
- gcc server.c -o server 
- gcc client.c -o client
- gcc test.c -o test
