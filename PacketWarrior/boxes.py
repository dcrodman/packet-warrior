from Tkinter import Tk, StringVar
import Tkinter as tk
try:
    import ttk
    py3 = 0
except ImportError:
    import tkinter.ttk as ttk
    py3 = 1

class Application(tk.Toplevel):

    def __init__(self, parent):
        self.parent = parent
        self.combo()

    def combo(self):
    	top = tk.Toplevel()
    	filterLabel = tk.Label(top, text="Select a filter using the up/down arrowhead keys \nor keyboard up/down arrow keys.\n Replace the value in quotes.")
        filterLabel.grid(column=0, row=0)
    	self.box_value = StringVar()
        self.box = ttk.Combobox(top, textvariable=self.box_value)
        self.box['values'] = ('host \'host\'', 'src host \'host\'', 'dst host \'host\'',
         'gateway \'host\'', 'dst net \'net\'', 'src net \'net\'', 'net \'net\'', 
         'dst port \'port\'', 'src port \'port\'', 'port \'port\'', 'ip proto \'protocol\'', 
         'ip6 proto \'protocol\'','ip broadcast', 'ip multicast', 'ip6 multicast')
        self.box.current(0)
        self.box.grid(column=0, row=1)

if __name__ == '__main__':
    root = Tk()
    app = Application(root)
    root.mainloop()