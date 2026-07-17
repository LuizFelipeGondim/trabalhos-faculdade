import sys
from Expression import *
from Lexer import Token, TokenType

"""
This file implements the parser of arithmetic expressions.

References:
  see https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
"""

class Parser:
  def __init__(self, tokens):
    """
    Initializes the parser. The parser keeps track of the list of tokens
    and the current token. For instance:
    """
    self.tokens = list(tokens)
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
    """
    Returns the expression associated with the stream of tokens.

    Examples:
    >>> parser = Parser([Token('123', TokenType.NUM)])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    123

    >>> parser = Parser([Token('True', TokenType.TRU)])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    True

    >>> parser = Parser([Token('False', TokenType.FLS)])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    False

    >>> tk0 = Token('~', TokenType.NEG)
    >>> tk1 = Token('123', TokenType.NUM)
    >>> parser = Parser([tk0, tk1])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    -123

    >>> tk0 = Token('3', TokenType.NUM)
    >>> tk1 = Token('*', TokenType.MUL)
    >>> tk2 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    12

    >>> tk0 = Token('3', TokenType.NUM)
    >>> tk1 = Token('*', TokenType.MUL)
    >>> tk2 = Token('~', TokenType.NEG)
    >>> tk3 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2, tk3])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    -12

    >>> tk0 = Token('30', TokenType.NUM)
    >>> tk1 = Token('/', TokenType.DIV)
    >>> tk2 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    7

    >>> tk0 = Token('3', TokenType.NUM)
    >>> tk1 = Token('+', TokenType.ADD)
    >>> tk2 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    7

    >>> tk0 = Token('30', TokenType.NUM)
    >>> tk1 = Token('-', TokenType.SUB)
    >>> tk2 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    26

    >>> tk0 = Token('2', TokenType.NUM)
    >>> tk1 = Token('*', TokenType.MUL)
    >>> tk2 = Token('(', TokenType.LPR)
    >>> tk3 = Token('3', TokenType.NUM)
    >>> tk4 = Token('+', TokenType.ADD)
    >>> tk5 = Token('4', TokenType.NUM)
    >>> tk6 = Token(')', TokenType.RPR)
    >>> parser = Parser([tk0, tk1, tk2, tk3, tk4, tk5, tk6])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    14

    >>> tk0 = Token('4', TokenType.NUM)
    >>> tk1 = Token('==', TokenType.EQL)
    >>> tk2 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    True

    >>> tk0 = Token('4', TokenType.NUM)
    >>> tk1 = Token('<=', TokenType.LEQ)
    >>> tk2 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    True

    >>> tk0 = Token('4', TokenType.NUM)
    >>> tk1 = Token('<', TokenType.LTH)
    >>> tk2 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    False

    >>> tk0 = Token('not', TokenType.NOT)
    >>> tk1 = Token('4', TokenType.NUM)
    >>> tk2 = Token('<', TokenType.LTH)
    >>> tk3 = Token('4', TokenType.NUM)
    >>> parser = Parser([tk0, tk1, tk2, tk3])
    >>> exp = parser.parse()
    >>> exp.eval(None)
    True

    >>> tk0 = Token('let', TokenType.LET)
    >>> tk1 = Token('v', TokenType.VAR)
    >>> tk2 = Token('<-', TokenType.ASN)
    >>> tk3 = Token('42', TokenType.NUM)
    >>> tk4 = Token('in', TokenType.INX)
    >>> tk5 = Token('v', TokenType.VAR)
    >>> tk6 = Token('end', TokenType.END)
    >>> parser = Parser([tk0, tk1, tk2, tk3, tk4, tk5, tk6])
    >>> exp = parser.parse()
    >>> exp.eval({})
    42

    >>> tk0 = Token('let', TokenType.LET)
    >>> tk1 = Token('v', TokenType.VAR)
    >>> tk2 = Token('<-', TokenType.ASN)
    >>> tk3 = Token('21', TokenType.NUM)
    >>> tk4 = Token('in', TokenType.INX)
    >>> tk5 = Token('v', TokenType.VAR)
    >>> tk6 = Token('+', TokenType.ADD)
    >>> tk7 = Token('v', TokenType.VAR)
    >>> tk8 = Token('end', TokenType.END)
    >>> parser = Parser([tk0, tk1, tk2, tk3, tk4, tk5, tk6, tk7, tk8])
    >>> exp = parser.parse()
    >>> exp.eval({})
    42
    """
    return self.parse_not()

  def parse_not(self):
    tk = self.current()
    if tk and tk.kind == TokenType.NOT:
      self.consume()
      exp = self.parse_relational()
      return Not(exp)
    return self.parse_relational()

  def parse_relational(self):
    left = self.parse_additive()
    while True:
      tk = self.current()
      if tk and tk.kind in (TokenType.EQL, TokenType.LEQ, TokenType.LTH):
        op = self.consume()
        right = self.parse_additive()
        if op.kind == TokenType.EQL:
            left = Eql(left, right)
        elif op.kind == TokenType.LEQ:
            left = Leq(left, right)
        elif op.kind == TokenType.LTH:
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
      exp = self.parse_primary()
      return Neg(exp)
    return self.parse_primary()

  def parse_primary(self):
    tk = self.current()
    if not tk:
      raise Exception("Unexpected End Of File")
    
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
      exp = self.parse()
      self.consume(TokenType.RPR)
      return exp
        
    elif tk.kind == TokenType.LET:
      self.consume()
      var_tk = self.consume(TokenType.VAR)
      self.consume(TokenType.ASN)
      exp_def = self.parse()
      self.consume(TokenType.INX)
      exp_body = self.parse()
      self.consume(TokenType.END)
      return Let(var_tk.text, exp_def, exp_body)
        
    elif tk.kind == TokenType.VAR:
      self.consume()
      return Var(tk.text)
        
    else:
      raise Exception(f"Unexpected token {tk.kind} at {tk.text}")