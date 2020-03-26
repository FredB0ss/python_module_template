
import pyrc4

it1 = pyrc4.Rc4(b"Fred")

for x in it1:
    print(it1.count, hex(x))

    it2 = pyrc4.Rc4(b"B0ss")

    for y in it2:
        print("\t", it2.count, hex(y))
        if 4 == it2.count:
            break

    if 10 == it1.count:
        break


it1 = pyrc4.Rc4(b"Fred")
for x in it1:
    print(it1.count, hex(x))
    if 10 == it1.count:
        break
