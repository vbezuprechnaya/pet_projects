class HashTable(object):
    def __init__(self, size):  # -size
        self.size = size
        self.table = [None] * self.size
        self.elementCount = 0

    def hashing(self, key):
        index = key % self.size
        return index

    def add(self, key, val):

        if self.is_full():
            self.double()

        index = self.hashing(key)

        if self.table[index] is not None:
            while self.table[index] is not None:
                index += 1
            self.table[index] = [key, val]
        else:
            self.table[index] = [key, val]

    def delete(self, key, val):
        index = self.hashing(key)
        for j in range(index, len(self.table)):
            if self.table[index] is not None and (self.table[j][0] == key and self.table[j][1] == val):
                self.table[index] = None
                break
            else:
                while self.table[index] is not None and self.table[j][0] != key and self.table[j][1] != val:
                    index += 1
                self.table[index] = None

    def search(self, key, val):
        res = False
        index = self.hashing(key)
        if self.table[index] is not None:
            for j in range(index, len(self.table)):
                if self.table[j] is not None and (self.table[j][0] == key and self.table[j][1] == val):
                    res = True
                    break
                else:
                    while self.table[index] is not None and self.table[j][0] != key and self.table[j][1] != val:
                        index += 1
                    res = True
        return res

    def is_full(self):
        items = 0
        for item in self.table:
            if item is not None:
                items += 1
        return items > len(self.table)/2

    def double(self):
        ht2 = HashTable(len(self.table) * 2)
        for j in range(len(self.table)):
            if self.table[j] is None:
                continue
            else:
                ht2.add(self.table[j][0], self.table[j][1])

        self.table = ht2.table
        self.size = ht2.size


# t = HashTable(5)
# #
# keys = [1, 5, 2, 4, 3, 5]
# val = [-1, 9, 11, 1, 6, 2]
# # #
# for i in range(6):
#     t.add(keys[i], val[i])
# t.add(0, 12)
# t.add(2, 10)
# #
# print()
# for i in t.table:
#     print(i)
# print()
#
# #
# for i in range(3):
#     print(t.search(keys[i], val[i]))

# t.add(10, 17)
# t.add(20,8)
# t.add(7,41)
# for i in t.table:
#     print(i)
# print()
# #
# print(t.search(7,41))
# #
# t.delete(-1,8)
# for i in t.table:
#     print(i)
# print()
