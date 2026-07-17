import sys
import enum

class Token:
  """
  This class contains the definition of Tokens. A token has two fields: its
  text and its kind. The "kind" of a token is a constant that identifies it
  uniquely. See the TokenType to know the possible identifiers (if you want).
  You don't need to change this class.
  """
  def __init__(self, tokenText, tokenKind):
    # The token's actual text. Used for identifiers, strings, and numbers.
    self.text = tokenText
    # The TokenType that this token is classified as.
    self.kind = tokenKind

class TokenType(enum.Enum):
  """
  These are the possible tokens. You don't need to change this class at all.
  """
  EOF = -1  # End of file
  NLN = 0   # New line
  WSP = 1   # White Space
  COM = 2   # Comment
  NUM = 3   # Number (integers)
  STR = 4   # Strings
  TRU = 5   # The constant true
  FLS = 6   # The constant false
  VAR = 7   # An identifier
  LET = 8   # The 'let' of the let expression
  INX = 9   # The 'in' of the let expression
  END = 10  # The 'end' of the let expression
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
  ASN = 212 # The assignment '<-' operator
  ORX = 213
  AND = 214
  IFX = 215
  THN = 216
  ELS = 217

class Lexer:
  def __init__(self, source):
    """
    The constructor of the lexer. It receives the string that shall be
    scanned.
    """
    self.source = source
    self.pos = 0
    self.length = len(source)

  def peek(self):
    if self.pos + 1 < self.length:
      return self.source[self.pos + 1]
    return None

  def tokens(self):
    """
    This method is a token generator: it converts the string encapsulated
    into this object into a sequence of Tokens. Examples:

    >>> l = Lexer("1 + 3")
    >>> [tk.kind for tk in l.tokens()]
    [<TokenType.NUM: 3>, <TokenType.ADD: 202>, <TokenType.NUM: 3>]

    >>> l = Lexer('1 * 2 -- 3\\n')
    >>> [tk.kind for tk in l.tokens()]
    [<TokenType.NUM: 3>, <TokenType.MUL: 204>, <TokenType.NUM: 3>]

    >>> l = Lexer("1 + var")
    >>> [tk.kind for tk in l.tokens()]
    [<TokenType.NUM: 3>, <TokenType.ADD: 202>, <TokenType.VAR: 7>]

    >>> l = Lexer("let v <- 2 in v end")
    >>> [tk.kind.name for tk in l.tokens()]
    ['LET', 'VAR', 'ASN', 'NUM', 'INX', 'VAR', 'END']
    """
    token = self.getToken()
    while token.kind != TokenType.EOF:
      if token.kind != TokenType.WSP and token.kind != TokenType.COM and token.kind != TokenType.NLN:
        yield token
      token = self.getToken()

  def getToken(self):
    """
    Return the next token.
    """
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
      
      if text_lower == 'true' and text[0] == 't':
        return Token(text, TokenType.TRU)
      if text_lower == 'false' and text[0] == 'f':
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