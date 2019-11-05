from core import get_line_indent

from .mocks import *

def test_simple():
    text = """line1
  line2
    """
    cursor = len(text)
    text += "line3"
    
    doc = BufferMock(text=text, cursor=cursor)
    bound = doc.get_iter_at_mark(doc.get_insert())
    
    assert get_line_indent(doc, bound, 2) == '    '

def test_bound():
    text = """line1
  line2
  """
    cursor = len(text)
    text += " "
    
    doc = BufferMock(text=text, cursor=cursor)
    bound = doc.get_iter_at_mark(doc.get_insert())
    
    assert get_line_indent(doc, bound, 2) == '  '
    
    bound.forward_char()
    
    assert get_line_indent(doc, bound, 2) == '   '

