import pygame
import math
import sys

pygame.init()
screenWidth = 900
screenHeight = 900
gameDisplay = pygame.display.set_mode((screenWidth,screenHeight))
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
LineColour = (50, 110, 180)
BackGroundColour = (100, 150, 210)
array = []
boardSize = 3
for i in range(0, boardSize):
    array.append([])
    for j in range(0, boardSize):
        array[i].append(0)
paddingX = 200
paddingY = 200
totalWidth = 500
totalHeight = 500
lineWidth = 20
lineHeight = 20

headerHeight = 80
footerHeight = 50

spacingX = (totalWidth-2*lineWidth)/3
spacingY = (totalHeight-2*lineHeight)/3
mouseX = 0
mouseY = 0
turn = 0
mouseLeft = False
lastMouseLeft = False
running = True
#GAMESTATE:
#0: start menu
#1: game
#2: game over
gameState = 0
imageX = pygame.image.load('x.png')
imageO = pygame.image.load('o.png')
class button:
    mainImage = 0
    altImage = 0
    hoveredOver = False
    x = 0
    y = 0
    width = 0
    height = 0
    active = True
    def __init__(self, _x, _y, _width, _height, _mainPath, _altPath):
        self.x = _x
        self.y = _y
        self.width = _width
        self.height = _height
        self.loadImages(_mainPath, _altPath)
    def loadImages(self, mainPath, altPath):
        self.mainImage = pygame.image.load(mainPath)
        self.altImage = pygame.image.load(altPath)
    def run(self):
        print(self.x)
    def draw(self):
        if self.active:
            if self.hoveredOver:
                gameDisplay.blit(self.altImage, (self.x,self.y))
            else:
                gameDisplay.blit(self.mainImage, (self.x,self.y))
    def checkForMouse(self):
        if self.active:
            global mouseX
            global mouseY
            global mouseLeft
            global lastMouseLeft
            if mouseX > self.x and mouseX < self.x + self.width and mouseY > self.y and mouseY < self.y + self.height:
                if mouseLeft and lastMouseLeft == False:
                    self.run()
                else:
                    self.hoveredOver = True
            else:
                self.hoveredOver = False
class pvpButton(button):
    def __init__(self, _x, _y):
        button.__init__(self, _x, _y, 400, 100, "pvpButtonMain.png", "pvpButtonAlt.png")
    def run(self):
        global gameState
        gameState = 1
        self.active = False
class pvcButton(button):
    def __init__(self, _x, _y):
        button.__init__(self, _x, _y, 400, 100, "pvcButtonMain.png", "pvcButtonAlt.png")
    def run(self):
        global gameState
        gameState = 1
        self.active = False
class exitButton(button):
    def __init__(self, _x, _y):
        button.__init__(self, _x, _y, 400, 100, "exitButtonMain.png", "exitButtonAlt.png")
    def run(self):
        global running
        running = False
        self.active = False
def drawRect(x, y, width, height, color):
    pygame.draw.rect(gameDisplay, color, [x, y,width ,height])
def testForResult():
    for i in range(0,boardSize):
        for j in range(0,boardSize):
            if i < boardSize - 2:
                if array[i][j] == array[i+1][j] == array[i+2][j] != 0:
                    print "WINNER"
                    if j < boardSize - 2:
                        if array[i][j] == array[i+1][j+1] == array[i+2][j+2] != 0:
                            print "WINNER"
            if j < boardSize - 2:
                if array[i][j] == array[i][j+1] == array[i][j+2] != 0:
                    print "WINNER"
            if j < boardSize - 2 and i > 1:
                if array[i][j] == array[i-1][j+1] == array[i-2][j+2] != 0:
                    print "WINNER"
def updateGame():
    global lastMouseLeft
    global mouseLeft
    global turn
    global gameState
    global RED
    global mouseX
    global mouseY
    if mouseLeft == True and lastMouseLeft == False:
        roundedX = int(math.floor((mouseX-paddingX)/(lineWidth+spacingX)))
        roundedY = int(math.floor((mouseY-paddingY)/(lineHeight+spacingY)))
        if roundedX > -1 and roundedX < 3 and roundedY > -1 and roundedY < 3:
            if array[roundedX][roundedY] == 0:
                if turn == 0:
                    array[roundedX][roundedY] = 1
                    turn = 1
                elif turn == 1:
                    array[roundedX][roundedY] = 2
                    turn = 0
def drawGame():
    drawRect(0,0,screenWidth,screenHeight, BackGroundColour)

    drawRect(0,0,screenWidth,headerHeight, LineColour)
    drawRect(0,screenHeight - footerHeight,screenWidth,footerHeight, LineColour)

    drawRect(paddingX+spacingX, paddingY, lineWidth, totalHeight, LineColour)
    drawRect(paddingX + 2*spacingX + lineWidth, paddingY, lineWidth, totalHeight, LineColour)
    drawRect(paddingX,paddingY + spacingY,totalWidth,lineHeight, LineColour)
    drawRect(paddingX,paddingY + 2*spacingY + lineHeight,totalWidth,lineHeight, LineColour)
    for i in range(0, 3):
        for j in range(0, 3):
            if array[i][j] ==1:
                gameDisplay.blit(imageX, (paddingX + 40 + i*(lineWidth+spacingX),paddingY + 40 + j*(lineHeight+spacingY)))
            elif array[i][j] == 2:
                gameDisplay.blit(imageO, (paddingX + 40 + i*(lineWidth+spacingX),paddingY + 40 + j*(lineHeight+spacingY)))
def wipeArray():
     for i in range(0, 3):
        for j in range(0, 3):
            array[i][j] = 0
def checkMouse():
    global lastMouseLeft
    global mouseLeft
    global mouseX
    global mouseY
    global running
    lastMouseLeft = mouseLeft
    for event in pygame.event.get():
        (mouseX, mouseY) = pygame.mouse.get_pos()
        if event.type == pygame.MOUSEBUTTONDOWN:
            mouseLeft = True
            (mouseX, mouseY) = pygame.mouse.get_pos()
            print mouseX, mouseY
        else:
            (mouseX, mouseY) = pygame.mouse.get_pos()
            mouseLeft = False
        if event.type == pygame.QUIT:
            running = False
def drawMenu():
    drawRect(0,0,screenWidth,screenHeight, BackGroundColour)
    drawRect(0,0,screenWidth,headerHeight, LineColour)
    drawRect(0,screenHeight - footerHeight,screenWidth,footerHeight, LineColour)
class buttonWrapper:
    buttons = []
    def addButton(self, _type, _x, _y):
        if _type == "pvp":
            self.buttons.append(pvpButton(_x, _y))
        elif _type == "pvc":
            self.buttons.append(pvcButton(_x, _y))
        elif _type == "exit":
            self.buttons.append(exitButton(_x, _y))
    def drawButtons(self):
        for i in range(0, len(self.buttons)):
            self.buttons[i].draw()
    def updateButtons(self):
        for i in range(0, len(self.buttons)):
            self.buttons[i].checkForMouse()
    def clear():
        self.buttons = []
buttonCollection = buttonWrapper()
buttonCollection.addButton("pvp", 100, 100)
buttonCollection.addButton("pvc", 100, 300)
buttonCollection.addButton("exit", 100, 500)
while running:
    checkMouse()
    buttonCollection.updateButtons()
    if gameState == 0:
        drawMenu()
    elif gameState == 1:
        updateGame()
        drawGame()
    buttonCollection.drawButtons()
    testForResult()
    pygame.display.update()
pygame.display.quit()
pygame.quit()
sys.exit()