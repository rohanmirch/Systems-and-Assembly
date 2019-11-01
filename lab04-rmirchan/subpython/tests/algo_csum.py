mem()
list = [76, 34, 63, 21, 74, 37, 51, 25, 71, 19, 7, 4, 98, 1, 11, 54, 62, 87, 99]
print(list)
idx = 1
while idx < len(list):
    list[idx] = list[idx] + list[idx - 1]
    idx = idx + 1
print(list)
del list
del idx
gc()
mem()
