

class Tree:
    def __init__(self, data, parent=None):
        self.children = []
        self.data = data
        self.parent = parent

    def add(self, data):
        child = Tree(data, self)
        self.children.append(child)
        return child

    def set(self, data):
        child = None
        for c in self.children:
            if c.data == data:
                return c
        return self.add(data)

    def __str__(self):
        if not self.children:
           return str(self.data)
        return '{data} [{children}]'.format(data=self.data, children=', '.join(map(str, self.children)))


    
global ast
ast = Tree("root")


