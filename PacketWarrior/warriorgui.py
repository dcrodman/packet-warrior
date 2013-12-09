#! /usr/bin/env python
#
# Partially enerated by PAGE version 4.1
# In conjuction with Tcl version 8.6
#    Nov. 28, 2013 06:55:56 PM
# Icons from Open Icon Library 
import sys, os

IMG_DIR = os.path.join(
       os.path.dirname(os.path.abspath(__file__)), 'img/')

try:
    from Tkinter import *
except ImportError:
    from tkinter import *
try:
    import ttk
    py3 = 0
except ImportError:
    import tkinter.ttk as ttk
    py3 = 1
import boxes, tkMessageBox, time
import tkMessageBox
import pcap_ext

def image_path(imgFile):
    return os.path.join(IMG_DIR, imgFile)

def vp_start_gui():
    '''Starting point when module is the main routine.'''
    global val, w, root
    root = Tk()
    root.title('PacketWarrior')
    root.geometry('400x35+319+74')
    root.configure(bg="#F1F1F1")
    set_Tk_var()
    w = PacketWarrior (root)
    init()
    root.mainloop()

def create_PacketWarrior (root):
    '''Starting point when module is imported by another program.'''
    global w, w_win
    capturing = None
    if w: # So we have only one instance of window.
        return
    w = Toplevel (root)
    w.title('PacketWarrior')
    w.geometry('400x35+319+74')
    root.configure(bg="#F1F1F1")
    set_Tk_var()
    w_win = PacketWarrior (w)
    init()
    return w_win

def destroy_PacketWarrior ():
    global w
    w.destroy()
    w = None


def set_Tk_var():
    # These are Tk variables used passed to Tkinter and must be
    # defined before the widgets using them are created.
    global NewCheck
    NewCheck = StringVar()


def init():
    pass

class PacketWarrior(Frame):
    pktEngine = pcap_ext.PacketEngine()
    packetList = []

    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.grid(ipady=0)
        self.packetfile = "temp.txt"

        for r in range(1):
            self.master.rowconfigure(r, weight=1)    
        for c in range(11):
            self.master.columnconfigure(c, weight=0)

        _bgcolor = 'systemWindowBody'  # X11 color: #ffffff
        _fgcolor = '#000000'  # X11 color: 'black'
        _compcolor = 'systemWindowBody' # X11 color: #ffffff
        _ana1color = 'systemWindowBody' # X11 color: #ffffff 
        _ana2color = 'systemWindowBody' # X11 color: #ffffff 
        #self.style = ttk.Style()
        if sys.platform == "win32":
            self.style.theme_use('winnative')

        self.menubar = Menu(master,bg=_bgcolor,fg=_fgcolor)
        master.configure(menu = self.menubar)

        self.packetwarrior = Menu(master,tearoff=0)
        self.menubar.add_cascade(menu=self.packetwarrior,
                activebackground="systemWindowBody",
                activeforeground="#111111",
                background="systemWindowBody",
                foreground="#000000",
                label="PacketWarrior")
        self.packetwarrior.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.about,
                foreground="#000000",
                label="About")
        self.packetwarrior.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.open_help,
                foreground="#000000",
                label="Help")
        self.packetwarrior.add_separator(
                background="systemWindowBody")
        self.packetwarrior.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.exit_pw,
                foreground="#000000",
                label="Exit")
        self.file = Menu(master,tearoff=0)
        self.menubar.add_cascade(menu=self.file,
                activebackground="systemWindowBody",
                activeforeground="#111111",
                background="systemWindowBody",
                foreground="#000000",
                label="File")
        self.file.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.open_file,
                foreground="#000000",
                label="Open")
        self.file.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.save_file_as,
                foreground="#000000",
                label="Save As...")
        self.file.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.close_file,
                foreground="#000000",
                label="Close")
        self.view = Menu(master,tearoff=0)
        self.menubar.add_cascade(menu=self.view,
                activebackground="systemWindowBody",
                activeforeground="#111111",
                background="systemWindowBody",
                foreground="#000000",
                label="View")
        self.view.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.show_packet_list,
                foreground="#000000",
                label="Packet List")
        self.capture = Menu(master,tearoff=0)
        self.menubar.add_cascade(menu=self.capture,
                activebackground="systemWindowBody",
                activeforeground="#111111",
                background="systemWindowBody",
                foreground="#000000",
                label="Capture")
        self.capture.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.get_devices,
                foreground="#000000",
                label="Set Device")
        self.capture.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.set_filters,
                foreground="#000000",
                label="Filters")
        self.capture.add_separator(
                background="systemWindowBody")
        self.capture.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.start_capture,
                foreground="#000000",
                label="Start Capture")
        self.capture.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.stop_capture,
                foreground="#000000",
                label="Stop Capture")
        self.capture.add_command(
                activebackground="systemWindowBody",
                activeforeground="#000000",
                background="systemWindowBody",
                command=self.reset_session,
                foreground="#000000",
                label="Reset Session")
    

        # Toolbar set up

        self._img1 = PhotoImage(file=image_path("open.gif"))
        self.OpenButton = Button(master, image=self._img1, command=self.open_file, border=0)
        self.OpenButton.grid(row=0, column=0)

        self._img2 = PhotoImage(file=image_path("saveAS.gif"))
        self.AsButton = Button(master, image=self._img2, command=self.save_file_as, bd=-2)
        self.AsButton.grid(row=0, column=1)

        self._img3 = PhotoImage(file=image_path("close.gif"))
        self.CloseButton = Button(master, image=self._img3, command=self.close_file, bd=-2)
        self.CloseButton.grid(row=0, column=2)

        self._img4 = PhotoImage(file=image_path("device.gif"))
        self.DeviceButton = Button(master, image=self._img4, command=self.get_devices, bd=-2)
        self.DeviceButton.grid(row=0, column=3)

        self._img5 = PhotoImage(file=image_path("filter.gif"))
        self.FilterButton = Button(master, image=self._img5, command=self.set_filters, bd=-2)
        self.FilterButton.grid(row=0, column=4)

        self._img6 = PhotoImage(file=image_path("play.gif"))
        self.StartButton = Button(master, image=self._img6, command=self.start_capture, bd=-2)
        self.StartButton.grid(row=0, column=5)

        self._img7 = PhotoImage(file=image_path("stop.gif"))
        self.StopButton = Button(master, image=self._img7, command=self.stop_capture, bd=-2)
        self.StopButton.grid(row=0, column=6)

        self._img8 = PhotoImage(file=image_path("help.gif"))
        self.HelpButton = Button(master, image=self._img8, command=self.open_help, bd=-2)
        self.HelpButton.grid(row=0, column=7)

        self._img9 = PhotoImage(file=image_path("exit.gif"))
        self.ExitButton = Button(master, image=self._img9, command=self.exit_pw, bd=-2)
        self.ExitButton.grid(row=0, column=8)

        self.PktLstButton = Button(master, command=self.show_packet_list, bd=-2, text="PacketList")
        self.PktLstButton.grid(row=0, column=9)

        # End Toolbar

    def show_packet_list(self):
        self.buildList()
        platform = sys.platform
        if platform == "darwin":
            os.system("open -a TextEdit %s" % self.packetfile) 
        elif platform == "linux":
            os.system("gedit %s" % self.packetfile)
        elif platform == 'win32':
            os.system("notepad %s" % self.packetfile)

    def about(self):
        tkMessageBox.showinfo("About PacketWarrior", "PacketWarrior\n\nVersion 0.1\n\nCopyright 2013\nDrew Rodman and Jonathan Loy")        

    def close_file(self):
        tkMessageBox.showinfo("Close File", "The Close File functionality not yet implemented.")

    def exit_pw(self):
        sys.exit()

    def open_file(self):
        platform = sys.platform
        import tkFileDialog,csv
        inputfile = tkFileDialog.askopenfilename(parent=root,title='Choose a file')
        if inputfile:
            if platform == "darwin":
                command = "open -a TextEdit %s" % inputfile
            elif platform == "linux":
                command = "gedit %s" % inputfile
            elif platform == 'win32':
                command =  "notepad %s" % inputfile
            os.system(command)
        

    def open_help(self):
        tkMessageBox.showinfo("Help", "The Help functionality not yet implemented.")

    def save_file_as(self):
        import tkFileDialog, shutil
        fout = tkFileDialog.asksaveasfilename(defaultextension='.txt')
        if fout:
            if self.packetfile != fout:
                shutil.copyfile(self.packetfile, fout)
                self.packetfile = fout

    def get_devices(self):
        devices = self.pktEngine.getAvailableDevices()
        self.deviceBox = boxes.DeviceBox(self, devices)
        self.deviceBox.box.bind('<Return>', self.set_device)

    def set_device(self, other):
        deviceString = self.deviceBox.box_value.get()
        deviceString = deviceString.translate(None, '\'')
        deviceString = str(deviceString)
        result = self.pktEngine.selectDevice(deviceString)   
        if result is None:
            tkMessageBox.showinfo("Device", "%s has been set as the selected device." % deviceString)


    def set_filters(self):
        self.filterBox = boxes.FilterBox(self)
        self.filterBox.box.bind('<Return>', self.update_filter)
    
    def update_filter(self, other):
        filterString = self.filterBox.box_value.get()
        filterString =  filterString.translate(None, '\'')
        result = self.pktEngine.setFilter(filterString)
        if result is False:
            tkMessageBox.showinfo("Filter", "%s has been set as a filter." % filterString)

    def start_capture(self):
        result = self.pktEngine.startCapture()
        self.capture_packet()

    def capture_packet(self):
        self.end_capture = 0
        self.packetList.append(self.pktEngine.getNextPacket())
        pkt = self.packetList[-1].payload()
        self.packetList.append(pkt)
        self.after(1, self.capture_more)

    def capture_more(self):
        if self.end_capture:
            return
        self.packetList.append(self.pktEngine.getNextPacket())
        pkt = self.packetList[-1].payload()
        self.packetList.append(pkt)
        self.after(1, self.capture_more)

    def stop_capture(self):
        self.end_capture = 1
        self.pktEngine.endCapture()

    def reset_session(self):
        self.pktEngine.resetSession()
        self.save_file()

    def buildList(self):
        w = open(self.packetfile, "w")
        i =0 
        for val in self.packetList:
            val = str(val)
            w.write(val)
            if (i % 2) != 0:
                w.write('\n\n')
            i = i +1
            

    
        

if __name__ == '__main__':
    vp_start_gui()