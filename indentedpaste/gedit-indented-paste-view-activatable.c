#include "gedit-indented-paste-view-activatable.h"
#include <gedit/gedit-view.h>
#include <gedit/gedit-view-activatable.h>
#include <gtk/gtk.h>
#include "gedit-indented-paste-core.h"

struct _GeditIndentedPasteViewActivatablePrivate
{
	GeditView *view;
	gulong button_press_handler_id;
};

enum
{
	PROP_0,
	PROP_VIEW
};

void gedit_indented_paste_view_activatable_do_paste (GeditView *view, gboolean primary);
static void on_clipboard_text_received (GtkClipboard *clipboard, const gchar *text, gpointer user_data);
static gboolean on_button_press (GtkWidget *widget, GdkEventButton *event, gpointer user_data);

static void gedit_view_activatable_iface_init (GeditViewActivatableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (GeditIndentedPasteViewActivatable,
				gedit_indented_paste_view_activatable,
				G_TYPE_OBJECT,
				0,
				G_ADD_PRIVATE_DYNAMIC (GeditIndentedPasteViewActivatable)
				G_IMPLEMENT_INTERFACE_DYNAMIC (GEDIT_TYPE_VIEW_ACTIVATABLE,
							       gedit_view_activatable_iface_init))

static void
gedit_indented_paste_view_activatable_get_property (GObject    *object,
						    guint       prop_id,
						    GValue     *value,
						    GParamSpec *pspec)
{
	GeditIndentedPasteViewActivatable *activatable = GEDIT_INDENTED_PASTE_VIEW_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_VIEW:
			g_value_set_object (value, activatable->priv->view);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gedit_indented_paste_view_activatable_set_property (GObject      *object,
						    guint         prop_id,
						    const GValue *value,
						    GParamSpec   *pspec)
{
	GeditIndentedPasteViewActivatable *activatable = GEDIT_INDENTED_PASTE_VIEW_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_VIEW:
			g_assert (activatable->priv->view == NULL);
			activatable->priv->view = GEDIT_VIEW (g_value_dup_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gedit_indented_paste_view_activatable_dispose (GObject *object)
{
	GeditIndentedPasteViewActivatable *activatable = GEDIT_INDENTED_PASTE_VIEW_ACTIVATABLE (object);

	g_clear_object (&activatable->priv->view);

	G_OBJECT_CLASS (gedit_indented_paste_view_activatable_parent_class)->dispose (object);
}

static void
gedit_indented_paste_view_activatable_finalize (GObject *object)
{

	G_OBJECT_CLASS (gedit_indented_paste_view_activatable_parent_class)->finalize (object);
}

static void
gedit_indented_paste_view_activatable_class_init (GeditIndentedPasteViewActivatableClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = gedit_indented_paste_view_activatable_get_property;
	object_class->set_property = gedit_indented_paste_view_activatable_set_property;
	object_class->dispose = gedit_indented_paste_view_activatable_dispose;
	object_class->finalize = gedit_indented_paste_view_activatable_finalize;

	g_object_class_override_property (object_class, PROP_VIEW, "view");
}

static void
gedit_indented_paste_view_activatable_class_finalize (GeditIndentedPasteViewActivatableClass *klass)
{
}

static void
gedit_indented_paste_view_activatable_init (GeditIndentedPasteViewActivatable *activatable)
{
	activatable->priv = gedit_indented_paste_view_activatable_get_instance_private (activatable);
}

static void
gedit_indented_paste_view_activatable_activate (GeditViewActivatable *activatable)
{
	GeditIndentedPasteViewActivatable *self = GEDIT_INDENTED_PASTE_VIEW_ACTIVATABLE (activatable);
	//g_object_set_data (G_OBJECT (self->priv->view), "gedit-indented-paste-view-activatable", self);

	self->priv->button_press_handler_id = g_signal_connect (self->priv->view,
								"button-press-event",
								G_CALLBACK (on_button_press),
								self);
}

static void
gedit_indented_paste_view_activatable_deactivate (GeditViewActivatable *activatable)
{
	GeditIndentedPasteViewActivatable *self = GEDIT_INDENTED_PASTE_VIEW_ACTIVATABLE (activatable);

	if (self->priv->button_press_handler_id > 0)
	{
		g_signal_handler_disconnect (self->priv->view, self->priv->button_press_handler_id);
		self->priv->button_press_handler_id = 0;
	}

	//g_object_set_data (G_OBJECT (self->priv->view), "gedit-indented-paste-view-activatable", NULL);
}

static void
on_clipboard_text_received (GtkClipboard *clipboard,
			    const gchar  *text,
			    gpointer      user_data)
{
	GeditView *view = GEDIT_VIEW (user_data);

	if (text != NULL && view != NULL)
	{
		do_indented_paste (GTK_TEXT_VIEW (view), text);
	}
}

void
gedit_indented_paste_view_activatable_do_paste (GeditView *view, gboolean primary)
{
	GtkClipboard *clipboard;
	GdkAtom selection = primary ? GDK_SELECTION_PRIMARY : GDK_SELECTION_CLIPBOARD;

	clipboard = gtk_clipboard_get_for_display (gtk_widget_get_display (GTK_WIDGET (view)), selection);
	gtk_clipboard_request_text (clipboard, on_clipboard_text_received, view);
}

static gboolean
on_button_press (GtkWidget      *widget,
		 GdkEventButton *event,
		 gpointer        user_data)
{
	GeditView *view = GEDIT_VIEW (widget);
	GdkModifierType state;

	state = gtk_accelerator_get_default_mod_mask ();

	if (event->button == 2 && (event->state & state) == GDK_SHIFT_MASK)
	{
		gedit_indented_paste_view_activatable_do_paste (view, TRUE);
		return TRUE;
	}

	return FALSE;
}

static void
gedit_view_activatable_iface_init (GeditViewActivatableInterface *iface)
{
	iface->activate = gedit_indented_paste_view_activatable_activate;
	iface->deactivate = gedit_indented_paste_view_activatable_deactivate;
}

void
gedit_indented_paste_view_activatable_register (PeasObjectModule *module)
{
	gedit_indented_paste_view_activatable_register_type (G_TYPE_MODULE (module));

	peas_object_module_register_extension_type (module,
						    GEDIT_TYPE_VIEW_ACTIVATABLE,
						    GEDIT_TYPE_INDENTED_PASTE_VIEW_ACTIVATABLE);
}
