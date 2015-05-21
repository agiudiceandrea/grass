#!/usr/bin/env python

import wx

from core.gcmd import RunCommand
from core.utils import _, GuiModuleMain
from gui_core.preferences import MapsetAccess


def main():
    app = wx.App()

    dlg = MapsetAccess(parent = None)
    dlg.CenterOnScreen()

    if dlg.ShowModal() == wx.ID_OK:
        ms = dlg.GetMapsets()
        RunCommand('g.mapsets',
                   parent = None,
                   mapset = '%s' % ','.join(ms),
                   operation = 'set')

    dlg.Destroy()


if __name__ == "__main__":
    GuiModuleMain(main)
