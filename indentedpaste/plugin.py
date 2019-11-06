# -*- coding: utf8 -*-

import gi
gi.require_version('Gedit', '3.0')
gi.require_version('Gtk', '3.0')

from gi.repository import GObject, Gio, Gtk, Gdk, Gedit

from .core import do_indented_paste

class IndentedPasteAppActivatable(GObject.Object, Gedit.AppActivatable):
    app = GObject.Property(type=Gedit.App)

    def __init__(self):
        GObject.Object.__init__(self)

    def do_activate(self):
        self.app.add_accelerator("<Primary><Shift>V", "win.indented-paste", None)

    def do_deactivate(self):
        self.app.remove_accelerator("win.indented-paste", None)


class IndentedPasteWindowActivatable(GObject.Object, Gedit.WindowActivatable):
    window = GObject.Property(type=Gedit.Window)

    def __init__(self):
        GObject.Object.__init__(self)

    def do_activate(self):
        action = Gio.SimpleAction(name="indented-paste")
        action.connect('activate', lambda a, p: self.indented_paste())
        self.window.add_action(action)

    def do_deactivate(self):
        self.window.remove_action("indented-paste")

    def do_update_state(self):
        view = self.window.get_active_view()
        enable = view is not None and view.get_editable()
        self.window.lookup_action("indented-paste").set_enabled(enable)

    def indented_paste(self):
        view = self.window.get_active_view()
        if view and hasattr(view, "indented_paste_view_activatable"):
            view.indented_paste_view_activatable.indented_paste()

class IndentedPasteViewActivatable(GObject.Object, Gedit.ViewActivatable):
    view = GObject.Property(type=Gedit.View)

    def __init__(self):
        GObject.Object.__init__(self)

    def do_activate(self):
        self.view.indented_paste_view_activatable = self
        self._handlers = [
            self.view.connect('button-press-event', self.on_button_event),
        ]

    def do_deactivate(self):
        for handler in self._handlers:
            self.view.disconnect(handler)

    def do_deactivate(self):
        delattr(self.view, "indented_paste_view_activatable")

    def on_clipboard_text(self, clipboard, text, view):
        do_indented_paste(view, text)

    def indented_paste(self, primary=False):
        buff = self.view.get_buffer()
        if buff is None:
            return

        clipboard_type = Gdk.SELECTION_CLIPBOARD if not primary else Gdk.SELECTION_PRIMARY
        clipboard = Gtk.Clipboard.get_for_display(self.view.get_display(), clipboard_type)
        clipboard.request_text(self.on_clipboard_text, self.view)

    def on_button_event(self, view, event):
        modifiers = event.state & Gtk.accelerator_get_default_mod_mask()

        if modifiers == Gdk.ModifierType.SHIFT_MASK and event.button == 2:
            self.indented_paste(primary=True)
            return True
    
        return False

