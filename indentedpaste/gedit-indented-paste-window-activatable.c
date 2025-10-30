#include "gedit-indented-paste-window-activatable.h"
#include <gedit/gedit-window.h>
#include <gedit/gedit-window-activatable.h>
#include <gedit/gedit-view.h>
#include "gedit-indented-paste-view-activatable.h"

struct _GeditIndentedPasteWindowActivatablePrivate
{
	GeditWindow *window;
};

enum
{
	PROP_0,
	PROP_WINDOW
};

static void gedit_window_activatable_iface_init (GeditWindowActivatableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (GeditIndentedPasteWindowActivatable,
				gedit_indented_paste_window_activatable,
				G_TYPE_OBJECT,
				0,
				G_ADD_PRIVATE_DYNAMIC (GeditIndentedPasteWindowActivatable)
				G_IMPLEMENT_INTERFACE_DYNAMIC (GEDIT_TYPE_WINDOW_ACTIVATABLE,
							       gedit_window_activatable_iface_init))

static void
gedit_indented_paste_window_activatable_get_property (GObject    *object,
						      guint       prop_id,
						      GValue     *value,
						      GParamSpec *pspec)
{
	GeditIndentedPasteWindowActivatable *activatable = GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_WINDOW:
			g_value_set_object (value, activatable->priv->window);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gedit_indented_paste_window_activatable_set_property (GObject      *object,
						      guint         prop_id,
						      const GValue *value,
						      GParamSpec   *pspec)
{
	GeditIndentedPasteWindowActivatable *activatable = GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_WINDOW:
			g_assert (activatable->priv->window == NULL);
			activatable->priv->window = GEDIT_WINDOW (g_value_dup_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gedit_indented_paste_window_activatable_dispose (GObject *object)
{
	GeditIndentedPasteWindowActivatable *activatable = GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE (object);

	g_clear_object (&activatable->priv->window);

	G_OBJECT_CLASS (gedit_indented_paste_window_activatable_parent_class)->dispose (object);
}

static void
gedit_indented_paste_window_activatable_finalize (GObject *object)
{

	G_OBJECT_CLASS (gedit_indented_paste_window_activatable_parent_class)->finalize (object);
}

static void
gedit_indented_paste_window_activatable_class_init (GeditIndentedPasteWindowActivatableClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = gedit_indented_paste_window_activatable_get_property;
	object_class->set_property = gedit_indented_paste_window_activatable_set_property;
	object_class->dispose = gedit_indented_paste_window_activatable_dispose;
	object_class->finalize = gedit_indented_paste_window_activatable_finalize;

	g_object_class_override_property (object_class, PROP_WINDOW, "window");
}

static void
gedit_indented_paste_window_activatable_class_finalize (GeditIndentedPasteWindowActivatableClass *klass)
{
}

static void
gedit_indented_paste_window_activatable_init (GeditIndentedPasteWindowActivatable *activatable)
{
	activatable->priv = gedit_indented_paste_window_activatable_get_instance_private (activatable);
}

static void
indented_paste_activate (GSimpleAction *action,
			 GVariant      *parameter,
			 gpointer       user_data)
{
	GeditIndentedPasteWindowActivatable *activatable = GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE (user_data);
	GeditView *view = gedit_window_get_active_view (activatable->priv->window);

	if (GEDIT_IS_VIEW (view))
	{
		gedit_indented_paste_view_activatable_do_paste (view, FALSE);
	}
}

static void
gedit_indented_paste_window_activatable_activate (GeditWindowActivatable *activatable)
{
	GeditIndentedPasteWindowActivatable *self = GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE (activatable);
	GSimpleAction *action;

	action = g_simple_action_new ("indented-paste", NULL);
	g_signal_connect (action, "activate", G_CALLBACK (indented_paste_activate), self);
	g_action_map_add_action (G_ACTION_MAP (self->priv->window), G_ACTION (action));
	g_object_unref (action);
}

static void
gedit_indented_paste_window_activatable_deactivate (GeditWindowActivatable *activatable)
{
	GeditIndentedPasteWindowActivatable *self = GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE (activatable);
	g_action_map_remove_action (G_ACTION_MAP (self->priv->window), "indented-paste");
}

static void
gedit_indented_paste_window_activatable_update_state (GeditWindowActivatable *activatable)
{
	GeditIndentedPasteWindowActivatable *self = GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE (activatable);
	GeditView *view = gedit_window_get_active_view (self->priv->window);
	gboolean enable = view != NULL && gtk_text_view_get_editable (GTK_TEXT_VIEW (view));
	GAction *action = g_action_map_lookup_action (G_ACTION_MAP (self->priv->window), "indented-paste");

	if (G_IS_SIMPLE_ACTION (action))
		g_simple_action_set_enabled (G_SIMPLE_ACTION (action), enable);
}

static void
gedit_window_activatable_iface_init (GeditWindowActivatableInterface *iface)
{
	iface->activate = gedit_indented_paste_window_activatable_activate;
	iface->deactivate = gedit_indented_paste_window_activatable_deactivate;
	iface->update_state = gedit_indented_paste_window_activatable_update_state;
}

void
gedit_indented_paste_window_activatable_register (PeasObjectModule *module)
{
	gedit_indented_paste_window_activatable_register_type (G_TYPE_MODULE (module));

	peas_object_module_register_extension_type (module,
						    GEDIT_TYPE_WINDOW_ACTIVATABLE,
						    GEDIT_TYPE_INDENTED_PASTE_WINDOW_ACTIVATABLE);
}
