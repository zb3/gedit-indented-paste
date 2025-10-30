#include "gedit-indented-paste-core.h"
#include <gtksourceview/gtksource.h>
#include <string.h>

static gint
get_line_indent_width (const gchar *line)
{
	gint i = 0;
	while (line[i] == ' ' || line[i] == '\t')
	{
		i++;
	}
	return i;
}

static gchar *
get_current_indent (GtkTextBuffer *buffer)
{
	GtkTextIter iter;
	GtkTextIter line_start;
	gchar *indent;
	gint indent_width;

	gtk_text_buffer_get_iter_at_mark (buffer, &iter, gtk_text_buffer_get_insert (buffer));
	line_start = iter;
	gtk_text_iter_set_line_offset (&line_start, 0);

	indent = gtk_text_buffer_get_text (buffer, &line_start, &iter, FALSE);

	indent_width = get_line_indent_width(indent);

	indent[indent_width] = '\0';
	return indent;
}

static gint
get_common_indent_width (const gchar *text)
{
	gchar **lines = g_strsplit (text, "\n", -1);
	gint min_indent = -1;
	gint i;

	for (i = 0; lines[i] != NULL; i++)
	{
		gchar *line = lines[i];
		gchar *trimmed_line = g_strstrip (g_strdup (line));

		if (strlen (trimmed_line) > 0)
		{
			gint current_indent = get_line_indent_width (line);
			if (min_indent == -1 || current_indent < min_indent)
			{
				min_indent = current_indent;
			}
		}
		g_free (trimmed_line);
	}

	g_strfreev (lines);

	return (min_indent > 0) ? min_indent : 0;
}

static gchar *
process_text_for_pasting (const gchar *text, const gchar *current_indent)
{
	GString *result = g_string_new ("");
	gchar **lines = g_strsplit (text, "\n", -1);
	gint common_indent_width = get_common_indent_width (text);
	gboolean first_line = TRUE;
	gint i;

	for (i = 0; lines[i] != NULL; i++)
	{
		gchar *line = lines[i];
		gint line_len = strlen (line);

		if (!first_line)
		{
			g_string_append_c (result, '\n');
		}

		gchar *trimmed_line = g_strstrip (g_strdup (line));
		gboolean is_blank = (strlen (trimmed_line) == 0);
		g_free (trimmed_line);

		if (!first_line && !is_blank)
		{
			g_string_append (result, current_indent);
		}

		if (!is_blank && line_len > common_indent_width)
		{
			g_string_append (result, line + common_indent_width);
		}
		else if (is_blank)
		{
			// Append nothing for blank lines, they will just be a newline
		}
		else
		{
			g_string_append (result, line);
		}

		first_line = FALSE;
	}

	g_strfreev (lines);

	return g_string_free (result, FALSE);
}

void
do_indented_paste (GtkTextView   *view,
		   const gchar *text)
{
	GtkTextBuffer *buffer;
	gchar *current_indent;
	gchar *processed_text;

	if (text == NULL)
	{
		return;
	}

	buffer = gtk_text_view_get_buffer (view);
	if (buffer == NULL)
	{
		return;
	}

	current_indent = get_current_indent (buffer);
	processed_text = process_text_for_pasting (text, current_indent);

	gtk_text_buffer_begin_user_action (buffer);
	gtk_text_buffer_insert_at_cursor (buffer, processed_text, -1);
	gtk_text_buffer_end_user_action (buffer);

	g_free (current_indent);
	g_free (processed_text);
}
