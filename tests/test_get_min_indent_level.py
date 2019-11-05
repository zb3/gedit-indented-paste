import pytest

from core import get_min_indent_level

def test_basic():
    case = """
    a
      b
        c
  d

 
         e
"""
    assert get_min_indent_level(case.split('\n')) == 2

    
def test_reliable_delta():
    case = """
     f
    f
    a
        b
           c
  d

 
         e
"""
    assert get_min_indent_level(case.split('\n')) == 2


def test_tabs():
    case = """
a
	b
		c
	d
e
"""
    assert get_min_indent_level(case.split('\n')) == 0
    
    case = """
a
	b
		c
	d
e
"""
    assert get_min_indent_level(case.split('\n')) == 0

@pytest.mark.parametrize("case", [
    """
   x""", "   x"
])
def test_no_trailing_newline(case):
    assert get_min_indent_level(case.split('\n')) == 3
    
def test_carriage_return():
    case = """
  x
 \r
  x
"""
    assert get_min_indent_level(case.split('\n')) == 2    

def test_empty():
    case = ""
    assert get_min_indent_level(case.split('\n')) == 0

