#include <glib.h>
#include <gtk/gtk.h>
#include "../indentedpaste/gedit-indented-paste-core.h"

// To access static functions for testing, we include the .c file.
// This is a common pattern for unit testing static functions in C.
#include "../indentedpaste/gedit-indented-paste-core.c"

typedef struct
{
	GtkTextView *view;
	GtkTextBuffer *buffer;
} TestFixture;

static void
fixture_setup (TestFixture *fixture,
	       gconstpointer user_data)
{
	fixture->buffer = gtk_text_buffer_new (NULL);
	fixture->view = GTK_TEXT_VIEW(gtk_text_view_new_with_buffer (fixture->buffer));
	g_object_ref_sink (fixture->view);

}

static void
fixture_teardown (TestFixture *fixture,
		  gconstpointer user_data)
{
	g_object_unref (fixture->buffer);
	g_object_unref (fixture->view);
}

static void
test_get_common_indent_width_basic (void)
{
	const gchar *text = "\n    a\n      b\n        c\n  d\n\n \n         e\n";
	g_assert_cmpint (get_common_indent_width (text), ==, 2);
}

static void
test_get_common_indent_width_reliable_delta (void)
{
	const gchar *text = "\n     f\n    f\n    a\n        b\n           c\n  d\n\n \n         e\n";
	g_assert_cmpint (get_common_indent_width (text), ==, 2);
}

static void
test_get_common_indent_width_tabs (void)
{
	const gchar *text = "\na\n\tb\n\t\tc\n\td\ne\n";
	g_assert_cmpint (get_common_indent_width (text), ==, 0);
}

static void
test_get_common_indent_width_no_trailing_newline (void)
{
	const gchar *text = "   x";
	g_assert_cmpint (get_common_indent_width (text), ==, 3);
}

static void
test_get_common_indent_width_carriage_return (void)
{
	const gchar *text = "\n  x\n \r\n  x\n";
	g_assert_cmpint (get_common_indent_width (text), ==, 2);
}

static void
test_get_common_indent_width_empty (void)
{
	const gchar *text = "";
	g_assert_cmpint (get_common_indent_width (text), ==, 0);
}

static void
test_get_current_indent_simple (TestFixture *fixture,
				gconstpointer ignored)
{
	const gchar *text = "line1\n  line2\n    line3";
	gtk_text_buffer_set_text (fixture->buffer, text, -1);

	GtkTextIter iter;
	gtk_text_buffer_get_end_iter (fixture->buffer, &iter);
	gtk_text_buffer_place_cursor (fixture->buffer, &iter);

	gchar *indent = get_current_indent (fixture->buffer);
	g_assert_cmpstr (indent, ==, "    ");
	g_free (indent);
}

static void
test_get_current_indent_bound (TestFixture *fixture,
			       gconstpointer ignored)
{
	const gchar *text = "line1\n  line2\n  ";
	gtk_text_buffer_set_text (fixture->buffer, text, -1);

	GtkTextIter iter;
	gtk_text_buffer_get_end_iter (fixture->buffer, &iter);
	gtk_text_buffer_place_cursor (fixture->buffer, &iter);

	gchar *indent = get_current_indent (fixture->buffer);
	g_assert_cmpstr (indent, ==, "  ");
	g_free (indent);

	gtk_text_buffer_insert_at_cursor (fixture->buffer, " ", 1);
	indent = get_current_indent (fixture->buffer);
	g_assert_cmpstr (indent, ==, "   ");
	g_free (indent);
}

static void
test_do_indented_paste_empty (TestFixture *fixture,
			      gconstpointer ignored)
{
	gtk_text_buffer_set_text (fixture->buffer, "abc", -1);
	GtkTextIter iter;
	gtk_text_buffer_get_iter_at_offset (fixture->buffer, &iter, 1);
	gtk_text_buffer_place_cursor (fixture->buffer, &iter);

	do_indented_paste (fixture->view, NULL);

	GtkTextIter start_iter;
	GtkTextIter end_iter;
	gtk_text_buffer_get_start_iter (fixture->buffer, &start_iter);
	gtk_text_buffer_get_end_iter (fixture->buffer, &end_iter);

	gchar *buffer_text = gtk_text_buffer_get_text (fixture->buffer,
						       &start_iter,
						       &end_iter,
						       FALSE);

	g_assert_cmpstr (buffer_text, ==, "abc");
	g_free (buffer_text);
}

static void
test_do_indented_paste_simple (TestFixture *fixture,
			       gconstpointer ignored)
{
	gtk_text_buffer_set_text (fixture->buffer, "abc", -1);
	GtkTextIter iter;
	gtk_text_buffer_get_iter_at_offset (fixture->buffer, &iter, 1);
	gtk_text_buffer_place_cursor (fixture->buffer, &iter);

	do_indented_paste (fixture->view, "test");

	GtkTextIter start_iter;
	GtkTextIter end_iter;
	gtk_text_buffer_get_start_iter (fixture->buffer, &start_iter);
	gtk_text_buffer_get_end_iter (fixture->buffer, &end_iter);

	gchar *buffer_text = gtk_text_buffer_get_text (fixture->buffer,
						       &start_iter,
						       &end_iter,
						       FALSE);
	g_assert_cmpstr (buffer_text, ==, "atestbc");
	g_free (buffer_text);
}

static void
test_do_indented_paste_basic (TestFixture *fixture,
			      gconstpointer ignored)
{
	const gchar *initial_text = "   line1\n\n \n    line2\n  ";
	const gchar *pasted_text = "  a\n b\n    c\n\n       d";
	const gchar *expected_text = "   line1\n\n \n    line2\n   a\n  b\n     c\n\n        d";

	gtk_text_buffer_set_text (fixture->buffer, initial_text, -1);
	GtkTextIter iter;
	gtk_text_buffer_get_end_iter (fixture->buffer, &iter);
	gtk_text_buffer_place_cursor (fixture->buffer, &iter);

	do_indented_paste (fixture->view, pasted_text);

	GtkTextIter start_iter;
	GtkTextIter end_iter;
	gtk_text_buffer_get_start_iter (fixture->buffer, &start_iter);
	gtk_text_buffer_get_end_iter (fixture->buffer, &end_iter);

	gchar *buffer_text = gtk_text_buffer_get_text (fixture->buffer,
						       &start_iter,
						       &end_iter,
						       FALSE);
	g_assert_cmpstr (buffer_text, ==, expected_text);
	g_free (buffer_text);
}

int
main (int argc, char **argv)
{
	gtk_init (&argc, &argv);
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/core/get_common_indent_width/basic", test_get_common_indent_width_basic);
	g_test_add_func ("/core/get_common_indent_width/reliable_delta", test_get_common_indent_width_reliable_delta);
	g_test_add_func ("/core/get_common_indent_width/tabs", test_get_common_indent_width_tabs);
	g_test_add_func ("/core/get_common_indent_width/no_trailing_newline", test_get_common_indent_width_no_trailing_newline);
	g_test_add_func ("/core/get_common_indent_width/carriage_return", test_get_common_indent_width_carriage_return);
	g_test_add_func ("/core/get_common_indent_width/empty", test_get_common_indent_width_empty);

	g_test_add ("/core/get_current_indent/simple", TestFixture, NULL, fixture_setup, test_get_current_indent_simple, fixture_teardown);
	g_test_add ("/core/get_current_indent/bound", TestFixture, NULL, fixture_setup, test_get_current_indent_bound, fixture_teardown);

	g_test_add ("/core/do_indented_paste/empty", TestFixture, NULL, fixture_setup, test_do_indented_paste_empty, fixture_teardown);
	g_test_add ("/core/do_indented_paste/simple", TestFixture, NULL, fixture_setup, test_do_indented_paste_simple, fixture_teardown);
	g_test_add ("/core/do_indented_paste/basic", TestFixture, NULL, fixture_setup, test_do_indented_paste_basic, fixture_teardown);

	return g_test_run ();
}
