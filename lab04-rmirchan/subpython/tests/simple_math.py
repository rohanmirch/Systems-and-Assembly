mem()
print(1)
print(2)
print(1 + 2)
print(3 * 5)
print(1 + 2 + 3 + 4 + 5 + 6 + 7 + 8)
gc()
mem()
a = 4
b = 5
c = 6
d = b * c
e = a + b + c
f = 4444
g = f * e
print(a, b, c, d, e, f, g)
mem()
del a
del b
del c
del d
del e
del f
del g
gc()
mem()
