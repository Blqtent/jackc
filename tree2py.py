"""
 *                             jack to python 
 *
 *                      1 april MMXXI PUBLIC DOMAIN
 *           The author disclaims copyright to this source code.
 *
 *
"""

stati = 0
memory = {}

def w(f, txt):
    f.write(txt)

def get_var(f, a):
    o = a.parent;
    while (o != None and o.tag != "class"):
        o = o.parent
    if (o != None):
        nf = 0;
        for b in o.children:
            if (b.tag == "static" and b.data == a.data):
                return o.data + "__" + a.data
            elif (b.tag == "field"):
                if (b.data == a.data):
                    return "__memory[__this+" + str(nf) + "]" 
                nf += 1
    return a.data

def expr_(f, e):
    global stati
    for a in e.children:
        if a.tag == "string":
            stati = stati + 1
            memory[stati] = a.data
            w(f, str(stati))
        elif a.tag == "int":
            w(f, a.data)
        elif a.tag == "varname":
            if a.data == "this":
                w(f, "__this")
            elif a.data == "false":
                w(f, "0")
            elif a.data == "true":
                w(f, "(-1)")
            elif a.data == "null":
                w(f, "None")
            else:
                w(f, get_var(f, a))
        elif a.tag == "arrayvar":
            w(f, "__memory[" + get_var(f, a) + "+(")
            for b in a.children:
                if (b.tag == "expr"):
                    expr_(f, b)
            w(f, ")]")
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
        elif (a.tag == "call"):
            call_(f, a)
        elif a.tag == "unaryop":
            w(f, a.data)
            expr_(f, a)


def vars_(f, b):
    for d in b.children:
        if (d.tag == "Array"):
            w(f, "")

def get_class(q):
    o = q.parent;
    while (o != None and o.tag != "method" and o.tag != "function" and
            o.tag != "constructor"):
        o = o.parent;
    if (o != None):
        for e in o.children:
            if (e.tag == "vars"):
                for t in e.children:
                    if (t.data == q.data):
                       return t.tag
    o = q.parent;
    while (o != None and o.tag != "class"):
        o = o.parent;
    if (o != None):
        for e in o.children:
            if (e.tag == "method" or e.tag == "field"):
                if (e.data == q.parent.data):
                    return o.data
    return q.data

def call_(f, c):
    cla = ""
    for q in c.children:
        if q.tag == "classo":
            cla = get_class(q) 
            w(f, "\t" + cla + "__" + c.data + "(")
            if (cla != "" and q.data == ""):
                w(f, "__this")
            for r in c.children:
                if r.tag == "args":
                    if (q.data == cla):
                        com = ""
                    else:
                        w(f, q.data)
                        com = ", "

                    for e in r.children:
                        w(f, com)
                        expr_(f, e)
                        com = ", "
            w(f, ")\n")

def statements_(f, b):
    for d in b.children:
        if (d.data[0:4] == "#py "):
            w(f, d.data[4:])
        elif (d.tag == "do"):
            for c in d.children:
                if (c.tag == "call"):
                    call_(f, c)
        elif (d.tag == "let"):
            for c in d.children:
                is_array = 0
                for e in c.children:
                    if (e.tag == "assign"):
                        if (c.tag == "varname" and is_array == 0):
                            w(f, "\t" + get_var(f, c)) 
                        w(f, "=")
                        for k in e.children:
                            if (k.tag == "expr"):
                                expr_(f, k)
                    elif (e.tag == "array"):
                        is_array = 1
                        w(f, "\t__memory[" + get_var(f, c)+ " +(")
                        for k in e.children:
                            if (k.tag == "expr"):
                                expr_(f, k)
                        w(f, ")]")
            w(f, "\n")
        elif (d.tag == "return"):
            w(f, "\t" + "return ")
            for e in d.children:
                expr_(f, e)
            w(f, "\n")

def escape(s):
    return s.replace("\\", "\\\\")

def process(ast, out):
    f = open(out, "a+")
    f.write("#! /usr/bin/python3\n")
    f.write("import sys\n")
    f.write("__memory = {}\n")
    ast.process(f, "")
    for c in ast.children:
        if (c.tag == "class"):
            nb_field = 0
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
                        elif b.tag == "vars":
                            vars_(f,b)
                    w(f, "\n")
                elif (m.tag == "field"):
                    nb_field += 1
                elif (m.tag == "static"):
                    w(f, c.data + "__" + m.data + "=0\n")
                elif (m.tag == "method"):
                    w(f, "def " + c.data + "__" + m.data + "(")
                    for b in m.children:
                        if b.tag == "params":
                            w(f, "__this")
                            for a in b.children:
                                w(f, ", " + a.data)
                    w(f, "):\n")
                    for b in m.children:
                        if b.tag == "statements":
                            statements_(f,b)
                        elif b.tag == "vars":
                            vars_(f,b)
                    w(f, "\n")
                elif (m.tag == "constructor"):
                    w(f, "def " + c.data + "__" + m.data + "(")
                    for b in m.children:
                        if b.tag == "params":
                            co = ""
                            for a in b.children:
                                w(f, co + a.data)
                                co = ", "
                    w(f, "):\n")
                    si = 0;
                    w(f, "\t__this = Memory__alloc(" + str(nb_field) +  ")\n")
                    for b in m.children:
                        if b.tag == "statements":
                            statements_(f,b)
                        elif b.tag == "vars":
                            vars_(f,b)
                    w(f, "\n")
    i = 0
    while (i < stati):
        i += 1;
        w(f, "__memory[" + str(i) + "] = " + escape(memory[i]) + "\n")
    f.write("__memory[0] = " + str(stati + 1) +  "\n")
    w(f, "Sys__init()\n")
    f.close

