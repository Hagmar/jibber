from collections import defaultdict
import argparse
import sys


class JibiInterpreter:
    class JibiFun:
        def __init__(self, name, start, end):
            self.name = name
            self.start = start
            self.end = end

        def run(self):
            pass

    def __init__(self, args):
        self.debug = args.debug
        self.program = args.code
        self.stack = []
        self.mem = [0]
        self.pp = 0
        self.mp = 0
        self.functions = {}
        self.ops = defaultdict(self.no_op)
        self.ops.update({
                'a': self.a,
                'b': self.b,
                'c': self.c,
                'd': self.d,
                'e': self.e,
                'f': self.f,
                'i': self.i,
                'j': self.j,
                'l': self.l,
                'm': self.m,
                'n': self.n,
                'o': self.o,
                'p': self.p,
                'r': self.r,
                's': self.s,
                't': self.t,
                'w': self.w,
                'x': self.x,
                'y': self.y,
                'z': self.z
        })
        self.context = []

    def dump_status(self):
        print('Stack: {}'.format(self.stack))
        print('Memory: {}, mp: {}'.format(self.mem, self.mp))

    def new_context(self):
        self.context.append((self.stack, self.mem, self.pp, self.mp))
        self.stack, self.mem, self.pp, self.mp = ([], [0], 0, 0)

    def pop_context(self):
        self.stack, self.mem, self.pp, self.mp = self.context.pop()

    def parse(self):
        i = 0
        ignore = False
        fun_names = []
        while i < len(self.program):
            if not ignore:
                c = self.program[i]
                if c == 'f':
                    if self.debug:
                        print('Found function "{}"'.format(self.program[i+1]))
                    fun_names.append((self.program[i+1], i))
                    ignore = True
                elif c == '.':
                    if fun_names:
                        fun, fun_i = fun_names.pop()
                        if self.debug:
                            print('Function {} is {} ({}-{})'.format(fun, self.program[fun_i:i+1], fun_i, i+1))
                        self.functions[fun] = self.JibiFun(fun, fun_i+2, i)
            else:
                ignore = False
            i += 1

    def run(self):
        while self.program[self.pp] != '.':
            op = self.program[self.pp]
            self.pp += 1
            if self.debug:
                self.dump_status()
                print('\nExecuting: {}\n'.format(op))
            self.ops[op]()
        if self.debug:
            self.dump_status()

    def no_op(self):
        pass

    def a(self):
        n = self.stack.pop()
        self.stack[-1] += n

    def b(self):
        pass

    def c(self):
        new_pp = self.functions[self.program[self.pp]].start
        self.new_context()
        self.pp = new_pp
        self.run()
        self.pop_context()

    def d(self):
        self.stack[-1] -= 1

    def e(self):
        self.stack.append(self.stack[-1])

    def f(self):
        self.pp = self.functions[self.program[self.pp]].end + 1
    
    def i(self):
        self.stack[-1] += 1

    def j(self):
        pass
    
    def l(self):
        pass
    
    def m(self):
        n = self.stack.pop()
        self.stack[-1] *= n

    def n(self):
        self.mp += 1
        if self.mp == len(self.mem):
            self.mem.append(0)

    def o(self):
        sys.stdout.write(chr(self.stack.pop()))

    def p(self):
        self.mp -= 1

    def r(self):
        c = '\n'
        while c == '\n':
            c = sys.stdin.read(1)
        self.stack.append(c)

    def s(self):
        n = self.stack.pop()
        self.stack[-1] -= n

    def t(self):
        self.stack.append(self.stack[-1])

    def w(self):
        self.mem[self.mp] = self.stack.pop()

    def x(self):
        self.stack.append(self.mem[self.mp])

    def y(self):
        self.stack.pop()

    def z(self):
        self.stack.append(0)


def main():
    args = parse_args()

    interpreter = JibiInterpreter(args)
    interpreter.parse()
    interpreter.run()


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('code')
    parser.add_argument('-d', '--debug', action='store_true')

    return parser.parse_args()


if __name__ == '__main__':
    main()
