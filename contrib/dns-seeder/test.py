import socket

seeders = [
    'bwkseed.mempool.pw',
    'bwkseed1.bulwarkcrypto.com',
    'bwkseed2.bulwarkcrypto.com',
    'bwkseed3.bulwarkcrypto.com',
    'bwkseed4.bulwarkcrypto.com',
    'bwkseed5.bulwarkcrypto.com',
    'bwkseed1.bulwarkcrypto.site',
    'bwkseed2.bulwarkcrypto.site',
    'bwkseed3.bulwarkcrypto.site',
    'bwkseed4.bulwarkcrypto.site',
    'bwkseed5.bulwarkcrypto.site',
    'bwk.goseed.club'
]

for seeder in seeders:
    try:
        ais = socket.getaddrinfo(seeder, 0)
    except socket.gaierror:
        ais = []
    
    print(seeder + ' = ' + str(len(ais)))
