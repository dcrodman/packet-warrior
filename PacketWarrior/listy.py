import sys

import Tkinter as tk
from tkintertable.Tables import TableCanvas
from tkintertable.TableModels import TableModel

def destroy_MyTable ():
    global w
    w.destroy()
    w = None

class MyTable(tk.Toplevel):
	
	def __init__(self, data):
		tk.Toplevel.__init__(self)
		tframe = tk.Frame(self)
		tframe.pack()
		tframe.pack_propagate(0)
		self.table = TableCanvas(tframe)
		self.table.createTableFrame()
		self.table.model = TableModel()
		self.data = data
		self.update()

	def fillTable(self):
		self.table.model.importDict(self.data)
		self.redraw()

	def redraw(self):
		self.table.redrawTable()

	def update(self):
		self.table.model.importDict(self.data)
		self.redraw()

if __name__ == "__main__":
    objName = MyTable()

        