import graphics
import sys
import time
                    

points = {}                            

center = [0, 0]
scale = [2, 2]
def getPoint(win, coords):
    x = win.getWidth() // 2 + (coords[0] - center[0]) / scale[0] * (win.getWidth() // 2) 
    y = win.getHeight() // 2 + (coords[1] - center[1]) / scale[1] * (win.getHeight() // 2) 
    return graphics.Point(x, -y)

def drawPoint(win, point):
    args = point.split()
    pnt = getPoint(win, list(map(int, args[:2])))
    point = graphics.Circle(pnt, 5)
    point.setFill('red')
    if len(args) == 2:
        point.draw(win)
    else:
        points[args[2]] = [point, graphics.Text(graphics.Point(pnt.x, pnt.y - 10), args[2])]
        for obj in points[args[2]]:
            obj.draw(win)
                
def drawPolygon(win, polygon):  
    points = [list(map(int, x)) for x in zip(polygon.split()[::2], polygon.split()[1::2])]
    points.append(points[0])
    for i in range(len(points) - 1):
        graphics.Line(getPoint(win, points[i]), getPoint(win, points[i + 1])).draw(win)

def erasePoint(name):
    for obj in points[name]:
        obj.undraw()
    del points[name]        

draw = {'point' : drawPoint, 'polygon' : drawPolygon}
erase = {'point' : erasePoint}

def doCommands(win, commands):
    for cmd in commands:
        commandType, tmp, cmd = cmd.partition(' ')
        if commandType == 'center':
            center = list(map(int, cmd.split()))
            
        elif commandType == 'scale':
            scale = list(map(float, cmd.split()))

        if commandType == 'draw':
            figureType, tmp, cmd = cmd.partition(' ')
            draw[figureType](win, cmd)

        elif commandType == 'erase':
            figureType, tmp, cmd = cmd.partition(' ')
            erase[figureType](cmd)            

def main():
    steps = [[]]
    stepNum = [-1]
    
    def nextStep():
        if (stepNum[0] + 1 < len(steps)):
            stepNum[0] += 1
            print(stepNum[0])
            doCommands(win, steps[stepNum[0]])
    
    def prevStep():
        if (stepNum[0] > 0):
            for i in range(stepNum[0]):
                doCommands(win, steps[i])
            stepNum[0] -= 1

    win = graphics.GraphWin('test', 600, 600)
    win.nextStep = nextStep
    win.prevStep = prevStep
                    
    commands = [s[:-1] for s in open(sys.argv[1], 'r')]
    for cmd in commands:
        if cmd.startswith('wait'):
            steps.append([])
        else:
            steps[-1].append(cmd)
    print(steps)

    nextStep()
    win.mainloop()

if __name__ == '__main__':
    main()                