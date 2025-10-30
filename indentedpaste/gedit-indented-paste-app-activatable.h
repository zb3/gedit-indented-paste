#pragma once

#include <libpeas/peas.h>

G_BEGIN_DECLS

#define GEDIT_TYPE_INDENTED_PASTE_APP_ACTIVATABLE             (gedit_indented_paste_app_activatable_get_type ())
#define GEDIT_INDENTED_PASTE_APP_ACTIVATABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEDIT_TYPE_INDENTED_PASTE_APP_ACTIVATABLE, GeditIndentedPasteAppActivatable))
#define GEDIT_INDENTED_PASTE_APP_ACTIVATABLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GEDIT_TYPE_INDENTED_PASTE_APP_ACTIVATABLE, GeditIndentedPasteAppActivatableClass))
#define GEDIT_IS_INDENTED_PASTE_APP_ACTIVATABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEDIT_TYPE_INDENTED_PASTE_APP_ACTIVATABLE))
#define GEDIT_IS_INDENTED_PASTE_APP_ACTIVATABLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GEDIT_TYPE_INDENTED_PASTE_APP_ACTIVATABLE))
#define GEDIT_INDENTED_PASTE_APP_ACTIVATABLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GEDIT_TYPE_INDENTED_PASTE_APP_ACTIVATABLE, GeditIndentedPasteAppActivatableClass))

typedef struct _GeditIndentedPasteAppActivatable         GeditIndentedPasteAppActivatable;
typedef struct _GeditIndentedPasteAppActivatableClass    GeditIndentedPasteAppActivatableClass;
typedef struct _GeditIndentedPasteAppActivatablePrivate  GeditIndentedPasteAppActivatablePrivate;

struct _GeditIndentedPasteAppActivatable
{
	GObject parent;

	GeditIndentedPasteAppActivatablePrivate *priv;
};

struct _GeditIndentedPasteAppActivatableClass
{
	GObjectClass parent_class;
};

GType	gedit_indented_paste_app_activatable_get_type	(void);

void	gedit_indented_paste_app_activatable_register	(PeasObjectModule *module);

G_END_DECLS
