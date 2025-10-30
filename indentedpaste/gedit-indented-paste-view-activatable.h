#pragma once

#include <gedit/gedit-view.h>
#include <libpeas/peas.h>

G_BEGIN_DECLS

#define GEDIT_TYPE_INDENTED_PASTE_VIEW_ACTIVATABLE             (gedit_indented_paste_view_activatable_get_type ())
#define GEDIT_INDENTED_PASTE_VIEW_ACTIVATABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEDIT_TYPE_INDENTED_PASTE_VIEW_ACTIVATABLE, GeditIndentedPasteViewActivatable))
#define GEDIT_INDENTED_PASTE_VIEW_ACTIVATABLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GEDIT_TYPE_INDENTED_PASTE_VIEW_ACTIVATABLE, GeditIndentedPasteViewActivatableClass))
#define GEDIT_IS_INDENTED_PASTE_VIEW_ACTIVATABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEDIT_TYPE_INDENTED_PASTE_VIEW_ACTIVATABLE))
#define GEDIT_IS_INDENTED_PASTE_VIEW_ACTIVATABLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GEDIT_TYPE_INDENTED_PASTE_VIEW_ACTIVATABLE))
#define GEDIT_INDENTED_PASTE_VIEW_ACTIVATABLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GEDIT_TYPE_INDENTED_PASTE_VIEW_ACTIVATABLE, GeditIndentedPasteViewActivatableClass))

typedef struct _GeditIndentedPasteViewActivatable         GeditIndentedPasteViewActivatable;
typedef struct _GeditIndentedPasteViewActivatableClass    GeditIndentedPasteViewActivatableClass;
typedef struct _GeditIndentedPasteViewActivatablePrivate  GeditIndentedPasteViewActivatablePrivate;

struct _GeditIndentedPasteViewActivatable
{
	GObject parent;

	GeditIndentedPasteViewActivatablePrivate *priv;
};

struct _GeditIndentedPasteViewActivatableClass
{
	GObjectClass parent_class;
};

GType	gedit_indented_paste_view_activatable_get_type	(void);

void	gedit_indented_paste_view_activatable_register	(PeasObjectModule *module);

void gedit_indented_paste_view_activatable_do_paste (GeditView *view, gboolean primary);

G_END_DECLS
