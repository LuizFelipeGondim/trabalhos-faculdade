import sys
from Expression import *
from Lexer import Token, TokenType

class Parser:
  def __init__(self, tokens):
    self.tokens = [tk for tk in tokens if tk.kind != TokenType.NLN]
    self.cur_token_idx = 0 

  def current(self):
    if self.cur_token_idx < len(self.tokens):
      return self.tokens[self.cur_token_idx]
    return None

  def consume(self, expected_kind=None):
    tk = self.current()
    if expected_kind and tk and tk.kind != expected_kind:
      raise Exception(f"Expected token {expected_kind}, got {tk.kind}")
    self.cur_token_idx += 1
    return tk

  def parse(self):
    try:
      exp = self.parse_ifThenElse()
      if self.current() is not None:
        raise Exception("Tokens restantes")
      return exp
    except Exception as e:
      sys.exit(f"Parse error: {e}")

  def parse_ifThenElse(self):
    tk = self.current()
    if tk and tk.kind == TokenType.IFX:
      self.consume(TokenType.IFX)
      cond = self.parse_ifThenElse()
      self.consume(TokenType.THN)
      e0 = self.parse_ifThenElse()
      self.consume(TokenType.ELS)
      e1 = self.parse_ifThenElse()
      return IfThenElse(cond, e0, e1)
    return self.parse_or()

  def parse_or(self):
    left = self.parse_and()
    while True:
      tk = self.current()
      if tk and tk.kind == TokenType.ORX:
        self.consume()
        right = self.parse_and()
        left = Or(left, right)
      else:
        break
    return left

  def parse_and(self):
    left = self.parse_eql()
    while True:
      tk = self.current()
      if tk and tk.kind == TokenType.AND:
        self.consume()
        right = self.parse_eql()
        left = And(left, right)
      else:
        break
    return left

  def parse_eql(self):
    left = self.parse_relational()
    while True:
      tk = self.current()
      if tk and tk.kind == TokenType.EQL:
        self.consume()
        right = self.parse_relational()
        left = Eql(left, right)
      else:
        break
    return left

  def parse_relational(self):
    left = self.parse_additive()
    while True:
      tk = self.current()
      if tk and tk.kind in (TokenType.LEQ, TokenType.LTH):
        op = self.consume()
        right = self.parse_additive()
        if op.kind == TokenType.LEQ:
          left = Leq(left, right)
        else:
          left = Lth(left, right)
      else:
        break
    return left

  def parse_additive(self):
    left = self.parse_multiplicative()
    while True:
      tk = self.current()
      if tk and tk.kind in (TokenType.ADD, TokenType.SUB):
        op = self.consume()
        right = self.parse_multiplicative()
        if op.kind == TokenType.ADD:
          left = Add(left, right)
        else:
          left = Sub(left, right)
      else:
        break
    return left

  def parse_multiplicative(self):
    left = self.parse_unary()
    while True:
      tk = self.current()
      if tk and tk.kind in (TokenType.MUL, TokenType.DIV):
        op = self.consume()
        right = self.parse_unary()
        if op.kind == TokenType.MUL:
          left = Mul(left, right)
        else:
          left = Div(left, right)
      else:
        break
    return left

  def parse_unary(self):
    tk = self.current()
    if tk and tk.kind == TokenType.NEG:
      self.consume()
      return Neg(self.parse_unary())
    if tk and tk.kind == TokenType.NOT:
      self.consume()
      return Not(self.parse_unary())
    return self.parse_primary()

  def parse_primary(self):
    tk = self.current()
    if not tk:
      raise Exception("EOF")
    
    if tk.kind == TokenType.NUM:
      self.consume()
      return Num(tk.text)
    elif tk.kind == TokenType.TRU:
      self.consume()
      return Bln(True)
    elif tk.kind == TokenType.FLS:
      self.consume()
      return Bln(False)
    elif tk.kind == TokenType.LPR:
      self.consume()
      exp = self.parse_ifThenElse()
      self.consume(TokenType.RPR)
      return exp
    elif tk.kind == TokenType.LET:
      self.consume()
      var_tk = self.consume(TokenType.VAR)
      self.consume(TokenType.ASN)
      exp_def = self.parse_ifThenElse()
      self.consume(TokenType.INX)
      exp_body = self.parse_ifThenElse()
      self.consume(TokenType.END)
      return Let(var_tk.text, exp_def, exp_body)
    elif tk.kind == TokenType.VAR:
      self.consume()
      return Var(tk.text)
    else:
      raise Exception(f"Unexpected {tk.kind}")