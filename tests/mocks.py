class IterMock:
    def __init__(self, buffer, offset):
        self.buffer = buffer
        self.offset = offset

    def get_char(self):
        if self.offset < len(self.buffer.text):
            return self.buffer.text[self.offset]

        return ''

    def forward_char(self):
        if self.offset < len(self.buffer.text):
            self.offset += 1

        return self.offset < len(self.buffer.text)

    def copy(self):
        return IterMock(self.buffer, self.offset)

    def compare(self, itr):
        if self.offset == itr.offset:
            return 0

        return -1 if self.offset < itr.offset else 1

    def get_text(self, itr):
        return self.buffer.text[self.offset:itr.offset]

    def get_line(self):
        line = -1

        for offset in self.buffer.get_line_offsets():
            if offset > self.offset:
                break

            line += 1

        return line


class BufferMock:
    def __init__(self, text='x', cursor=0, selection_bound=None):
        self.text = text
        self.cursor = cursor

        if selection_bound is None:
            self.selection_bound = cursor
        else:
            self.selection_bound = selection_bound

    def get_insert(self):
        return self.cursor

    def get_iter_at_mark(self, offset):
        return IterMock(self, offset)

    def get_line_offsets(self):
        offset = 0
        yield offset

        while True:
            try:
                offset = self.text.index('\n', offset) + 1
                yield offset
            except ValueError:
                break


    def get_iter_at_line(self, target):
        line = 0

        for offset in self.get_line_offsets():
            if line == target:
                break

            line += 1

        return IterMock(self, offset)

    def insert(self, itr, what):
        self.text = self.text[:itr.offset] + what + self.text[itr.offset:]
        itr.offset += len(what)

    def delete(self, start_itr, end_itr):
        self.text = self.text[:start_itr.offset] + self.text[end_itr.offset:]
        end_itr.offset -= end_itr.offset - start_itr.offset

    def begin_user_action(self):
        pass

    def end_user_action(self):
        pass

    def get_has_selection(self):
        return self.cursor != self.selection_bound

    def get_selection_bounds(self):
        return [IterMock(self, self.cursor), IterMock(self, self.selection_bound)]


class ViewMock:
    def __init__(self, buffer=BufferMock(), tab_width=4, use_spaces=True):
        self.tab_width = tab_width
        self.use_spaces = use_spaces
        self.buffer = buffer

    def get_tab_width(self):
        return self.tab_width

    def get_insert_spaces_instead_of_tabs(self):
        return self.use_spaces

    def get_buffer(self):
        return self.buffer

    def scroll_mark_onscreen(self, mark):
        pass

