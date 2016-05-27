from glob import glob
import wx, serial

# ======================================================

class TestFrame(wx.Frame):
    def __init__(self):
        self.w_size = (300, 200)
        wx.Frame.__init__(self, None, -1, 'Testing OpenFeeder', size=self.w_size)
        self.Show(True)
        self.Center()
        self.panel = wx.Panel(self, pos=(0,0), size=self.w_size)
        self.panel.SetBackgroundColour('#000000')
        self.btn_test = wx.Button(self.panel, -1, label='Feed', pos=(50,70), size=(200, -1))
        self.btn_test.Bind(wx.EVT_LEFT_UP, self.onTest)
    
        ### connect with Arduino
        arduino_scan_str = "/dev/cu.usbmodem*"
        self.arduino_name = ""
        for aConn in self.serial_scan(arduino_scan_str):
            self.arduino_name = aConn.name
            self.aConn = aConn
        if self.arduino_name == "": print "Arduino is not connected."
        else: print str(self.arduino_name) + " connected."

    # --------------------------------------------------

    def try_open(self, port):
        ''' function for Arduino-chip connection '''
        try:
            port = serial.Serial(port, 9600, timeout = 0)
        except serial.SerialException:
            return None
        else:
            return port

    # --------------------------------------------------

    def serial_scan(self, arduino_scan_str):
        ''' try to connect to Arduino-chip '''
        for fn in glob(arduino_scan_str):
            port = self.try_open(fn)
            if port is not None:
                yield port

    # --------------------------------------------------

    def onTest(self, event):
        ''' function for sending a feed signal to ARDUINO '''
        if self.arduino_name != "": # If the ARDUINO-chip is connected
            self.aConn.write('feed') # send a signal to Arduino

# ======================================================

class TestApp(wx.App):
    def OnInit(self):
        self.frame = TestFrame()
        self.frame.Show()
        self.SetTopWindow(self.frame)
        return True

# ======================================================

if __name__ == '__main__':
    app = TestApp(redirect = False)
    app.MainLoop()
