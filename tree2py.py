

class Tree:
    def __init__(self, tag, data, parent=None):
        self.children = []
        self.tag = tag
        self.data = data
        self.parent = parent

    def add(self, tag, data):
        child = Tree(tag, data, self)
        self.children.append(child)
        return child

    def set(self, tag, data):
        child = None
        for c in self.children:
            if (c.data == data) and (c.tag == tag):
                return c
        return self.add(tag, data)

    def __str__(self):
        if not self.children:
           return str("<" + self.tag + " data=\"" + self.data + "\"/>")
        return '<{tag} data="{data}">{children}</{tag}>'.format(tag=self.tag,data=self.data, children='\n '.join(map(str, self.children)))

    def process(self, f, n):
        print n + self.tag + " " + self.data + ""
        l = n + "  ";
        for c in self.children:
            c.process(f, l)
def w(f, txt):
    f.write(txt)

def expr_(f, e):
    for a in e.children:
        if a.tag == "string":
            w(f, a.data)
        elif a.tag == "int":
            w(f, a.data)
        elif a.tag == "op":
            o = a.data
            if o == "=":
                o = "=="
            w(f, o)
        elif a.tag == "paran":
            w(f, "(")
            for b in a.children:
                if (b.tag == "expr"):
                    expr_(f, b)
            w(f, ")")


def statements_(f, b):
    for d in b.children:
        if (d.data[0:4] == "#py "):
            w(f, d.data[4:])
        elif (d.tag == "do"):
            for c in d.children:
                if (c.tag == "call"):
                    cla = ""
                    for q in c.children:
                        if q.tag == "class":
                            cla = q.data
                    w(f, "\t" + cla + "__" + c.data + "(")
                    for q in c.children:
                        if q.tag == "args":
                            com = ""
                            for e in q.children:
                                w(f, com)
                                expr_(f, e)
                                com = ", "
                    w(f, ")\n")
        elif (d.tag == "return"):
                    w(f, "\t" + "return ")
                    for e in d.children:
                        expr_(f, e)
                    w(f, "\n")

def process(ast, out):
    f = open(out, "a+")
    f.write("#! /usr/bin/python\n")
    f.write("import sys\n")
    ast.process(f, "")
    for c in ast.children:
        if (c.tag == "class"):
            for m in c.children:
                if (m.tag == "function"):
                    w(f, "def " + c.data + "__" + m.data + "(")
                    for b in m.children:
                        if b.tag == "params":
                            co = ""
                            for a in b.children:
                                w(f, co + a.data)
                                co = ", "
                    w(f, "):\n")
                    for b in m.children:
                        if b.tag == "statements":
                            statements_(f,b)
                    w(f, "\n")
    w(f, "Main__main()\n")
    f.close

