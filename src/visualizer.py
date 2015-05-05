import sys
import matplotlib.pyplot as plt
from matplotlib.widgets import Button

def getOptions(args):
    d = dict(tuple(x.split('=')) for x in args)
    if 'name' in d:
        name = d['name']
        del d['name']
        return name, d
    return '', d

def drawPoint(point):
    args = point.split()
    name, options = getOptions(args[2:]) if len(args) > 2 else ('', {})
    plt.text(float(args[0]), float(args[1]), name)
    plt.plot(float(args[0]), float(args[1]), 'o', **options) 
              
def drawSegment(segment):
    args = segment.split()
    name, options = getOptions(args[4:]) if len(args) > 4 else ('', {})
    segment = plt.plot([float(args[0]), float(args[2])], [float(args[1]), float(args[3])], '-', aa=True, lw=1.5, **options)

def drawLine(line):
    args = line.split()
    name, options = getOptions(args[3:]) if len(args) > 3 else ('', {})
    args = list(map(float, args[:3]))
    plt.autoscale(False)
    if abs(args[1] - 0) < 1e-6:
        plt.plot([-args[2] / args[0], -args[2] / args[0]], [-10**9, 10**9], '-', **options)
    else:
        plt.plot([-10**9, 10**9], [(-args[2] + 10**9 * args[0]) / args[1], (-args[2] - 10**9 * args[0]) / args[1]])

def doCommands(commands):
    plt.cla()
    plt.axes([0.1, 0.1, 0.8, 0.8])
    plt.axis('equal')

    draw = {'point' : drawPoint, 'segment' : drawSegment, 'line' : drawLine}
    for cmd in commands:
        commandType, tmp, cmd = cmd.partition(' ')
        if commandType == 'draw':
            figureType, tmp, cmd = cmd.partition(' ')
            draw[figureType](cmd)

    xlim = plt.gca().get_xlim()
    ylim = plt.gca().get_ylim()
    midX = (xlim[0] + xlim[1]) / 2.0
    midY = (ylim[0] + ylim[1]) / 2.0
    lenX = (midX - xlim[0]) * 1.15 
    lenY = (midY - ylim[0]) * 1.15
    plt.axis([midX - lenX, midX + lenX, midY - lenY, midY + lenY])
    plt.draw()
            
def main():
    
    steps = [[]]
    stepNum = [-1]
    def nextStep(event):
        if (stepNum[0] + 1 < len(steps)):
            stepNum[0] += 1
            doCommands(steps[stepNum[0]])
    
    def prevStep(event):
        if (stepNum[0] > 0):
            stepNum[0] -= 1
            doCommands(steps[stepNum[0]])

    commands = [s[:-1] for s in open(sys.argv[1], 'r')]
    for cmd in commands:
        if cmd == 'wait':
            steps.append([])
        else:
            steps[-1].append(cmd)

    prevCoords = plt.axes([0.7, 0.02, 0.075, 0.05])
    nextCoords = plt.axes([0.78, 0.02, 0.075, 0.05])
    nextStep(0)
    prevButton = Button(prevCoords, 'Prev')
    nextButton = Button(nextCoords, 'Next')   
    nextButton.on_clicked(nextStep)
    prevButton.on_clicked(prevStep)
    plt.show()

if __name__ == '__main__':
    main()                
