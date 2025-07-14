a = ord('a')
f = ord('f')
A = ord('A')
F = ord('F')
n0 = ord('0')
n9 = ord('9')

output = ""

for i in range(0, 16):
    for j in range(0, 16):
        v = i * 16 + j
        n = -1
        if v >= a and v <= f:
            n = v - a + 10
        elif v >= A and v <= F:
            n = v - A + 10
        elif v >= n0 and v <= n9:
            n = v - n0
        output += f"{n:>2}, "
    output += "\n"

print(output)

