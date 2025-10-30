#include "gedit-indented-paste-plugin.h"
#include "gedit-indented-paste-app-activatable.h"
#include "gedit-indented-paste-window-activatable.h"
#include <gedit/gedit-view.h>
#include <gtk/gtk.h>
#include "gedit-indented-paste-core.h"
#include "gedit-indented-paste-view-activatable.h"

G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
	gedit_indented_paste_app_activatable_register (module);
	gedit_indented_paste_window_activatable_register (module);
	gedit_indented_paste_view_activatable_register (module);
}
