import sys
from abc import ABC, abstractmethod
from Expression import *

class Visitor(ABC):
  """
  The visitor pattern consists of two abstract classes: the Expression and the
  Visitor. The Expression class defines on method: 'accept(visitor, args)'.
  This method takes in an implementation of a visitor, and the arguments that
  are passed from expression to expression. The Visitor class defines one
  specific method for each subclass of Expression. Each instance of such a
  subclasse will invoke the right visiting method.
  """
  @abstractmethod
  def visit_var(self, exp, arg): pass
  @abstractmethod
  def visit_bln(self, exp, arg): pass
  @abstractmethod
  def visit_num(self, exp, arg): pass
  @abstractmethod
  def visit_eql(self, exp, arg): pass
  @abstractmethod
  def visit_add(self, exp, arg): pass
  @abstractmethod
  def visit_sub(self, exp, arg): pass
  @abstractmethod
  def visit_mul(self, exp, arg): pass
  @abstractmethod
  def visit_div(self, exp, arg): pass
  @abstractmethod
  def visit_leq(self, exp, arg): pass
  @abstractmethod
  def visit_lth(self, exp, arg): pass
  @abstractmethod
  def visit_neg(self, exp, arg): pass
  @abstractmethod
  def visit_not(self, exp, arg): pass
  @abstractmethod
  def visit_let(self, exp, arg): pass
  @abstractmethod
  def visit_and(self, exp, arg): pass
  @abstractmethod
  def visit_or(self, exp, arg): pass
  @abstractmethod
  def visit_ifThenElse(self, exp, arg): pass

class EvalVisitor(Visitor):
  """
  The EvalVisitor class evaluates logical and arithmetic expressions. The
  result of evaluating an expression is the value of that expression. The
  inherited attribute propagated throughout visits is the environment that
  associates the names of variables with values.

  Examples:
  >>> e0 = Let('v', Add(Num(40), Num(2)), Mul(Var('v'), Var('v')))
  >>> e1 = Not(Eql(e0, Num(1764)))
  >>> ev = EvalVisitor()
  >>> e1.accept(ev, {})
  False

  >>> e0 = Let('v', Add(Num(40), Num(2)), Sub(Var('v'), Num(2)))
  >>> e1 = Lth(e0, Var('x'))
  >>> ev = EvalVisitor()
  >>> e1.accept(ev, {'x': 41})
  True
  """
  def visit_var(self, exp, env):
    if exp.identifier not in env:
      sys.exit("Def error")
    return env[exp.identifier]

  def visit_bln(self, exp, env):
    if isinstance(exp.bln, str):
      return exp.bln.lower() == 'true'
    return exp.bln

  def visit_num(self, exp, env):
    return int(exp.num)

  def visit_eql(self, exp, env):
    l = exp.left.accept(self, env)
    r = exp.right.accept(self, env)
    if type(l) == type(r) and type(l) in (int, bool):
      return l == r
    sys.exit("Type error")

  def visit_add(self, exp, env):
    l = exp.left.accept(self, env)
    r = exp.right.accept(self, env)
    if type(l) == int and type(r) == int:
      return l + r
    sys.exit("Type error")

  def visit_sub(self, exp, env):
    l = exp.left.accept(self, env)
    r = exp.right.accept(self, env)
    if type(l) == int and type(r) == int:
      return l - r
    sys.exit("Type error")

  def visit_mul(self, exp, env):
    l = exp.left.accept(self, env)
    r = exp.right.accept(self, env)
    if type(l) == int and type(r) == int:
      return l * r
    sys.exit("Type error")

  def visit_div(self, exp, env):
    l = exp.left.accept(self, env)
    r = exp.right.accept(self, env)
    
    if type(l) == int and type(r) == int:
      if r == 0:
        print("1D")
        sys.exit()
      return l // r
    sys.exit("Type error")

  def visit_leq(self, exp, env):
    l = exp.left.accept(self, env)
    r = exp.right.accept(self, env)
    if type(l) == int and type(r) == int:
      return l <= r
    sys.exit("Type error")

  def visit_lth(self, exp, env):
    l = exp.left.accept(self, env)
    r = exp.right.accept(self, env)
    if type(l) == int and type(r) == int:
      return l < r
    sys.exit("Type error")

  def visit_neg(self, exp, env):
    v = exp.exp.accept(self, env)
    if type(v) == int:
      return -v
    sys.exit("Type error")

  def visit_not(self, exp, env):
    v = exp.exp.accept(self, env)
    if type(v) == bool:
      return not v
    sys.exit("Type error")

  def visit_let(self, exp, env):
    val0 = exp.exp_def.accept(self, env)
    new_env = env.copy()
    new_env[exp.identifier] = val0
    return exp.exp_body.accept(self, new_env)

  def visit_and(self, exp, env):
    l = exp.left.accept(self, env)
    if not isinstance(l, bool): sys.exit("Type error")
    if not l:
      return False
    r = exp.right.accept(self, env)
    if not isinstance(r, bool): sys.exit("Type error")
    return r

  def visit_or(self, exp, env):
    l = exp.left.accept(self, env)
    if not isinstance(l, bool): sys.exit("Type error")
    if l:
      return True
    r = exp.right.accept(self, env)
    if not isinstance(r, bool): sys.exit("Type error")
    return r

  def visit_ifThenElse(self, exp, env):
    cond = exp.cond.accept(self, env)
    if not isinstance(cond, bool): sys.exit("Type error")
    if cond:
      return exp.e0.accept(self, env)
    else:
      return exp.e1.accept(self, env)

class UseDefVisitor(Visitor):
  """
  The UseDefVisitor class reports the use of undefined variables. It takes
  as input an environment of defined variables, and produces, as output,
  the set of all the variables that are used without being defined.

  Examples:
  >>> e0 = Let('v', Add(Num(40), Num(2)), Mul(Var('v'), Var('v')))
  >>> e1 = Not(Eql(e0, Num(1764)))
  >>> ev = UseDefVisitor()
  >>> len(e1.accept(ev, set()))
  0

  >>> e0 = Let('v', Add(Num(40), Num(2)), Sub(Var('v'), Num(2)))
  >>> e1 = Lth(e0, Var('x'))
  >>> ev = UseDefVisitor()
  >>> len(e1.accept(ev, set()))
  1

  >>> e = Let('v', Add(Num(40), Var('v')), Sub(Var('v'), Num(2)))
  >>> ev = UseDefVisitor()
  >>> len(e.accept(ev, set()))
  1

  >>> e1 = Let('v', Add(Num(40), Var('v')), Sub(Var('v'), Num(2)))
  >>> e0 = Let('v', Num(3), e1)
  >>> ev = UseDefVisitor()
  >>> len(e0.accept(ev, set()))
  0
  """
  def visit_var(self, exp, env):
    return {exp.identifier} if exp.identifier not in env else set()
  def visit_bln(self, exp, env): return set()
  def visit_num(self, exp, env): return set()
  def visit_eql(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_add(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_sub(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_mul(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_div(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_leq(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_lth(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_neg(self, exp, env): return exp.exp.accept(self, env)
  def visit_not(self, exp, env): return exp.exp.accept(self, env)
  def visit_and(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_or(self, exp, env): return exp.left.accept(self, env) | exp.right.accept(self, env)
  def visit_ifThenElse(self, exp, env): return exp.cond.accept(self, env) | exp.e0.accept(self, env) | exp.e1.accept(self, env)
  def visit_let(self, exp, env):
    ud = exp.exp_def.accept(self, env)
    ne = env.copy()
    ne.add(exp.identifier)
    return ud | exp.exp_body.accept(self, ne)

def safe_eval(exp):
  """
  This method applies one simple semantic analysis onto an expression, before
  evaluating it: it checks if the expression contains free variables, there
  is, variables used without being defined.

  Example:
  >>> e0 = Let('v', Add(Num(40), Num(2)), Mul(Var('v'), Var('v')))
  >>> e1 = Not(Eql(e0, Num(1764)))
  >>> safe_eval(e1)
  Value is False

  >>> e0 = Let('v', Add(Num(40), Num(2)), Sub(Var('v'), Num(2)))
  >>> e1 = Lth(e0, Var('x'))
  >>> safe_eval(e1)
  Error: expression contains undefined variables.
  """
  ud = exp.accept(UseDefVisitor(), set())
  if len(ud) > 0:
    sys.exit("Def error")
  print(f"Value is {exp.accept(EvalVisitor(), {})}")