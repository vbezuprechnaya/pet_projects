from AVL_tree import AVL_Tree
from Hash import HashTable
from random import randint, seed
import matplotlib.pyplot as plot
import timeit

size = 100
table = HashTable(size)
Tree = AVL_Tree()
root = None
add_nums = []
del_nums = []
time_insert = []
time_add = []
time_find = []
time_search = []
time_delete = []
time_delh = []
val = []
list_val = []
list_keys = []

# количество элементов
seed(0)
n = 50
d = 50

for i in range(n):
    num = randint(-d, d)
    add_nums.append(num)
    list_val.append(num)

start_insert = timeit.default_timer()
for i in add_nums:
    root = Tree.insert(root, i)
    time_insert.append(timeit.default_timer() - start_insert)

for i in range(n):
    key = randint(-d, d)
    list_keys.append(key)

start_add = timeit.default_timer()
for i in range(n):
    table.add(list_keys[i], list_val[i])
    time_add.append(timeit.default_timer() - start_add)

for i in range(n):
    v = randint(-d, d)
    val.append(v)
    list_val.append(v)

for i in range(n):
    key = randint(-d, d)
    list_keys.append(key)

start_find = timeit.default_timer()
for v in val:
    Tree.search(root, v)
    time_find.append(timeit.default_timer() - start_find)

start_search = timeit.default_timer()
for i in range(n):
    table.search(list_keys[i], list_val[i])
    time_search.append(timeit.default_timer() - start_search)

for i in range(n):
    num = randint(-d, d)
    del_nums.append(num)
    list_val.append(num)

for i in range(n):
    key = randint(-d, d)
    list_keys.append(key)

start_delete = timeit.default_timer()
for i in del_nums:
    root = Tree.delete(root, i)
    time_delete.append(timeit.default_timer() - start_delete)

start_delh = timeit.default_timer()
for i in range(n):
    table.delete(list_keys[i], list_val[i])
    time_delh.append(timeit.default_timer() - start_delh)


fig, ax = plot.subplots(dpi=200)
csfont = {'fontname': 'DejaVu Sans'}
plot.plot([i for i in range(n)], time_insert, color='#C79FEF')
plot.plot([i for i in range(n)], time_add, color='#bd1919')
ax.set_xlabel('Количество элементов', **csfont, color='#FFFFFF', fontsize=9)
ax.set_ylabel('Время добавления (мкс)', **csfont, color='#FFFFFF', fontsize=9)
plot.savefig('rand_add.png')


fig, ax = plot.subplots(dpi=200)
csfont = {'fontname': 'DejaVu Sans'}
plot.plot([i for i in range(n)], time_find, color='#C79FEF')
plot.plot([i for i in range(n)], time_search, color='#bd1919')
ax.set_xlabel('Количество элементов', **csfont, color='#000000', fontsize=9)
ax.set_ylabel('Время поиска (мкс)', **csfont, color='#000000', fontsize=9)
plot.savefig('rand_find.png')


fig, ax = plot.subplots(dpi=200)
csfont = {'fontname': 'DejaVu Sans'}
plot.plot([i for i in range(n)], time_delete, color='#C79FEF')
plot.plot([i for i in range(n)], time_delh, color='#bd1919')
ax.set_xlabel('Количество элементов', **csfont, color='#000000', fontsize=9)
ax.set_ylabel('Время удаления (мкс)', **csfont, color='#000000', fontsize=9)
plot.savefig('rand_delete.png')

# if __name__ == '__main__':
#     print_hi('PyCharm')

