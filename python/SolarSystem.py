""" Initializing """
import numpy as np
import math
from scipy.constants import G
import astropy.units as u
import turtle
import time
import random

""" The gravitational constant in [AU^3 / year^2 * M_sun] """
G = G * (u.m * u.m * u.m / (u.s * u.s * u.kg)) # SI-units
G = G.to((u.AU*u.AU*u.AU/(u.year*u.year*u.Msun))).value # astronomical units

""" Initiates turtle screen """
win = turtle.Screen() # initiate window to draw in
win.screensize() # resize canvas
win.setup(width = 1.0, height = 1.0) # set window size
win.setworldcoordinates(-30, -30, 30, 30) # set the coordinate system (AU)
win.bgcolor('black') # background color of window
win.tracer(0) # no automatic window update

""" Defines the class for a celestial body """
class CelestialBody(turtle.Turtle):
    
    # Initialization of the celestial body
    def __init__(self, classification, name, mass, color, size, x, y, vx, vy): 
        super().__init__(shape='circle')
        self.classification = classification
        self.color(color)
        self.size = size
        self.shapesize(size, size)
        self.up()
        self.name = name
        self.mass = mass
        self.pos = np.array([x,y])
        self.vel = np.array([vx,vy])
        
    # Function that calculates gravitational pull
    def gravForce(self, Body):
        eps = 0.001 # dampening factor used in acceleration-calculation
        d = np.linalg.norm(Body.pos - self.pos) # distance between bodies
        a = G * Body.mass * (Body.pos - self.pos) / ((eps + d**3))    
        return a 
    
    # Step function
    def move(self, destination):
        self.goto(destination)
        self.pos = destination


""" Computes the gravitational attraction on all planets in the system """
def Gravity(bodies):  
    # Array containing the acceleration for the planets 
    acceleration = np.zeros((len(bodies)-1,2)) # only planets
    
    # Calculates the gravitational attraction on all planetary bodies
    for i, planet in enumerate(bodies[1:]): # only planets
        atot = np.array([0.,0.])
        for body in bodies:            
            if body is planet: # body can't attract itself
                continue          
            acc = planet.gravForce(body)           
            atot += acc         
        acceleration[i,:] = atot
    return acceleration 

""" Converts 10^24 kg to M_sun """
def kgToSolMass(mass):
    newMass = mass * math.pow(10,24) * u.kg.to(u.Msun)
    return newMass

""" Converts km/s to AU/year """
def km_sToAU_Year(vel):
    newVel = vel * (u.km / u.s).to(u.au / u.year)
    return newVel

""" Converts 10^6 km to AU """
def kmToAU(dist):
    newDist = dist * math.pow(10,6) * u.km.to(u.au)
    return newDist

""" Initiates the celestial bodies in the solar system """
""" Data taken from https://nssdc.gsfc.nasa.gov/planetary/factsheet/ """
sun = CelestialBody('star','The Sun', 1, 'yellow', 0.5, 0, 0, 0, 0)

mercury = CelestialBody('planet','Mercury', kgToSolMass(0.330), 'brown', 0.1, 
kmToAU(57.9), 0, 0, km_sToAU_Year(47.4))

venus = CelestialBody('planet','Venus', kgToSolMass(4.87), 'green', 0.2, 
kmToAU(108.2), 0, 0, km_sToAU_Year(35.0))

earth = CelestialBody('planet','Earth', kgToSolMass(5.97), 'blue', 0.2, 
1, 0, 0, km_sToAU_Year(29.8))

mars = CelestialBody('planet','Mars', kgToSolMass(0.642), 'red', 0.2, 
kmToAU(227.9), 0, 0, km_sToAU_Year(24.1))

jupiter = CelestialBody('planet','Jupiter', kgToSolMass(1898), 'orange', 0.3, 
kmToAU(778.6), 0, 0, km_sToAU_Year(13.1))

saturn = CelestialBody('planet','Saturn', kgToSolMass(568), 'maroon', 0.3, 
kmToAU(1433.5), 0, 0, km_sToAU_Year(9.7))

uranus = CelestialBody('planet','Uranus', kgToSolMass(86.8), 'purple', 0.3, 
kmToAU(2872.5), 0, 0, km_sToAU_Year(6.8))

neptune = CelestialBody('planet','Neptune', kgToSolMass(102), 'lightseagreen', 
0.3, kmToAU(4495.1), 0, 0, km_sToAU_Year(5.4))

""" Creates a list of the bodies """
bodies = []
bodies.append(sun)
bodies.append(mercury)
bodies.append(venus)
bodies.append(earth)
bodies.append(mars)
bodies.append(jupiter)
bodies.append(saturn)
bodies.append(uranus)
bodies.append(neptune)


""" Vectors containing velocity and position for moving bodies """
vel = np.zeros((len(bodies)-1,2)) # only planets
pos = np.zeros((len(bodies)-1,2)) # only planets

i = 0
for planet in bodies[1:]: # for stationary Sun
    if planet.classification == 'star':
        continue
    vel[i,:] = planet.vel 
    pos[i,:] = planet.pos
    i += 1
    
""" Time-stepping """
t = 0
dt = 1 * u.day.to(u.year) # time-step is 1 day in units of [year]

""" Initial acceleration in the system """
a = Gravity(bodies)

runSim = True
T = 100

""" Main simulation loop """
while runSim:
    win.update()
    
    # Updates the current time in days and years since simulation start in
    # the simulation window top-border
    win.title( "Day: " + str(round(t*u.year.to(u.day))) + 
              " Year: " + str(math.floor((t*u.year).value)))
   
    # Loop for moving the planets (and random stars)
    for i, planet in enumerate(bodies[1:]): # excluding the movement of the Sun
        planet.move(pos[i,:]) # moves the planets one frame
       # planet.down() # trace orbits (SLOW), comment for faster simulation

        
    # Leapfrog integration
    vel += a*dt/2	
    pos += vel*dt
    a = Gravity(bodies)
    vel += a*dt/2
    t += dt
    
   # if t <= T + dt and t > T: # stops Simulation after time T
   #     runSim = False
    
    time.sleep(0) # change depending on how smooth the simulation should be
