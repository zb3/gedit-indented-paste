#include "gedit-indented-paste-app-activatable.h"
#include <gedit/gedit-app.h>
#include <gedit/gedit-app-activatable.h>

struct _GeditIndentedPasteAppActivatablePrivate
{
	GeditApp *app;
};

enum
{
	PROP_0,
	PROP_APP
};

static void gedit_app_activatable_iface_init (GeditAppActivatableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (GeditIndentedPasteAppActivatable,
				gedit_indented_paste_app_activatable,
				G_TYPE_OBJECT,
				0,
				G_ADD_PRIVATE_DYNAMIC (GeditIndentedPasteAppActivatable)
				G_IMPLEMENT_INTERFACE_DYNAMIC (GEDIT_TYPE_APP_ACTIVATABLE,
							       gedit_app_activatable_iface_init))

static void
gedit_indented_paste_app_activatable_get_property (GObject    *object,
						   guint       prop_id,
						   GValue     *value,
						   GParamSpec *pspec)
{
	GeditIndentedPasteAppActivatable *activatable = GEDIT_INDENTED_PASTE_APP_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_APP:
			g_value_set_object (value, activatable->priv->app);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gedit_indented_paste_app_activatable_set_property (GObject      *object,
						   guint         prop_id,
						   const GValue *value,
						   GParamSpec   *pspec)
{
	GeditIndentedPasteAppActivatable *activatable = GEDIT_INDENTED_PASTE_APP_ACTIVATABLE (object);

	switch (prop_id)
	{
		case PROP_APP:
			g_assert (activatable->priv->app == NULL);
			activatable->priv->app = GEDIT_APP (g_value_dup_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gedit_indented_paste_app_activatable_dispose (GObject *object)
{
	GeditIndentedPasteAppActivatable *activatable = GEDIT_INDENTED_PASTE_APP_ACTIVATABLE (object);

	g_clear_object (&activatable->priv->app);

	G_OBJECT_CLASS (gedit_indented_paste_app_activatable_parent_class)->dispose (object);
}

static void
gedit_indented_paste_app_activatable_finalize (GObject *object)
{

	G_OBJECT_CLASS (gedit_indented_paste_app_activatable_parent_class)->finalize (object);
}

static void
gedit_indented_paste_app_activatable_class_init (GeditIndentedPasteAppActivatableClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = gedit_indented_paste_app_activatable_get_property;
	object_class->set_property = gedit_indented_paste_app_activatable_set_property;
	object_class->dispose = gedit_indented_paste_app_activatable_dispose;
	object_class->finalize = gedit_indented_paste_app_activatable_finalize;

	g_object_class_override_property (object_class, PROP_APP, "app");
}

static void
gedit_indented_paste_app_activatable_class_finalize (GeditIndentedPasteAppActivatableClass *klass)
{
}

static void
gedit_indented_paste_app_activatable_init (GeditIndentedPasteAppActivatable *activatable)
{
	activatable->priv = gedit_indented_paste_app_activatable_get_instance_private (activatable);
}

static void
gedit_indented_paste_app_activatable_activate (GeditAppActivatable *activatable)
{
	GeditIndentedPasteAppActivatable *self = GEDIT_INDENTED_PASTE_APP_ACTIVATABLE (activatable);

	gtk_application_set_accels_for_action (GTK_APPLICATION (self->priv->app),
										   "win.indented-paste",
										   (const gchar *[]) { "<Primary><Shift>V", NULL });

}

static void
gedit_indented_paste_app_activatable_deactivate (GeditAppActivatable *activatable)
{
	GeditIndentedPasteAppActivatable *self = GEDIT_INDENTED_PASTE_APP_ACTIVATABLE (activatable);

	gtk_application_set_accels_for_action (GTK_APPLICATION (self->priv->app),
										   "win.indented-paste",
										   (const gchar *[]) { NULL });
}

static void
gedit_app_activatable_iface_init (GeditAppActivatableInterface *iface)
{
	iface->activate = gedit_indented_paste_app_activatable_activate;
	iface->deactivate = gedit_indented_paste_app_activatable_deactivate;
}

void
gedit_indented_paste_app_activatable_register (PeasObjectModule *module)
{
	gedit_indented_paste_app_activatable_register_type (G_TYPE_MODULE (module));

	peas_object_module_register_extension_type (module,
						    GEDIT_TYPE_APP_ACTIVATABLE,
						    GEDIT_TYPE_INDENTED_PASTE_APP_ACTIVATABLE);
}
