import math
import platform
import sys
import tkinter as tk                    

class MainWindow(tk.Canvas):
    def __init__(self, title="test",
                 width=600, height=600):
       
        master = tk.Tk()
        master.protocol("WM_DELETE_WINDOW", self.close)
        tk.Canvas.__init__(self, master, width=width, height=height)
        self.master.title(title)
        self.pack()
        master.resizable(0,0)
        self.foreground = "black"
        self.items = []
        self.mouseX = None
        self.mouseY = None
        self.height = height
        self.width = width
        self._mouseCallback = None
        master.lift()

        self.items = []
     
        self.buttons = tk.Frame(master, width=100)
        self.buttons.pack(side='bottom')
        self.buttonPrev = tk.Button(self.buttons)
        self.buttonPrev['text'] = '<'
        self.buttonPrev.pack(side='left')
        self.buttonNext = tk.Button(self.buttons)
        self.buttonNext['text'] = '>'
        self.buttonNext.pack(side='right')
        self.buttonNext.bind("<Button-1>", self.__nextStep)
        self.buttonPrev.bind("<Button-1>", self.__prevStep)

        
        if platform.system() == 'Windows':
            self.scaleFactor = 360
        else:
            self.scaleFactor = 3
        self.leftUpper = [-7, 7]
        self.scale = [15, 15]
        #self.scale = [self.width, self.height]
        self.bind('<B1-Motion>', self.onDrag)
        self.bind_all('<MouseWheel>', self.onMouseWheel)
        self.bind_all("<Button-4>", self._wheelUp)
        self.bind_all("<Button-5>", self._wheelDown)              
        self.bind('<Motion>', self.onMouseMotion)

    def close(self):
        self.master.destroy()

    def addItem(self, item):
        self.items.append(item)

    def removeItem(self, item):
        self.items.remove(item)

    def clear(self):
        for item in self.items[:]:
            item.undraw()
        self.items.clear() 

    def redraw(self):
        for item in self.items[:]:
            item.undraw()
            item.draw(self)
        self.update()

    def transformX(self, x):
        return float(x - self.leftUpper[0]) / self.scale[0] * self.width

    def transformY(self, y):
        return -float(y - self.leftUpper[1]) / self.scale[1] * self.height

    def __nextStep(self, event):
        self.nextStep(self)

    def __prevStep(self, event):
        self.prevStep(self)

    def onMouseMotion(self, event):
        self.mouseX, self.mouseY = event.x, event.y

    def onDrag(self, event):
        if not self.mouseX:
            self.mouseX = event.x
            self.mouseY = event.y
        self.leftUpper[0] -= float(event.x - self.mouseX) / self.width * self.scale[0]
        self.leftUpper[1] += float(event.y - self.mouseY) / self.height * self.scale[1]
        self.mouseX = event.x
        self.mouseY = event.y
        self.redraw()

    def _wheelUp(self, event):
        event.delta = 1
        self.onMouseWheel(event)

    def _wheelDown(self, event):
        event.delta = -1
        self.onMouseWheel(event)

    def onMouseWheel(self, event):
        if event.delta > 0:
            self.scale[0] /= event.delta / self.scaleFactor + 1
            self.scale[1] /= event.delta / self.scaleFactor + 1
            self.leftUpper[0] /= event.delta / self.scaleFactor + 1 
            self.leftUpper[1] /= event.delta / self.scaleFactor + 1
        else:
            self.scale[0] *= -event.delta / self.scaleFactor + 1
            self.scale[1] *= -event.delta / self.scaleFactor + 1
            self.leftUpper[0] *= -event.delta / self.scaleFactor + 1 
            self.leftUpper[1] *= -event.delta / self.scaleFactor + 1
        self.redraw()


class Shape:
    def __init__(self):
       self.canvas = None 

    def draw(self, win):
        if not self.canvas:
            win.addItem(self)
            self.canvas = win
            self.id, self.labelId = self._draw(win)
            win.update()

    def _draw(self, win):
        pass

    def undraw(self):
        if self.canvas:
            self.canvas.removeItem(self)
            self.canvas.delete(self.id)
            self.canvas.delete(self.labelId)
            self.canvas.update()
            self.canvas = None


class Point(Shape):
    def __init__(self, x, y, name = ''):
        Shape.__init__(self)
        self.x = x
        self.y = y
        self.name = name

    def _draw(self, win):
        cx = win.transformX(self.x)
        cy = win.transformY(self.y)
        id = win.create_oval(cx - 5, cy - 5, cx + 5, cy + 5, fill = 'red')
        labelId = None
        if self.name:
            labelId = win.create_text(cx, cy - 15, text = self.name)
        return id, labelId

class Segment(Shape):
    def __init__(self, x1, y1, x2, y2, name = ''):
        Shape.__init__(self)
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2
        self.name = name

    def _drawLabel(self, win):
        id = None
        if self.name:
            id = win.create_text((x1 + x2) / 2, (y1 + y2) / 2, 
                    text = self.name, angle=-180.0 / math.pi * math.atan((y2 - y1) / (x2 - x1)))
        return id
  
    def _draw(self, win):
        x1 = win.transformX(self.x1)
        y1 = win.transformY(self.y1)
        x2 = win.transformX(self.x2)
        y2 = win.transformY(self.y2)
        id = win.create_line(x1, y1, x2, y2, width = 2)
        return id, self._drawLabel(win)

class Line(Segment):
    def __init__(self, a, b, c, name = ''):
        norm = math.sqrt(a * a + b * b)
        a /= norm
        b /= norm
        Segment.__init__(self, -10**4 * a, (-c + 10**4 * a) / b, 10**4 * a, (-c - 10**4 * a) / b, name)

class Ray(Segment):
    def __init__(self, x, y, dirX, dirY, name = ''):
        norm = math.sqrt(dirX**2 + dirY**2)
        self.dirX, self.dirY = dirX / norm, dirY / norm
        Segment.__init__(self, x, y, 10**4 * dirX + x, 10**4 * dirY + y, name)

    def _drawLabel(self, win):
        x = win.transformX(self.x1)
        y = win.transformY(self.y1)
        return win.create_text(x, y, text = self.name, anchor = tk.SW, angle = -180 / math.pi * math.atan(self.dirX / self.dirY))

def drawPoint(win, point):
    args = point.split()
    name = ''
    if len(args) == 3:
        name = args[2]
    Point(int(args[0]), int(args[1]), name).draw(win)
                
def drawPolygon(win, polygon):  
    points = [list(map(int, x)) for x in zip(polygon.split()[::2], polygon.split()[1::2])]
    points.append(points[0])
    for i in range(len(points) - 1):
        Segment(points[i][0], points[i][1], points[i + 1][0], points[i + 1][1]).draw(win)

def drawSegment(win, segment):
    args = segment.split()
    Segment(int(args[0]), int(args[1]), int(args[2]), int(args[3])).draw(win)
def doCommands(win, commands):
    draw = {'point' : drawPoint, 'segment' : drawSegment, 'polygon' : drawPolygon}
    for cmd in commands:
        commandType, tmp, cmd = cmd.partition(' ')
        if commandType == 'draw':
            figureType, tmp, cmd = cmd.partition(' ')
            draw[figureType](win, cmd)

def main():
    
    steps = [[]]
    stepNum = [-1]
    
    def nextStep(win):
        if (stepNum[0] + 1 < len(steps)):
            win.clear()
            stepNum[0] += 1
            doCommands(win, steps[stepNum[0]])
    
    def prevStep(win):
        if (stepNum[0] > 0):
            win.clear()
            stepNum[0] -= 1
            doCommands(win, steps[stepNum[0]])

    win = MainWindow('test', 600, 600)
    win.nextStep = nextStep
    win.prevStep = prevStep
                    
    commands = [s[:-1] for s in open(sys.argv[1], 'r')]
    for cmd in commands:
        if cmd == 'wait':
            steps.append([])
        else:
            steps[-1].append(cmd)

    nextStep(win)
    win.mainloop()

if __name__ == '__main__':
    main()                
