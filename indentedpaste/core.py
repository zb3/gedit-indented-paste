def get_line_indent(doc, bound, line):
    start = doc.get_iter_at_line(line)
    end = start.copy()

    while True:
        ch = end.get_char()

        if ch not in (' ', '\t'):
            break

        end.forward_char()

    if bound.compare(end) < 0:
        end = bound

    return start.get_text(end)

def get_min_indent_level(lines):
    min_level = -1

    for line in lines:
        level = 0
        empty = True

        for ch in line:
            if ch in (' ', '\t'):
                level += 1
            else:
                if ch != '\r':
                    empty = False

                break

        if not empty and (min_level == -1 or level < min_level):
            min_level = level

    return max(0, min_level)

def do_indented_paste(view, text):
    if not text:
        return

    doc = view.get_buffer()

    itr = doc.get_iter_at_mark(doc.get_insert())
    current_indent = get_line_indent(doc, itr, itr.get_line())

    text_lines = text.split('\n')

    min_indent_level = get_min_indent_level(text_lines)
    first_line = True
    new_lines = []

    for line in text_lines:
        if 0 < min_indent_level <= len(line):
            line = line[min_indent_level:]

        if not first_line:
            line = current_indent + line
        else:
            first_line = False

        new_lines.append(line)

    doc.begin_user_action()
    if doc.get_has_selection():
        start, end = doc.get_selection_bounds()
        doc.delete(start, end)
        itr = end

    doc.insert(itr, '\n'.join(new_lines))
    doc.end_user_action()

    view.scroll_mark_onscreen(doc.get_insert())

