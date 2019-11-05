import pytest

from core import do_indented_paste
#from unittest.mock import patch

from .mocks import *

def test_empty():
    doc = BufferMock(text='abc', cursor=1)
    
    do_indented_paste(ViewMock(doc), None)
    
    assert doc.text == 'abc'

def test_simple():
    doc = BufferMock(text='abc', cursor=1)
    
    do_indented_paste(ViewMock(doc),'test')
    
    assert doc.text == 'atestbc'

def test_simple_selection():
    doc = BufferMock(text='abc', cursor=1, selection_bound=2)
    
    do_indented_paste(ViewMock(doc),'test')
    
    assert doc.text == 'atestc'

def test_basic():
    text = """   line1

 
    line2
  """
    cursor = len(text)
    text += """
    line3
      line4"""
    
    doc = BufferMock(text=text, cursor=cursor)
    
    pasted_text = """  a
 b
    c

       d"""
            
    
    do_indented_paste(ViewMock(doc),pasted_text)

    assert doc.text == """   line1

 
    line2
   a
  b
     c
  
        d
    line3
      line4"""
      
def test_bound():
    text = """   line1

 
    line2
  """
    cursor = len(text)
    text += """  more indent
    line3
      line4"""

    doc = BufferMock(text=text, cursor=cursor)
    
    pasted_text = """  a
  b
      c
          d"""
            
    
    do_indented_paste(ViewMock(doc),pasted_text)

    assert doc.text == """   line1

 
    line2
  a
  b
      c
          d  more indent
    line3
      line4"""
      



