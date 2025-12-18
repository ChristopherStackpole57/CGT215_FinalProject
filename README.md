# Asteroid Defense!
Developed by Christopher Stackpole \
for Purdue University CGT 21500: Computer Graphics Programming I

## About the Game

### Brief Outline

A series of previously undetected asteroids has been inbound towards Earth for nearly one hundred thousand years according to 
our scientists. You must use the powerful laser weaponry that our planets best engineers have created in order to destroy the 
incoming asteroid. As well, each asteroid we destroy means recoverable resources that we can spend building more and stronger 
turrets. Use these resources carefully, though, as each new turret and further upgrade will cost us significantly more 
resources and any destroyed SWORDs will be a massive loss of investment!

World -> Your home planet, you must defend this at all costs \
Asteroids -> You must destroy these massive asteroids before they destroy your planet \
ARC -> Asteroid Removal Cannon; A massive, hugely powerful laser weapon used to defend the planet \
Lasers -> An advanced form of energy weapon that our scientists have developed for light speed target destruction \
SWORDs -> Special Weapons and Ordinance Reliquary Depot(s); these are outer-atmospheric turrets that will help defend your planet

### How To Play

A -> Turn Counter Clockwise \
D -> Turn Clockwise 

W -> Shoot Laser 

Q -> Spawn new SWORD \
Z -> Upgrade SWORD Damage \
X -> Upgrade SWORD Fire Rate 

*Note* \
Controls are set by constexpr in **Services/EngineServices/InputService/InputService.h** if they need to be changed.

## About the Development

### Some Important Structural Notes

I built this game (and the engine underneath it) with a couple of key goals for the overall structure:
- A Service-Based Architecture for centralizing program communication
	- This architecture must support and enforce a singleton pattern for stronger system management.
	- Services should use templates whenever possible to best enable polymorphism and general usage.
- A central Call Manager (Service) to proide semi-explicit control (i.e. layered) over program exection.
	- There must be a distinguishment by call order on startup, tick, and shutdown, to allow explicit control over the order 
	  different programs execute in relative to each other.
- Input should be processed and then dispatched to whatever callbacks/listeners are registered with it.
	- Registering these callbacks should be as straightforward as possible to allow better ease of use.
- Resources should be easily fetchable and cached to allow for quicker lookups and lower overall VRAM usage.

Overall I believe I did a pretty decent job sticking to these core principles, though as will be discussed in the next section
I did encounter plenty of other issues.

### Slight Post Mortem Analysis
I generally belive the process of creating this game went well. For many of these architectural designs and systems it was
my first time implementing them (in C++, at least) and though there are certainly main things I would change I do believe
it was an effective first attempt.

A couple of things that I think went really well:
- I believe the enforcement of the singleton pattern is probably the part of this project I am most proud of. I am very
happy with the `Service* service = Services().Get<Service>()` pattern I created and how that let me utilize the service
architecture throughout the project.
- I am also very happy with the call manager and the effective control I gave myself over program execution. While creating it
I was thinking about issues I've encountered with Unity's call system and how I would like to see those issues resolved, and
I believe I have pretty effectively resolved them. 

Some of the major things I noticed I need to do better:
- Much stronger lifecycle management; especially towards the end of the project I made some poor lifecycle management decisions
that left me creating overcomplicated chains of references and management.
- More service ownership; I generally tried to leave ownership of most objects with the object that created them (for instance
game objects / services own the render objects they create). This is not a terrible approach, but it did leave me needing to
hold references to every render object in systems outside the RenderService, which means I cannot *always* guaruntee pointer
validity for the rendering.
- More thorough API exposure and implementation; in trying to keep the scope of this project smaller I wanted to limit the
amount of API style work I did with my interfaces (i.e. keeping IService and IGameObject pretty limited). This complicated
the more complex parts of my game though, as it meant many of the game object classes had to expose their own API control for
things like getting rotation, size, etc. In the long run (or in otherwords when I remake this engine more professionally
sometime in the near future), it would serve me well to create a more thorough and put together API.