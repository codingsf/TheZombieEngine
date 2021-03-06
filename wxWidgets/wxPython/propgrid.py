## This file reverse renames symbols in the wx package to give
## them their wx prefix again, for backwards compatibility.
##
## Generated by BuildRenamers in config.py

# This silly stuff here is so the wxPython.wx module doesn't conflict
# with the wx package.  We need to import modules from the wx package
# here, then we'll put the wxPython.wx entry back in sys.modules.
import sys
_wx = None
if sys.modules.has_key('wxPython.wx'):
    _wx = sys.modules['wxPython.wx']
    del sys.modules['wxPython.wx']

import wx.propgrid

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxPG_AUTO_SORT = wx.propgrid.PG_AUTO_SORT
wxPG_HIDE_CATEGORIES = wx.propgrid.PG_HIDE_CATEGORIES
wxPG_ALPHABETIC_MODE = wx.propgrid.PG_ALPHABETIC_MODE
wxPG_BOLD_MODIFIED = wx.propgrid.PG_BOLD_MODIFIED
wxPG_SPLITTER_AUTO_CENTER = wx.propgrid.PG_SPLITTER_AUTO_CENTER
wxPG_TOOLTIPS = wx.propgrid.PG_TOOLTIPS
wxPG_HIDE_MARGIN = wx.propgrid.PG_HIDE_MARGIN
wxPG_STATIC_SPLITTER = wx.propgrid.PG_STATIC_SPLITTER
wxPG_STATIC_LAYOUT = wx.propgrid.PG_STATIC_LAYOUT
wxPG_LIMITED_EDITING = wx.propgrid.PG_LIMITED_EDITING
wxPG_TOOLBAR = wx.propgrid.PG_TOOLBAR
wxPG_DESCRIPTION = wx.propgrid.PG_DESCRIPTION
wxPG_COMPACTOR = wx.propgrid.PG_COMPACTOR
wxPG_EX_INIT_NOCAT = wx.propgrid.PG_EX_INIT_NOCAT
wxPG_EX_NO_FLAT_TOOLBAR = wx.propgrid.PG_EX_NO_FLAT_TOOLBAR
wxPG_EX_CLASSIC_SPACING = wx.propgrid.PG_EX_CLASSIC_SPACING
wxPG_DEFAULT_STYLE = wx.propgrid.PG_DEFAULT_STYLE
wxPGMAN_DEFAULT_STYLE = wx.propgrid.PGMAN_DEFAULT_STYLE
wxPG_BOOL_USE_CHECKBOX = wx.propgrid.PG_BOOL_USE_CHECKBOX
wxPG_FLOAT_PRECISION = wx.propgrid.PG_FLOAT_PRECISION
wxPG_FILE_WILDCARD = wx.propgrid.PG_FILE_WILDCARD
wxPG_FILE_SHOW_FULL_PATH = wx.propgrid.PG_FILE_SHOW_FULL_PATH
wxPG_USER_ATTRIBUTE = wx.propgrid.PG_USER_ATTRIBUTE
wxEVT_PG_SELECTED = wx.propgrid.wxEVT_PG_SELECTED
wxEVT_PG_CHANGED = wx.propgrid.wxEVT_PG_CHANGED
wxEVT_PG_HIGHLIGHTED = wx.propgrid.wxEVT_PG_HIGHLIGHTED
wxEVT_PG_RIGHT_CLICK = wx.propgrid.wxEVT_PG_RIGHT_CLICK
wxEVT_PG_PAGE_CHANGED = wx.propgrid.wxEVT_PG_PAGE_CHANGED
wxEVT_PG_LEFT_DCLICK = wx.propgrid.wxEVT_PG_LEFT_DCLICK
wxEVT_PG_CREATECTRL = wx.propgrid.wxEVT_PG_CREATECTRL
wxEVT_PG_DESTROYCTRL = wx.propgrid.wxEVT_PG_DESTROYCTRL
wxPropertyGridEvent = wx.propgrid.PropertyGridEvent
wxPropertyGridEventPtr = wx.propgrid.PropertyGridEventPtr
wxPropertyCategory = wx.propgrid.PropertyCategory
wxStringProperty = wx.propgrid.StringProperty
wxIntProperty = wx.propgrid.IntProperty
wxFloatProperty = wx.propgrid.FloatProperty
wxBoolProperty = wx.propgrid.BoolProperty
wxLongStringProperty = wx.propgrid.LongStringProperty
wxFileProperty = wx.propgrid.FileProperty
wxDirProperty = wx.propgrid.DirProperty
wxArrayStringProperty = wx.propgrid.ArrayStringProperty
wxEnumProperty = wx.propgrid.EnumProperty
wxFontProperty = wx.propgrid.FontProperty
wxColourProperty = wx.propgrid.ColourProperty
wxStringCallBackProperty = wx.propgrid.StringCallBackProperty
wxImageFileProperty = wx.propgrid.ImageFileProperty
wxCustomProperty = wx.propgrid.CustomProperty
wxAdvImageFileProperty = wx.propgrid.AdvImageFileProperty
wxParentProperty = wx.propgrid.ParentProperty
wxFlagsProperty = wx.propgrid.FlagsProperty
wxPGProperty = wx.propgrid.PGProperty
wxPGPropertyPtr = wx.propgrid.PGPropertyPtr
wxPropertyGrid = wx.propgrid.PropertyGrid
wxPropertyGridPtr = wx.propgrid.PropertyGridPtr


