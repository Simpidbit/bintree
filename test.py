from random import randint

oper_num = 10000

node_set = set()
msg = ''

class node_numbers_t:
    def __init__(self):
        self.node_set = set()

    def generate(self):
        g = randint(0, 1000)
        self.node_set.add(g)
        return g

    def remove(self, n):
        self.node_set.remove(n)

    def rand_choose(self):
        l = len(self.node_set)
        ti = randint(0, l - 1)
        i = 0
        for e in self.node_set:
            if i == ti:
                return e
            i += 1

    def empty(self) -> bool:
        return len(self.node_set) == 0

man = node_numbers_t()

for i in range(oper_num):
    # 操作
    oper = int()
    if man.empty():
        oper = 0
    else:
        oper = randint(0, 1)
    #oper = 0

    # 操作数
    num = int()
    if oper == 0:
        # 添加元素
        num = man.generate()
    else:
        # 删除元素
        num = man.rand_choose()
        while True:
            try:
                man.remove(num)
            except KeyError:
                break


    msg += f'{oper} {num}\n'

with open('input.txt', 'wt') as f:
    f.write(f'{oper_num}\n' + msg)
