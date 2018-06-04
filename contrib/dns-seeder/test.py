import socket

seeders = [
    'bwkseed.mempool.pw',
    'bwk.goseed.club'
]

for seeder in seeders:
    try:
        ais = socket.getaddrinfo(seeder, 0)
    except socket.gaierror:
        ais = []
    
    print(seeder + ' = ' + str(len(ais)))