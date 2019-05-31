import pygame
import math
import sys
import copy
import random

pygame.init()
#set colours for use by functions that draw
DarkerLineColour = (20, 70, 130)
LineColour = (50, 110, 180)
BackGroundColour = (100, 150, 210)
DarkerBackGroundColour = (90, 140, 200)
#setting the screen width and height
screenWidth = 900
screenHeight = 900
#setting the screen width and height for the actually window with pygame
gameDisplay = pygame.display.set_mode((screenWidth,screenHeight))
#setting the window's icon
pygame.display.set_icon(pygame.image.load("icon.png"))
#setting the title of the window
pygame.display.set_caption('Tic Tac Toe')
# a simple draw rectangle function, easier to use than pygames draw.rect function in my opinion
def drawRect(x, y, width, height, color):
    pygame.draw.rect(gameDisplay, color, [x, y,width ,height])
###GUI ELEMENTS:-----------
#class for the incrememtal clicker, only used to change the number of games played in the best-of mode,
class IncrementalClicker:
    x = 0
    y = 0
    value = 0
    maxValue = 0
    minValue = 0
    increment = 0
    buttonWidth = 40
    buttonHeight = 50
    #sometimes GUI elements can be invisible, This means they aren't drawn and are unable to be interacted with
    visible = True
    #for all classes with the __init__() method, it is run when the class is first instantiated as an object
    def __init__(self, _x, _y, _minValue, _maxValue, _increment):
        #this code is in lots of GUI element classes, just allows you to have "centered" as a parameter instead of manually setting the position to the center of the screen
        if _x == "centered":
            self.x  = screenWidth/2 - 68
        else:
            self.x = _x
        if _y == "centered":
            self.y  = screenHeight/2 - 62
        else:
            self.y = _y
        self.value = _minValue
        self.minValue = _minValue
        self.maxValue = _maxValue
        self.increment = _increment
    #draw method, all GUI elements have this method, it draws the element
    def draw(self):
        if self.visible:
            drawRect(self.x, self.y, self.buttonWidth, self.buttonHeight, DarkerLineColour)
            drawRect(self.x, self.y+self.buttonHeight + 2, self.buttonWidth, self.buttonHeight, DarkerLineColour)
            drawRect(self.x + self.buttonWidth + 2,self.y, self.buttonWidth*2, self.buttonHeight*2 + 2, DarkerLineColour)
            drawRect(self.x + self.buttonWidth + 6,self.y+4, self.buttonWidth*2 - 8, self.buttonHeight*2 + 2 - 8, DarkerBackGroundColour)
            game.drawText(str(self.value), self.x + self.buttonWidth*2 + 2, self.y + self.buttonHeight, 55, DarkerLineColour)
    #method for detecting if the mouse is interacting with the object
    def checkForMouse(self):
        if self.visible:
            if game.mouseLeft and game.lastMouseLeft == False:
                if game.mouseX > self.x and game.mouseX < self.x + self.buttonWidth and game.mouseY > self.y and game.mouseY < self.y + self.buttonHeight and self.value + self.increment <= self.maxValue:
                    self.value += self.increment
                elif game.mouseX > self.x and game.mouseX < self.x + self.buttonWidth and game.mouseY > self.y+self.buttonHeight + 2 and game.mouseY < self.y + self.buttonHeight*2 + 2 and self.value - self.increment >= self.minValue:
                    self.value -= self.increment
#class for the text box seen on the setup screen
class TextBox:
    text = ""
    x = 0
    y = 0
    width = 610
    height = 80
    #when the textbox is in focus, much like how windows in OSX, linux and Windows
    inFocus = False
    #name is the text displayed when the textbox as no text and is not in focus
    name = "DefaultName"
    def __init__(self, _x, _y, _name):
        if _x == "centered":
            self.x  = screenWidth/2 - self.width/2
        else:
            self.x = _x
        if _y == "centered":
            self.y  = screenHeight/2 - self.height/2
        else:
            self.y = _y
        self.name = _name
    def draw(self):

        drawRect(self.x, self.y, self.width, self.height, LineColour)
        if self.inFocus:
            drawRect(self.x+4, self.y+4, self.width-8, self.height-8, DarkerBackGroundColour)
        else:
            drawRect(self.x+4, self.y+4, self.width-8, self.height-8, BackGroundColour)
        if self.text == "" and self.inFocus == False:
            game.drawText(self.name, self.x + self.width/2, self.y + self.height/2, 55, LineColour)
        else:
            game.drawText(self.text, self.x + self.width/2, self.y + self.height/2, 55, DarkerLineColour)
    def checkForMouse(self):
        if game.mouseLeft and game.lastMouseLeft == False:
            if game.mouseX > self.x and game.mouseX < self.x + self.width and game.mouseY > self.y and game.mouseY < self.y + self.height:
                self.inFocus = True
            else:
                self.inFocus = False
        elif game.mouseX > self.x and game.mouseX < self.x + self.width and game.mouseY > self.y and game.mouseY < self.y + self.height:
            self.hoveredOver = False
        if self.inFocus:
            if game.keyDownString != game.lastKeyDownString:
                fontTest = pygame.font.Font('Roboto-thin.ttf',55)
                textWidth, textHeight = fontTest.size(self.text)
                if game.keyDownString == "BACKSPACE":
                    self.text = self.text[:(len(self.text)-1)]
                elif textWidth < 570:
                    self.text += game.keyDownString
#Toggle element for letting the users toggle options, such as best-of mode
class Toggle:
    value = False
    x = 0
    y = 0
    width = 80
    height = 30
    def __init__(self, _x, _y):
        if _x == "centered":
            self.x  = screenWidth/2 - self.width/2
        else:
            self.x = _x
        if _y == "centered":
            self.y  = screenHeight/2 - self.height/2
        else:
            self.y = _y
    def draw(self):
        drawRect(self.x, self.y, self.width, self.height, DarkerLineColour)
        if self.value == False:
            drawRect(self.x+4, self.y+4, self.width - 8, self.height - 8, BackGroundColour)
            drawRect(self.x+4 + (self.width - 8)/2, self.y+4, (self.width - 8)/2, self.height - 8, LineColour)
            game.drawText("Off", self.x + self.width*2, self.y + self.height/2, 55, DarkerLineColour)
        else:
            drawRect(self.x+4, self.y+4, self.width - 8, self.height - 8, BackGroundColour)
            drawRect(self.x+4, self.y+4, (self.width - 8)/2, self.height - 8, DarkerBackGroundColour)
            game.drawText("On", self.x + self.width*2, self.y + self.height/2, 55, DarkerLineColour)
    def checkForMouse(self):
        if game.mouseLeft and game.lastMouseLeft == False:
            if game.mouseX > self.x and game.mouseX < self.x + self.width and game.mouseY > self.y and game.mouseY < self.y + self.height:
                if self.value:
                    self.value = False
                else:
                    self.value = True
#Base class Button inherited by all button classes
class Button:
    mainImage = 0
    altImage = 0
    hoveredOver = False
    x = 0
    y = 0
    width = 0
    height = 0
    value = False
    def __init__(self, _x, _y, _width, _height, _mainPath, _altPath):
        if _x == "centered":
            self.x  = screenWidth/2 - _width/2
        else:
            self.x = _x
        if _y == "centered":
            self.y  = screenHeight/2 - _height/2
        else:
            self.y = _y
        self.width = _width
        self.height = _height
        self.loadImages(_mainPath, _altPath)
    def loadImages(self, mainPath, altPath):
        #needed to load the 2 different images, the main one, and the one displayed when hovered over by the mouse
        self.mainImage = pygame.image.load(mainPath)
        self.altImage = pygame.image.load(altPath)
    #this method is replaced by the derived classes with methods than run the code that carries out the functionality of that specific button
    def run(self):
        print(self.x)
    def draw(self):
        if self.hoveredOver:
            gameDisplay.blit(self.altImage, (self.x,self.y))
        else:
            gameDisplay.blit(self.mainImage, (self.x,self.y))
    def checkForMouse(self):
        if game.mouseX > self.x and game.mouseX < self.x + self.width and game.mouseY > self.y and game.mouseY < self.y + self.height:
            if game.mouseLeft and game.lastMouseLeft == False:
                self.run()
            else:
                self.hoveredOver = True
        else:
            self.hoveredOver = False
###BUTTONS:-----------
#leads to the setup screen, but with player vs player mode on
class pvpButton(Button):
    def __init__(self, _x, _y):
        Button.__init__(self, _x, _y, 400, 100, "pvpButtonMain.png", "pvpButtonAlt.png")
    def run(self):
        game.gameState = 1
        game.aiMode = 0
        #to prevent clicking into next screen
        game.lastMouseLeft = True
#leads to the setup screen, but with player vs AI mode on
class pvaiButton(Button):
    def __init__(self, _x, _y):
        Button.__init__(self, _x, _y, 400, 100, "pvcButtonMain.png", "pvcButtonAlt.png")
    def run(self):
        game.gameState = 1
        game.aiMode = 1
        #to prevent clicking into next screen
        game.lastMouseLeft = True
#closes the game
class exitButton(Button):
    def __init__(self, _x, _y):
        Button.__init__(self, _x, _y, 400, 100, "exitButtonMain.png", "exitButtonAlt.png")
    def run(self):
        game.running = False
        #to prevent clicking into next screen
        game.lastMouseLeft = True
#goes back to the main menu
class menuButton(Button):
    def __init__(self, _x, _y):
        Button.__init__(self, _x, _y, 400, 100, "menuButtonMain.png", "menuButtonAlt.png")
    def run(self):
        game.gameState = 0
        #to prevent clicking into next screen
        game.lastMouseLeft = True
#starts a game, usually switching from the setup screen to the game screen.
class playButton(Button):
    def __init__(self, _x, _y):
        Button.__init__(self, _x, _y, 400, 100, "playButtonMain.png", "playButtonAlt.png")
    def run(self):
        game.gameState = 2
        #to prevent clicking into next screen
        game.lastMouseLeft = True
#loads the save game
class loadSaveButton(Button):
    def __init__(self, _x, _y):
        Button.__init__(self, _x, _y, 400, 100, "loadButtonMain.png", "loadButtonAlt.png")
    def run(self):
        game.wipeGrid()
        game.readFromSave()
        game.gameState = 2
        #to prevent clicking into next screen
        game.lastMouseLeft = True
#saves the current game and exits to the menu
class saveAndExitButton(Button):
    def __init__(self, _x, _y):
        Button.__init__(self, _x, _y, 400, 100, "s&qButtonMain.png", "s&qButtonAlt.png")
    def run(self):
        game.writeToSave()
        game.gameState = 0
        #to prevent clicking into next screen
        game.lastMouseLeft = True
###/BUTTONS:-----------
###/GUI ELEMENTS:-----------
#this class manages all the gui elements, include drawing them, updating them, and the array that contains them
class GUIElementWrapper:
    buttons = []
    #method specifically for adding buttons
    #NOTE ABOUT BUTTONS: I wish there was a nicer way to have inheritance for buttons, as I have to have many different classes that all work in a similar way. But that might be the way I have to have it as there run functions are all different
    def addButton(self, _type, _x, _y):
        if _type == "pvp":
            self.buttons.append(pvpButton(_x, _y))
        elif _type == "pvc":
            self.buttons.append(pvaiButton(_x, _y))
        elif _type == "exit":
            self.buttons.append(exitButton(_x, _y))
        elif _type == "menu":
            self.buttons.append(menuButton(_x, _y))
        elif _type == "play":
            self.buttons.append(playButton(_x, _y))
        elif _type == "load":
            self.buttons.append(loadSaveButton(_x, _y))
        elif _type == "save":
            self.buttons.append(saveAndExitButton(_x, _y))
    #method for adding a textbox, cant have one method for adding all types of GUI elements without having redundant parameters (name etc)
    def addTextBox(self, _x, _y, _name):
        self.buttons.append(TextBox(_x, _y, _name))
    #method for adding a toggle
    def addToggle(self, _x, _y):
        self.buttons.append(Toggle(_x, _y))
    def addIncrimentalClicker(self, _x, _y, _minValue, _maxValue, _increment):
        self.buttons.append(IncrementalClicker(_x, _y, _minValue, _maxValue, _increment))
    #draws all GUI elements
    def drawElements(self):
        for i in range(0, len(self.buttons)):
            self.buttons[i].draw()
    def updateElements(self):
        for i in range(0, len(self.buttons)):
            self.buttons[i].checkForMouse()
    def clear(self):
        self.buttons = []
###SCREENS:-----------
#Screen base class inherited by all screen derived clases
class Screen:
    #the init function is called when the game switches to this specific screen,
    #it is different from a __Init__() method as it is not only run when the screen
    #is instantiated, but everytime the game switches to this screen(code for this
    #can be found in game's manageScreens method) this is required as the GUI elemnts
    #are setup differently for each different screen, and the setup for that is in
    #this method.
    def init(self):
        x = 0
    #similar to init(), deInit is called when the current screen is changed from this one to another
    def deInit(self):
        x = 0
    def update(self):
        x = 0
    def draw(self):
        x = 0
#The main menu screen first displayed when the game is started
class MenuScreen(Screen):
    tileImage = 0
    #the init function is called when the game switches to this specific screen, it is different from a __Init__() method as it is not only run when the screen instantiated, but everytime the game switches to this screen(code for this in game manageScreens method) 
    def init(self):
        game.GUIElements.addButton("pvp", "centered", 320)
        game.GUIElements.addButton("pvc", "centered", 440)
        game.GUIElements.addButton("load", "centered", 560)
        game.GUIElements.addButton("exit", "centered", 680)
        self.titleImage = pygame.image.load("title.png")
    def draw(self):
        drawRect(0,0,game.screenWidth, game.screenHeight, BackGroundColour)
        drawRect(0,0,game.screenWidth, game.headerHeight, LineColour)
        drawRect(0,game.screenHeight - game.footerHeight, game.screenWidth, game.footerHeight, LineColour)
        gameDisplay.blit(self.titleImage, (150, 100))
#the setup screen, used for both AI mode games and player vs player games, (the setup and running of the object is different based on whether or not AI mode is on or not)
class SetupScreen(Screen):
    def init(self):
        if game.aiMode != 0:
            game.GUIElements.addTextBox("centered", 100, "Enter Player Name")
            game.GUIElements.addToggle("centered", 230)
        else:
            game.GUIElements.addTextBox("centered", 100, "Enter Player1's Name")
            game.GUIElements.addTextBox("centered", 200, "Enter Player2's Name")
        game.GUIElements.addToggle("centered", 300)
        game.GUIElements.addIncrimentalClicker("centered", 360, 3, 13, 2)
        game.GUIElements.addButton("menu", "centered", 500)
        game.GUIElements.addButton("play", "centered", 620)
        game.resetBestOf()
        game.wipeGrid()
    def deInit(self):
        game.player1Name = game.GUIElements.buttons[0].text
        if game.aiMode != 0:
            game.player2Name = "AI"
            game.bestOfMode = game.GUIElements.buttons[2].value
            if game.GUIElements.buttons[1].value:
                game.aiMode = 2
            else:
                game.aiMode = 1
            game.bestOfTotalRounds = game.GUIElements.buttons[3].value
        else:
            game.player2Name = game.GUIElements.buttons[1].text
            game.bestOfMode = game.GUIElements.buttons[2].value
            game.bestOfTotalRounds = game.GUIElements.buttons[3].value
        if game.player1Name == "":
            game.player1Name = "Player 1"
        if game.player2Name == "":
            game.player2Name = "Player 2"
        #too make sure player one goes first every time
        game.turn = 1
    def update(self):
        game.GUIElements.buttons[3].visible = game.GUIElements.buttons[2].value
    def draw(self):
        drawRect(0,0,game.screenWidth, game.screenHeight, BackGroundColour)
        drawRect(0,0,game.screenWidth,game.headerHeight, LineColour)
        drawRect(0,game.screenHeight - game.footerHeight,game.screenWidth,game.footerHeight, LineColour)
        if game.aiMode != 0:
            game.drawText("Never lose:", 240, 240, 55, LineColour)
            game.drawText("BestOf:", 240, 320, 55, LineColour)
        else:
            game.drawText("BestOf:", 240, 380, 55, LineColour)
#the screen used for the actually game, displaying the board. again much like the setup screen it is used for both player vs player games and player vs AI games
class GameScreen(Screen):
    def init(self):
        game.GUIElements.addButton("save", "centered" ,750)
    def update(self):
        if game.aiMode != 0:
            if game.turn == 1:
                roundedX = int(math.floor((game.mouseX-game.paddingX)/(game.lineWidth+game.spacingX)))
                roundedY = int(math.floor((game.mouseY-game.paddingY)/(game.lineHeight+game.spacingY)))
                if roundedX > -1 and roundedX < 3 and roundedY > -1 and roundedY < 3 and game.mouseLeft == True and game.lastMouseLeft == False and game.grid[roundedX][roundedY] == 0:
                    game.grid[roundedX][roundedY] = 1
                    game.turn = 2
            else:
                if game.aiMode == 1:
                    game.runRandomAI()
                if game.aiMode == 2:
                    game.runMinMaxAI()
        elif game.mouseLeft == True and game.lastMouseLeft == False:
            roundedX = int(math.floor((game.mouseX-game.paddingX)/(game.lineWidth+game.spacingX)))
            roundedY = int(math.floor((game.mouseY-game.paddingY)/(game.lineHeight+game.spacingY)))
            if roundedX > -1 and roundedX < 3 and roundedY > -1 and roundedY < 3 and game.grid[roundedX][roundedY] == 0:
                if game.turn == 1:
                    game.grid[roundedX][roundedY] = 1
                    game.turn = 2
                elif game.turn == 2:
                    game.grid[roundedX][roundedY] = 2
                    game.turn = 1
        result = game.testForResult(game.grid)
        cellsFilled = 0
        for i in range(0,game.boardSize):
            for j in range(0,game.boardSize):
                if game.grid[i][j] != 0:
                    cellsFilled += 1
        if cellsFilled == 9 and result == 0:
            self.draw()
            pygame.display.update()
            pygame.time.delay(game.showResultDelay*1000)
            game.wipeGrid()
        elif result != 0:
            self.draw()
            game.drawStrike()
            pygame.display.update()
            pygame.time.delay(game.showResultDelay*1000)
            if game.bestOfMode:
                game.wipeGrid()
                game.bestOfRound += 1
                if result == 1:
                    game.bestOfScorePlayer1 += 1
                if result == 2:
                    game.bestOfScorePlayer2 += 1
                if game.bestOfScorePlayer1 > game.bestOfTotalRounds/2:
                    game.gameState = 3
                    game.winner = result
                elif game.bestOfScorePlayer2 > game.bestOfTotalRounds/2:
                    game.gameState = 3
                    game.winner = result
            else:
                game.wipeGrid()
                game.gameState = 3
                game.winner = result
    def draw(self):
        drawRect(0,0,screenWidth,screenHeight, BackGroundColour)

        drawRect(0,0,game.screenWidth,game.headerHeight, LineColour)
        drawRect(0,game.screenHeight - game.footerHeight,game.screenWidth,game.footerHeight, LineColour)

        drawRect(game.paddingX+game.spacingX, game.paddingY, game.lineWidth, game.totalHeight, LineColour)
        drawRect(game.paddingX + 2*game.spacingX + game.lineWidth, game.paddingY, game.lineWidth, game.totalHeight, LineColour)
        drawRect(game.paddingX,game.paddingY + game.spacingY,game.totalWidth,game.lineHeight, LineColour)
        drawRect(game.paddingX,game.paddingY + 2*game.spacingY + game.lineHeight,game.totalWidth,game.lineHeight, LineColour)
        for i in range(0, 3):
            for j in range(0, 3):
                if game.grid[i][j] ==1:
                    gameDisplay.blit(game.imageX, (game.paddingX + 40 + i*(game.lineWidth+game.spacingX),game.paddingY + 40 + j*(game.lineHeight+game.spacingY)))
                elif game.grid[i][j] == 2:
                    gameDisplay.blit(game.imageO, (game.paddingX + 40 + i*(game.lineWidth+game.spacingX),game.paddingY + 40 + j*(game.lineHeight+game.spacingY)))
        if game.turn == 1:
            game.drawText(game.player1Name + "'s turn", screenWidth/2, 710, 55, DarkerLineColour)
        else:
            game.drawText(game.player2Name + "'s turn", screenWidth/2, 710, 55, DarkerLineColour)
        if game.bestOfMode:
            game.drawText("Round: " + str(game.bestOfRound + 1) + " (Best of " + str(game.bestOfTotalRounds) + ")", screenWidth/2, 140, 55, DarkerLineColour)
        if game.bestOfMode:
            game.drawText(game.player1Name + ": " + str(game.bestOfScorePlayer1), screenWidth-210, 30, 35, BackGroundColour)
            game.drawText(game.player2Name + ": " + str(game.bestOfScorePlayer2), 210, 30, 35, BackGroundColour)
class GameOverScreen(Screen):
    def init(self):
        game.GUIElements.addButton("menu", "centered", screenHeight/2 + 100)
    def draw(self):
        drawRect(0,0,game.screenWidth, game.screenHeight, BackGroundColour)
        drawRect(0,0,game.screenWidth,game.headerHeight, LineColour)
        drawRect(0,game.screenHeight - game.footerHeight,game.screenWidth,game.footerHeight, LineColour)
        if game.winner == 1:
            game.drawText(game.player1Name + " wins", screenWidth/2, screenHeight/2 - 100, 55, DarkerLineColour)
        else:
            game.drawText(game.player2Name + " wins", screenWidth/2, screenHeight/2 - 100, 55, DarkerLineColour)
        if game.bestOfMode:
            if game.winner == 1:
                game.drawText(str(game.bestOfScorePlayer1) + " : " + str(game.bestOfScorePlayer2), screenWidth/2, screenHeight/2, 55, DarkerLineColour)
            else:
                game.drawText(str(game.bestOfScorePlayer2) + " : " + str(game.bestOfScorePlayer1), screenWidth/2, screenHeight/2, 55, DarkerLineColour)
###/SCREENS:-----------
#The main game vlass, most of the game is contained in here, including all screens, game logic, game variables, etc
class Game:
    #window variables
    screenWidth = 900
    screenHeight = 900
    running = True

    #Game variables
    turn = 1
    boardSize = 3
    grid = []
    GUIElements = GUIElementWrapper()
    for i in range(0, boardSize):
        grid.append([])
        for j in range(0, boardSize):
            grid[i].append(0)
    player1Name = "Player 1"
    player2Name = "Player 2"
    winner = 0
    bestOfMode = True
    bestOfTotalRounds = 3
    bestOfRound = 0
    bestOfScorePlayer1 = 0
    bestOfScorePlayer2 = 0
    aiMode = 1
    showResultDelay = 2 #seconds the board is shown for after the game has finished

    #Drawing variables
    paddingX = 200
    paddingY = 180
    totalWidth = 500
    totalHeight = 500
    lineWidth = 20
    lineHeight = 20
    headerHeight = 80
    footerHeight = 50
    spacingX = (totalWidth-2*lineWidth)/3
    spacingY = (totalHeight-2*lineHeight)/3

    #Screens
    menuScreen = MenuScreen()
    setupScreen = SetupScreen()
    gameScreen = GameScreen()
    gameOverScreen = GameOverScreen()

    #Input variables
    mouseX = 0
    mouseY = 0
    mouseLeft = False
    lastMouseLeft = False
    isKeyDown = False
    keyDownString = "default"
    lastKeyDownString = "default"

    #GAMESTATE:
    #0: start menu
    #1: setup
    #2: game
    #3: game over
    gameState = 0
    lastGameState = 0

    #Game images
    imageX = pygame.image.load('x.png')
    imageO = pygame.image.load('o.png')
    verticalStrike = pygame.image.load('verticalStrike.png')
    horizontalStrike = pygame.image.load('horizontalStrike.png')
    #top left to bottom right
    imagediagonalStrike1 = pygame.image.load('diagonalStrike1.png')
    #bottom left to top right
    imagediagonalStrike2 = pygame.image.load('diagonalStrike2.png')
    ###/AI -------

    bestNextMove = copy.deepcopy(grid)

    def gamecomplete(self, grid):
        #checks if the board is fully populated
        result = 1
        for i in range(0,self.boardSize):
            for j in range(0,self.boardSize):
                if grid[i][j] == 0:
                    return 0
        return result

    def score(self, grid,boardSize):
        winner = self.testForResult(grid)
        testResult = 0
        if winner == 1:
            result = 1000
        elif winner == 2:
            result = -1000
        else:
            result = 0
        return result
    def miniMax(self, board,player,rec_level):
        blankspace = " "
        if player == 1:
            bestscore = -100
        else:
            bestscore = 100
        currentscore = self.score(board,self.boardSize)
        #If game over
        if currentscore !=0 or self.gamecomplete(board)==1:
            bestscore = currentscore*(1+(10-rec_level)//100)
        else:
            currentscore = 0
            #Loop through every cell to find the next cell that has not been taken
            for i in range(0,self.boardSize):
                for j in range(0,self.boardSize):
                    #Take a copy of the game state                    
                    if board[i][j] == 0:
                        prospect = copy.deepcopy(board)
                        #If an empty cell is found we need to spawn another game
                        #Set the next available cell to the current player
                        prospect[i][j]=player
                        #finding the best move depends on whos turn it is
                        if player == 2:
                            #spawn off a new game for player 2
                            prospectMiniMax = self.miniMax(prospect,1,rec_level+1)
                            if prospectMiniMax<bestscore:
                                bestscore = prospectMiniMax
                                if rec_level == 0:
                                    #record the current game state because this is the current best state for AI
                                    self.bestNextMove =  copy.deepcopy(prospect)
                        else:
                            #spawn off a new game for player 1
                            prospectMiniMax = self.miniMax(prospect,2,rec_level+1)                      
                            if prospectMiniMax>bestscore:
                                bestscore = prospectMiniMax 
        #returning the score of the game jsut evaluation (only useful on recursion levels other than 1) the idea of the function is the change the self.bestnextmove variable to the best result, not return a variable(it's just needed for recursion to work)
        return bestscore 
    def runMinMaxAI(self):
        cellsFilled = 0
        for i in range(0,self.boardSize):
            for j in range(0,self.boardSize):
                if self.grid[i][j] != 0:
                    cellsFilled += 1
        #some pre-defined moved for early in the game to greatly improve time, the minimax algorithm would actually make the same moves but it would take a long time as it has to evaluation all 9 or 8 squares in the boar
        if cellsFilled == 0:
            self.wipeGrid()
            self.grid =[[0,0,0],[0,2,0],[0,0,0]]
        elif cellsFilled == 1:
            if self.grid == [[0,0,0],[0,1,0],[0,0,0]]:
                self.grid = [[0,0,0],[0,1,0],[2,0,0]]
            else:
                self.grid[1][1] = 2
        else:
            #the miniMax algorithm actually returns a value, although is it only relevant to the recursion
            self.miniMax(self.grid, 2, 0)
            #self.bestNextMove has already been set by slef.miniMax
            self.grid = self.bestNextMove
        #switching the turn back to the player
        self.turn = 1
    def runRandomAI(self):
        #small delay added to make it feel like you are playing a real player
        pygame.time.delay(300)
        while True: 
            x = random.randint(0, 2)
            y = random.randint(0, 2)
            if self.grid[x][y] == 0:
                self.grid[x][y] = 2
                break
        self.turn = 1                  
    ###/AI -------
    
    ###saving code -------
    saveFile = open("save.txt", "r+")
    def wipeSave(self):
        self.saveFile = open("save.txt", "r+")
        self.saveFile.truncate()
    def writeToSave(self):
        self.wipeSave()
        for i in range(0, 3):
            for j in range(0, 3):
                self.saveFile.write(str(self.grid[i][j]) + '|')
        if game.bestOfMode:
            self.saveFile.write("y")
        else:
            self.saveFile.write("n")
        self.saveFile.write(str(self.bestOfScorePlayer1))
        self.saveFile.write(str(self.bestOfScorePlayer2))
        self.saveFile.write(str(self.bestOfRound))
        self.saveFile.write(str(self.turn))
        self.saveFile.write(str(self.aiMode))
        self.saveFile.close()
    def isInt(self, _string):
        try: 
            int(_string)
            return True
        except ValueError:
            return False
    def readFromSave(self):
        self.saveFile = open("save.txt", "r+")
        string = self.saveFile.read(24)
        if self.isInt(string[0]):
            self.grid[0][0] = int(string[0])
        if self.isInt(string[2]):
            self.grid[0][1] = int(string[2])
        if self.isInt(string[4]):
            self.grid[0][2] = int(string[4])
        if self.isInt(string[6]):
            self.grid[1][0] = int(string[6])
        if self.isInt(string[8]):
            self.grid[1][1] = int(string[8])
        if self.isInt(string[10]):
            self.grid[1][2] = int(string[10])
        if self.isInt(string[12]):
            self.grid[2][0] = int(string[12])
        if self.isInt(string[14]):
            self.grid[2][1] = int(string[14])
        if self.isInt(string[16]):
            self.grid[2][2] = int(string[16])
        if string[18] == "y":
            self.bestOfMode = True
            self.bestOfScorePlayer1 = int(string[19])
            self.bestOfScorePlayer2 = int(string[20])
            self.bestOfRound = int(string[21])
        else:
            self.bestOfMode = False
        self.turn = int(string[22])
        self.aiMode = int(string[23])
        if self.aiMode != 0:
            self.player2Name = "AI"
    ###/saving code -------
    def resetBestOf(self):
        self.bestOfRound = 0
        self.bestOfScorePlayer1 = 0
        self.bestOfScorePlayer2 = 0
    ###Text drawing
    def text_objects(self, _text, _font, _color):
        textSurface = _font.render(_text, True, _color)
        return textSurface, textSurface.get_rect()
    def drawText(self, _string, _x, _y, _size, _color):
        largeText = pygame.font.Font('Roboto-thin.ttf',_size)
        TextSurf, TextRect = self.text_objects(_string, largeText, _color)
        TextRect.center = ((_x),(_y))
        gameDisplay.blit(TextSurf, TextRect)
    ###/Text drawing
    #method for drawing the red strike through the board when a player wins
    def drawStrike(self):
        winType = self.getWinType()
        if winType == 1:
            gameDisplay.blit(self.horizontalStrike, (self.paddingX,self.paddingY-self.spacingY))
        if winType == 2:
            gameDisplay.blit(self.horizontalStrike, (self.paddingX,self.paddingY))
        if winType == 3:
            gameDisplay.blit(self.horizontalStrike, (self.paddingX,self.paddingY+self.spacingY))
        if winType == 4:
            gameDisplay.blit(self.verticalStrike, (self.paddingX-self.spacingX,self.paddingY))
        if winType == 5:
            gameDisplay.blit(self.verticalStrike, (self.paddingX,self.paddingY))
        if winType == 6:
            gameDisplay.blit(self.verticalStrike, (self.paddingX+self.spacingX,self.paddingY))
        if winType == 7:
            gameDisplay.blit(self.imagediagonalStrike1, (self.paddingX,self.paddingY))
        if winType == 8:
            gameDisplay.blit(self.imagediagonalStrike2, (self.paddingX,self.paddingY))
    #method used to find the type of win, (diagonal top left to bottom right, horizontal bottomrow, etc)
    def getWinType(self):
        for i in range(0,self.boardSize):
            if self.grid[0][i] == self.grid[1][i] == self.grid[2][i] and self.grid[0][i] != 0:
                return i + 1
            if self.grid[i][0] == self.grid[i][1] == self.grid[i][2] and self.grid[i][0] != 0:
                return 4 + i
        if self.grid != 0:
            if self.grid[0][0] == self.grid[1][1] == self.grid[2][2]:
                return 7
            if self.grid[2][0] == self.grid[1][1] == self.grid[0][2]:
                return 8
        return 0
    #test the grid to see if a player/AI has won, returns 1 for player 1, 2 for player 2/AI and 0 if nobody has won
    def testForResult(self, grid):
        for i in range(0,self.boardSize):
            if grid[0][i] == grid[1][i] == grid[2][i] and grid[0][i] != 0:
                return grid[0][i]
            if grid[i][0] == grid[i][1] == grid[i][2] and grid[i][0] != 0:
                return grid[i][0]
        if grid != 0:
            if grid[0][0] == grid[1][1] == grid[2][2]:
                return grid[0][0]
            if grid[2][0] == grid[1][1] == grid[0][2]:
                return grid[2][0]
        return 0
    #methof used to completely clear the grid
    def wipeGrid(self):
     for i in range(0, 3):
        for j in range(0, 3):
            self.grid[i][j] = 0
    
    def pollForInputs(self):
        self.keyDownString = "default"
        self.lastKeyDownString = self.keyDownString
        self.lastMouseLeft = self.mouseLeft
        for event in pygame.event.get():
            (self.mouseX, self.mouseY) = pygame.mouse.get_pos()
            if event.type == pygame.MOUSEBUTTONDOWN:
                self.mouseLeft = True
                (self.mouseX, self.mouseY) = pygame.mouse.get_pos()
                #print self.mouseX, self.mouseY
            else:
                (self.mouseX, self.mouseY) = pygame.mouse.get_pos()
                self.mouseLeft = False
            if event.type == pygame.QUIT:
                self.running = False
            if event.type == pygame.KEYDOWN:
                self.isKeyDown = True
                self.keyDownString = unichr(event.key)
                #print event.key
                if event.key == 8:
                    self.keyDownString = "BACKSPACE"
                elif int(event.key) < 32 or int(event.key) > 126:
                    self.keyDownString = ""
            else:
                self.isKeyDown = True
    def manageScreens(self):
        if self.gameState != self.lastGameState:
            if self.lastGameState == 0:
                self.menuScreen.deInit()
            elif self.lastGameState == 1:
                self.setupScreen.deInit()
            elif self.lastGameState == 2:
                self.gameScreen.deInit()
            elif self.lastGameState == 3:
                self.gameOverScreen.deInit()
            self.GUIElements.clear()
            if self.gameState == 0:
                self.menuScreen.init()
            elif self.gameState == 1:
                self.setupScreen.init()
            elif self.gameState == 2:
                self.gameScreen.init()
            elif self.gameState == 3:
                self.gameOverScreen.init()
            elif self.gameState == 4:
                self.setupScreenPvAi.init()
    
        self.lastGameState = self.gameState  
    def update(self):
        if self.gameState == 3:
            self.gameOverScreen.update()
        elif self.gameState == 0:
            self.menuScreen.update()
        elif self.gameState == 1:
            self.setupScreen.update()
        elif self.gameState == 2:
            self.gameScreen.update()
        self.GUIElements.updateElements()
    def draw(self):
        if self.gameState == 0:
            self.menuScreen.draw()
        elif self.gameState == 1:
            self.setupScreen.draw()
        elif self.gameState == 2:
            self.gameScreen.draw()
        elif self.gameState == 3:
            self.gameOverScreen.draw()
        self.GUIElements.drawElements()
    def mainLoop(self):      
        self.pollForInputs()
        self.manageScreens()
        self.update()
        self.draw()

        pygame.display.update()
    def start(self):
        while self.running:
            self.mainLoop()
game = Game()
game.menuScreen.init()
game.start()
game.saveFile.close()
pygame.display.quit()
pygame.quit()
sys.exit()