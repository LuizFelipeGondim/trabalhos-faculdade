import sys
import enum

class Token:
  def __init__(self, tokenText, tokenKind):
    self.text = tokenText
    self.kind = tokenKind

class TokenType(enum.Enum):
  EOF = -1
  NLN = 0
  WSP = 1
  COM = 2
  NUM = 3
  STR = 4
  TRU = 5
  FLS = 6
  VAR = 7
  LET = 8
  INX = 9
  END = 10
  EQL = 201
  ADD = 202
  SUB = 203
  MUL = 204
  DIV = 205
  LEQ = 206
  LTH = 207
  NEG = 208
  NOT = 209
  LPR = 210
  RPR = 211
  ASN = 212
  ORX = 213
  AND = 214
  IFX = 215
  THN = 216
  ELS = 217

class Lexer:
  def __init__(self, source):
    self.source = source
    self.pos = 0
    self.length = len(source)

  def peek(self):
    if self.pos + 1 < self.length:
      return self.source[self.pos + 1]
    return None

  def tokens(self):
    token = self.getToken()
    while token.kind != TokenType.EOF:
      if token.kind not in (TokenType.WSP, TokenType.COM, TokenType.NLN):
        yield token
      token = self.getToken()

  def getToken(self):
    if self.pos >= self.length:
      return Token("", TokenType.EOF)

    char = self.source[self.pos]

    if char in ' \t\r\f\v':
      start = self.pos
      while self.pos < self.length and self.source[self.pos] in ' \t\r\f\v':
        self.pos += 1
      return Token(self.source[start:self.pos], TokenType.WSP)

    if char == '\n':
      self.pos += 1
      return Token('', TokenType.NLN)

    if char == '-' and self.peek() == '-':
      start = self.pos
      while self.pos < self.length and self.source[self.pos] != '\n':
        self.pos += 1
      if self.pos < self.length and self.source[self.pos] == '\n':
        self.pos += 1
      return Token(self.source[start:self.pos], TokenType.COM)

    if char == '(' and self.peek() == '*':
      start = self.pos
      self.pos += 2  
      while self.pos < self.length:
        if self.source[self.pos] == '*' and self.peek() == ')':
          self.pos += 2  
          break
        self.pos += 1
      return Token(self.source[start:self.pos], TokenType.COM)

    if char == '"':
      start = self.pos
      self.pos += 1
      while self.pos < self.length and self.source[self.pos] != '"':
        self.pos += 1
      if self.pos < self.length:
        self.pos += 1 

      return Token(self.source[start+1:self.pos-1], TokenType.STR)

    if char.isdigit():
      start = self.pos
      while self.pos < self.length and self.source[self.pos].isdigit():
        self.pos += 1
      return Token(self.source[start:self.pos], TokenType.NUM)

    if char.isalpha():
      start = self.pos
      while self.pos < self.length and (self.source[self.pos].isalnum() or self.source[self.pos] == '_'):
        self.pos += 1
      text = self.source[start:self.pos]
      text_lower = text.lower()
      
      if text_lower == 'true':
        return Token(text, TokenType.TRU)
      if text_lower == 'false':
        return Token(text, TokenType.FLS)
      if text_lower == 'not':
        return Token(text, TokenType.NOT)
      if text_lower == 'let':
        return Token(text, TokenType.LET)
      if text_lower == 'in':
        return Token(text, TokenType.INX)
      if text_lower == 'end':
        return Token(text, TokenType.END)
      if text_lower == 'or':
        return Token(text, TokenType.ORX)
      if text_lower == 'and':
        return Token(text, TokenType.AND)
      if text_lower == 'if':
        return Token(text, TokenType.IFX)
      if text_lower == 'then':
        return Token(text, TokenType.THN)
      if text_lower == 'else':
        return Token(text, TokenType.ELS)
      if text_lower == 'div':
        return Token(text, TokenType.DIV)
          
      return Token(text, TokenType.VAR)

    if char == '<' and self.peek() == '-':
      self.pos += 2
      return Token('<-', TokenType.ASN)
    if char == '<' and self.peek() == '=':
      self.pos += 2
      return Token('<=', TokenType.LEQ)
    if char == '<':
      self.pos += 1
      return Token('<', TokenType.LTH)
    if char == '=' and self.peek() == '=':
      self.pos += 2
      return Token('==', TokenType.EQL)
    if char == '=':
      self.pos += 1
      return Token('=', TokenType.EQL)
    if char == '+':
      self.pos += 1
      return Token('+', TokenType.ADD)
    if char == '-':
      self.pos += 1
      return Token('-', TokenType.SUB)
    if char == '*':
      self.pos += 1
      return Token('*', TokenType.MUL)
    if char == '/':
      self.pos += 1
      return Token('/', TokenType.DIV)
    if char == '~':  
      self.pos += 1
      return Token('~', TokenType.NEG)
    if char == '(':
      self.pos += 1
      return Token('(', TokenType.LPR)
    if char == ')':
      self.pos += 1
      return Token(')', TokenType.RPR)

    bad_char = self.source[self.pos]
    self.pos += 1
    return Token(bad_char, TokenType.WSP)