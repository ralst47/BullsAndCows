sequince = [0, 1, 2, 3, 4]


def swap(sequince, i, j):
    sequince[i], sequince[j] = sequince[j], sequince[i]


def reverse(sequince, index):
    shift = index + 1
    n = len(sequince)
    for i in range((n-shift)//2):
        sequince[shift+i], sequince[n-1-i] = sequince[n-1-i], sequince[shift+i]


def k_permutaion_of_n(k, sequince):
    n = len(sequince)
    for j in range(k, n):
        if sequince[j] > sequince[k-1]:
            break
    else:
        j = n
    if j < n:
        swap(sequince, k-1, j)
        return sequince[:k:]
    else:
        reverse(sequince, k-1)
        for i in range(k-2, -1, -1):
            if sequince[i] < sequince[i+1]:
                break
        else:
            return None
        for j in range(n-1, i, -1):
            if sequince[j] > sequince[i]:
                break
        swap(sequince, i, j)
        reverse(sequince, i)
        return sequince[:k:]


k = 3
perm = sequince[:k:]
while perm is not None:
    print(perm)
    perm = k_permutaion_of_n(k, sequince)
