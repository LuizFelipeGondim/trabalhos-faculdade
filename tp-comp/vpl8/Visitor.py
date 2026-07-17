import sys
from abc import ABC, abstractmethod
from Expression import *
import Asm as AsmModule

class Visitor(ABC):
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

class GenVisitor(Visitor):
  def __init__(self):
    self.var_count = 0

  def next_var_name(self):
    name = f"t{self.var_count}"
    self.var_count += 1
    return name

  def visit_var(self, exp, prog):
    return exp.identifier

  def visit_bln(self, exp, prog):
    v = self.next_var_name()
    val = 1 if exp.bln else 0
    prog.add_inst(AsmModule.Addi(v, "x0", val))
    return v

  def visit_num(self, exp, prog):
    v = self.next_var_name()
    prog.add_inst(AsmModule.Addi(v, "x0", int(exp.num)))
    return v

  def visit_add(self, exp, prog):
    l = exp.left.accept(self, prog)
    r = exp.right.accept(self, prog)
    v = self.next_var_name()
    prog.add_inst(AsmModule.Add(v, l, r))
    return v

  def visit_sub(self, exp, prog):
    l = exp.left.accept(self, prog)
    r = exp.right.accept(self, prog)
    v = self.next_var_name()
    prog.add_inst(AsmModule.Sub(v, l, r))
    return v

  def visit_mul(self, exp, prog):
    l = exp.left.accept(self, prog)
    r = exp.right.accept(self, prog)
    v = self.next_var_name()
    prog.add_inst(AsmModule.Mul(v, l, r))
    return v

  def visit_div(self, exp, prog):
    l = exp.left.accept(self, prog)
    r = exp.right.accept(self, prog)
    v = self.next_var_name()
    prog.add_inst(AsmModule.Div(v, l, r))
    return v

  def visit_lth(self, exp, prog):
    l = exp.left.accept(self, prog)
    r = exp.right.accept(self, prog)
    v = self.next_var_name()
    prog.add_inst(AsmModule.Slt(v, l, r))
    return v

  def visit_leq(self, exp, prog):
    l = exp.left.accept(self, prog)
    r = exp.right.accept(self, prog)
    t1 = self.next_var_name()
    v = self.next_var_name()
    prog.add_inst(AsmModule.Slt(t1, r, l))
    prog.add_inst(AsmModule.Xori(v, t1, 1))
    return v

  def visit_eql(self, exp, prog):
    l = exp.left.accept(self, prog)
    r = exp.right.accept(self, prog)
    t1 = self.next_var_name()
    t2 = self.next_var_name()
    t3 = self.next_var_name()
    v = self.next_var_name()
    prog.add_inst(AsmModule.Slt(t1, l, r))
    prog.add_inst(AsmModule.Slt(t2, r, l))
    prog.add_inst(AsmModule.Add(t3, t1, t2))
    prog.add_inst(AsmModule.Xori(v, t3, 1))
    return v

  def visit_neg(self, exp, prog):
    a = exp.exp.accept(self, prog)
    v = self.next_var_name()
    prog.add_inst(AsmModule.Sub(v, "x0", a))
    return v

  def visit_not(self, exp, prog):
    a = exp.exp.accept(self, prog)
    v = self.next_var_name()
    prog.add_inst(AsmModule.Xori(v, a, 1))
    return v

  def visit_let(self, exp, prog):
    val_reg = exp.exp_def.accept(self, prog)
    prog.add_inst(AsmModule.Add(exp.identifier, val_reg, "x0"))
    return exp.exp_body.accept(self, prog)