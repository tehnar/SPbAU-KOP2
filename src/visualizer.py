import sys
import math
import matplotlib                    
matplotlib.rc('font', family='Arial')
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import matplotlib.animation as animation
import matplotlib.patches as patches

EPS = 10 ** -6

def getOptions(args):
    d = dict(tuple(x.split('=')) for x in args)
    if 'name' in d:
        name = d['name']
        del d['name']
        return name, d
    return '', d

def drawPoint(point):
    args = point.split()
    name, options = getOptions(args[2:])
    args = list(map(float, args[:2]))
    plt.annotate(name, xy=(args[0], args[1]), xycoords='data', ha = 'center',
                xytext=(0, 10), textcoords='offset points')
    if not 'color' in options:
        options['color'] = 'black'
    plt.plot(args[0], args[1], 'o', **options) 
              
def drawSegment(segment):
    args = segment.split()
    name, options = getOptions(args[4:])
    args = list(map(float, args[:4]))
    plt.plot([args[0], args[2]], [args[1], args[3]], '-', lw=1.5, **options)

def drawVector(vector):
    args = vector.split()
    name, options = getOptions(args[4:])
    args = list(map(float, args[:4]))
    plt.annotate('', xy=(args[2], args[3]), xycoords='data',
                xytext=(args[0], args[1]), textcoords='data',
                arrowprops=dict(color='blue', arrowstyle='->'))

def drawLine(line):
    args = line.split()
    name, options = getOptions(args[3:])
    args = list(map(float, args[:3]))
    plt.autoscale(False)
    if abs(args[1] - 0) < EPS:
        plt.plot([-args[2] / args[0], -args[2] / args[0]], [-10**9, 10**9], '-', lw=1.5, **options)
    else:
        plt.plot([-10**9, 10**9], [(-args[2] + 10**9 * args[0]) / args[1], 
                (-args[2] - 10**9 * args[0]) / args[1]], '-', lw=1.5, **options)

def drawRay(ray):
    args = ray.split()
    name, options = getOptions(args[4:])
    args = list(map(float, args[:4]))
    plt.autoscale(False)
    plt.plot([args[0], args[0] + 10**9 * args[2]], [args[1], args[1] + 10**9 * args[3]], '-', **options)

def drawAngle(angle):
    args = angle.split()
    coords = list(map(float, args[:6]))

    drawVector(' '.join(str(x) for x in coords[:4]))    
    drawVector(' '.join(str(x) for x in (coords[:2] + coords[4:])))    

    angle1 = math.degrees(math.atan2(coords[3] - coords[1], coords[2] - coords[0]))
    angle2 = math.degrees(math.atan2(coords[5] - coords[1], coords[4] - coords[0]))
    
    len1 = math.sqrt((coords[2] - coords[0])**2 + (coords[3] - coords[1])**2)
    len2 = math.sqrt((coords[4] - coords[0])**2 + (coords[5] - coords[1])**2)
    length = min(len1, len2) / 2

    delta = angle2 - angle1
    if delta > 360:
        delta -= 360
    if delta < 0:
        delta += 360
    if delta > 180:
        angle1, angle2 = angle2, angle1
    
    plt.gca().add_patch(patches.Arc((coords[0], coords[1]), length, length, theta1=angle1, theta2=angle2))

def getCommands(fname):
    steps = [[[]]]
    inF = open(fname, 'r', encoding='utf-8')
    commands = [s[:-1] for s in inF]
    inF.close()

    for cmd in commands:
        if cmd == 'end':
            if len(steps[-1][-1]) == 0:
                steps[-1].pop()
            steps.append([[]])
        elif cmd == 'wait':
            steps[-1].append([])
        else:
            steps[-1][-1].append(cmd)
    if len(steps[-1][-1]) == 0:
        steps[-1].pop()
        if len(steps[-1]) == 0:    
            steps.pop()
    return steps
            

class Visualizer:
    def __init__(self, commands):
        mng = plt.get_current_fig_manager()
        mng.resize(800, 800)
        self.curCommand = [0, -1]
        self.commands = commands
        self.text = None
        self.draw = {'point' : drawPoint, 'segment' : drawSegment, 'line' : drawLine, 
                     'ray' : drawRay, 'vector' : drawVector, 'angle' : drawAngle}
        self.nextStep()

    def getStepCount(self):
        cnt = 0
        for cmd in self.commands:
            cnt += len(cmd)
        return cnt
                        
    def nextStep(self, event = 0):
        clear = False
        if self.curCommand[1] == -1:
            clear = True
        if self.curCommand[1] + 1 == len(self.commands[self.curCommand[0]]):
            if self.curCommand[0] + 1 < len(self.commands):
                self.curCommand[0] += 1
                self.curCommand[1] = 0
            clear = True
        else:
            self.curCommand[1] += 1

        self.doCommands(self.commands[self.curCommand[0]][self.curCommand[1]], clear)
    
    def prevStep(self, event = 0):
        self.doCommands([], True)
        if self.curCommand[1] == 0:
            if self.curCommand[0] > 0:
                self.curCommand[0] -= 1
                self.curCommand[1] = len(self.commands[self.curCommand[0]]) - 1
        else:
            self.curCommand[1] -= 1

        for commands in self.commands[self.curCommand[0]]:
            self.doCommands(commands, False)
    

    def fixScaling(self):
        xlim = plt.gca().get_xlim()
        ylim = plt.gca().get_ylim()
        midX = (xlim[0] + xlim[1]) / 2.0
        midY = (ylim[0] + ylim[1]) / 2.0
        lenX = (midX - xlim[0]) * 1.15 
        lenY = (midY - ylim[0]) * 1.15
        plt.axis([midX - lenX, midX + lenX, midY - lenY, midY + lenY])

    def doCommands(self, commands, clear):
        if clear:
            if self.text:
                self.text.remove();
                self.text = None
            plt.delaxes(plt.gca())
            plt.axes([0.075, 0.1, 0.85, 0.85])
            plt.axis('equal')
        for cmd in commands:
            commandType, tmp, cmd = cmd.partition(' ')
            if commandType != 'print':
                self.draw[commandType](cmd)
            elif commandType == 'print':
                if self.text:
                    self.text.remove()
                cmd = cmd.replace('\\n', '\n')
                self.text = plt.text(0.01, 0.99, cmd, size = 14, transform = plt.gca().transAxes, va='top')
        #if clear:        
        #    self.fixScaling()        
        plt.draw()


def showVisualization(fname):    
    prevCoords = plt.axes([0.75, 0.02, 0.075, 0.03])
    nextCoords = plt.axes([0.85, 0.02, 0.075, 0.03])
    prevButton = Button(prevCoords, 'Prev')
    nextButton = Button(nextCoords, 'Next')   

    plt.axes([0.05, 0.1, 0.9, 0.85])
    plt.cla()
    visualizer = Visualizer(getCommands(fname))
    
    nextButton.on_clicked(visualizer.nextStep)
    prevButton.on_clicked(visualizer.prevStep)

    plt.show()

def makeSlides(picsName, fname):
    visualizer = Visualizer(getCommands(fname))
                               
    cnt = visualizer.getStepCount()
    filename, dot, extension = picsName.rpartition('.')
    for i in range(cnt):
        plt.axis('off')
        plt.savefig((filename + '{0:03d}.' + extension).format(i), bbox_inches = 0, 
                     pad_inches = 0, dpi = plt.gcf().dpi) 
        visualizer.nextStep()

if __name__ == '__main__':    
    if len(sys.argv) == 2:
        showVisualization(sys.argv[1])
                        
    elif len(sys.argv) == 4:
        if sys.argv[1] != '--slides':
            print('Usage: python3 visualizer.py [--slides picsName] inputFileName')
        else:
            makeSlides(sys.argv[2], sys.argv[3])

    else:    
        print('Usage: python3 visualizer.py [--slides picsName] inputFileName')        
