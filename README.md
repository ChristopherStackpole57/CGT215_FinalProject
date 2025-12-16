# Untitled Space Game (not the CotC Untitled Space Game though)

## Brief Outline

World -> \
Asteroids -> \
ARC -> \
Lasers -> \
SWORDs -> \

## How To Play

A -> Turn Counter Clockwise \
D -> Turn Clockwise \

W -> Shoot Laser \

Q -> Spawn new SWORD \
Z -> Upgrade SWORD Damage \ 
X -> Upgrade SWORD Fire Rate \

Controls are set by constexpr in InputService.h if they need to be changed

## Some Important Notes

## Slight Post Mortem Analysis
top level design was pretty solid (service architecture etc.) \
design got worse towards the end of the project (ran out of time) \
plenty of things i ran into that i wasnt expecting so i didnt have a good design pre planned \
lots of inconsistencies in how i approach object lifecycle management \

A Different Header Style
========================
i should probably remove scoops and caches because im not going to implement them

## todo
by importance:
- SWORD aiming
- world health bar
- create dedicated release build

## personal; notes

After spending hours fighting SFPhysics because it is fundamentally structured in a very different way than my service
architecture, I have made the decision to not use it. There are a lot of very specific requirements that I have for how the
physics code I use needs to be structured and SFPhysics doesn't fit those requirements. I also have tons of experience writing
Euler integration models already, so rewriting it here won't take me too long.

Organization has become pretty inconsistent as I have had to add things I didn't initially plan for. Definitely lots to learn.

Some dangerous things I noticed:
- lots of sketchy GameObject behavior, lots of work that has to be done manually
- weird combination of some behaviors being very decoupled (because I actually planned for them) and others being super
  coupled (because I didn't plan for them)