#pragma once

#include <gedit/gedit-window.h>
#include <libpeas/peas.h>

G_BEGIN_DECLS

#define GEDIT_TYPE_INDENTED_PASTE_WINDOW_ACTIVATABLE             (gedit_indented_paste_window_activatable_get_type ())
#define GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEDIT_TYPE_INDENTED_PASTE_WINDOW_ACTIVATABLE, GeditIndentedPasteWindowActivatable))
#define GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GEDIT_TYPE_INDENTED_PASTE_WINDOW_ACTIVATABLE, GeditIndentedPasteWindowActivatableClass))
#define GEDIT_IS_INDENTED_PASTE_WINDOW_ACTIVATABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEDIT_TYPE_INDENTED_PASTE_WINDOW_ACTIVATABLE))
#define GEDIT_IS_INDENTED_PASTE_WINDOW_ACTIVATABLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GEDIT_TYPE_INDENTED_PASTE_WINDOW_ACTIVATABLE))
#define GEDIT_INDENTED_PASTE_WINDOW_ACTIVATABLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GEDIT_TYPE_INDENTED_PASTE_WINDOW_ACTIVATABLE, GeditIndentedPasteWindowActivatableClass))

typedef struct _GeditIndentedPasteWindowActivatable         GeditIndentedPasteWindowActivatable;
typedef struct _GeditIndentedPasteWindowActivatableClass    GeditIndentedPasteWindowActivatableClass;
typedef struct _GeditIndentedPasteWindowActivatablePrivate  GeditIndentedPasteWindowActivatablePrivate;

struct _GeditIndentedPasteWindowActivatable
{
	GObject parent;

	GeditIndentedPasteWindowActivatablePrivate *priv;
};

struct _GeditIndentedPasteWindowActivatableClass
{
	GObjectClass parent_class;
};

GType	gedit_indented_paste_window_activatable_get_type	(void);

void	gedit_indented_paste_window_activatable_register	(PeasObjectModule *module);


G_END_DECLS
