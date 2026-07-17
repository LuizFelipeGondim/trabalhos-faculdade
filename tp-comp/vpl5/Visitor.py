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
  def visit_var(self, exp, arg):
    pass

  @abstractmethod
  def visit_bln(self, exp, arg):
    pass

  @abstractmethod
  def visit_num(self, exp, arg):
    pass

  @abstractmethod
  def visit_eql(self, exp, arg):
    pass

  @abstractmethod
  def visit_add(self, exp, arg):
    pass

  @abstractmethod
  def visit_sub(self, exp, arg):
    pass

  @abstractmethod
  def visit_mul(self, exp, arg):
    pass

  @abstractmethod
  def visit_div(self, exp, arg):
    pass

  @abstractmethod
  def visit_leq(self, exp, arg):
    pass

  @abstractmethod
  def visit_lth(self, exp, arg):
    pass

  @abstractmethod
  def visit_neg(self, exp, arg):
    pass

  @abstractmethod
  def visit_not(self, exp, arg):
    pass

  @abstractmethod
  def visit_let(self, exp, arg):
    pass

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
      print(f"Variavel inexistente {exp.identifier}")
      import sys
      sys.exit(0)
        
    return env[exp.identifier]

  def visit_bln(self, exp, env):
    if isinstance(exp.bln, str):
      return exp.bln.lower() == 'true'
    return exp.bln

  def visit_num(self, exp, env):
    return int(exp.num)

  def visit_eql(self, exp, env):
    return exp.left.accept(self, env) == exp.right.accept(self, env)

  def visit_add(self, exp, env):
    return exp.left.accept(self, env) + exp.right.accept(self, env)

  def visit_sub(self, exp, env):
    return exp.left.accept(self, env) - exp.right.accept(self, env)

  def visit_mul(self, exp, env):
    return exp.left.accept(self, env) * exp.right.accept(self, env)

  def visit_div(self, exp, env):
    return exp.left.accept(self, env) // exp.right.accept(self, env)

  def visit_leq(self, exp, env):
    return exp.left.accept(self, env) <= exp.right.accept(self, env)

  def visit_lth(self, exp, env):
    return exp.left.accept(self, env) < exp.right.accept(self, env)

  def visit_neg(self, exp, env):
    return -exp.exp.accept(self, env)

  def visit_not(self, exp, env):
    return not exp.exp.accept(self, env)

  def visit_let(self, exp, env):
    val0 = exp.exp_def.accept(self, env)
    new_env = env.copy()
    new_env[exp.identifier] = val0
    return exp.exp_body.accept(self, new_env)

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
    if exp.identifier not in env:
      return {exp.identifier}
    return set()

  def visit_bln(self, exp, env):
    return set()

  def visit_num(self, exp, env):
    return set()

  def visit_eql(self, exp, env):
    return exp.left.accept(self, env).union(exp.right.accept(self, env))

  def visit_add(self, exp, env):
    return exp.left.accept(self, env).union(exp.right.accept(self, env))

  def visit_sub(self, exp, env):
    return exp.left.accept(self, env).union(exp.right.accept(self, env))

  def visit_mul(self, exp, env):
    return exp.left.accept(self, env).union(exp.right.accept(self, env))

  def visit_div(self, exp, env):
    return exp.left.accept(self, env).union(exp.right.accept(self, env))

  def visit_leq(self, exp, env):
    return exp.left.accept(self, env).union(exp.right.accept(self, env))

  def visit_lth(self, exp, env):
    return exp.left.accept(self, env).union(exp.right.accept(self, env))

  def visit_neg(self, exp, env):
    return exp.exp.accept(self, env)

  def visit_not(self, exp, env):
    return exp.exp.accept(self, env)

  def visit_let(self, exp, env):
    undefs_def = exp.exp_def.accept(self, env)
    new_env = env.copy()
    new_env.add(exp.identifier)
    undefs_body = exp.exp_body.accept(self, new_env)
    return undefs_def.union(undefs_body)

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
  visitor_use_def = UseDefVisitor()
  undefs = exp.accept(visitor_use_def, set())

  if len(undefs) > 0:
    print("Error: expression contains undefined variables.")
  else:
    visitor_eval = EvalVisitor()
    val = exp.accept(visitor_eval, {})
    print(f"Value is {val}")