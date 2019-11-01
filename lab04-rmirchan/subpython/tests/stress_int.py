a = 0
b = 1
i = 0
while i < 10000:
    b = a + b
    a = b - a
    i = i + 1
print(a, b, i)
