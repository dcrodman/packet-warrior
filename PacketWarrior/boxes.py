from Tkinter import Tk, StringVar
import Tkinter as tk
try:
    import ttk
    py3 = 0
except ImportError:
    import tkinter.ttk as ttk
    py3 = 1

class FilterBox(tk.Toplevel):

    def __init__(self, parent):
        self.parent = parent
        self.combo()

    def combo(self):
    	top = tk.Toplevel()
    	top.geometry('320x150+319+252')
    	deviceLabel = tk.Label(top, text='Select a filter using the up/down arrowhead keys' \
            '\nor keyboard up/down arrow keys.\n Note: Replace the value in quotes.' \
            '\nNote: Add double backslashes (\\\\) \nin front of tcp, udp, and imcp.')
        deviceLabel.grid(column=0, row=0)
    	self.box_value = StringVar()
        self.box = ttk.Combobox(top, textvariable=self.box_value)
        self.box['values'] = ('ip proto tcp', 'ip proto udp', 'ip proto imcp', 'host \'host\'', 'src host \'host\'', 
            'dst host \'host\'','gateway \'host\'', 'dst net \'net\'', 'src net \'net\'', 
            'net \'net\'', 'dst port \'port\'', 'src port \'port\'', 'port \'port\'', 
            'ip6 proto \'protocol\'','ip broadcast', 'ip multicast', 'ip6 multicast')
        self.box.current(0)
        self.box.grid(column=0, row=1)

class DeviceBox(tk.Toplevel):
    def __init__(self, parent, devices):
        self.parent = parent
        self.combo(devices)

    def combo(self, devices):
        top = tk.Toplevel()
        top.geometry('320x100+319+132')
        filterLabel = tk.Label(top, text='Select a device using the up/down arrowhead keys' \
            '\nor keyboard up/down arrow keys.')
        filterLabel.grid(column=0, row=0)
        self.box_value = StringVar()
        self.box = ttk.Combobox(top, textvariable=self.box_value)
        self.box['values'] = devices
        self.box.current(0)
        self.box.grid(column=0, row=1)

class PacketListBox(tk.Toplevel):
    def __init__(self, parent, pList):
        self.parent = parent
        self.textBox(pList)

    def textBox(self, pList):
        top = tk.Toplevel()
        top.geometry('320x600+500+118')
        packetLabel = tk.Label(top, text='Packets Captured')
        packetLabel.grid(column=0, row=0)
        self.box = tk.Text(top)
        from Tkinter import INSERT
        rowNum = 1
        for val in pList:
            pkt = str(val)
            self.box.insert(INSERT, pkt)
            self.box.insert(INSERT, "\n")
            self.box.grid(column=0, row=rowNum)
            rowNum = rowNum +1
        


if __name__ == '__main__':
    root = Tk()
    app = Application(root)
    root.mainloop()

