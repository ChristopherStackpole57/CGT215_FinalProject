After spending hours fighting SFPhysics because it is fundamentally structured in a very different way than my service
architecture, I have made the decision to not use it. There are a lot of very specific requirements that I have for how the
physics code I use needs to be structured and SFPhysics doesn't fit those requirements. I also have tons of experience writing
Euler integration models already, so rewriting it here won't take me too long.

Organization has become pretty inconsistent as I have had to add things I didn't initially plan for. Definitely lots to learn.

Some dangerous things I noticed:
- lots of sketchy GameObject behavior, lots of work that has to be done manually
- weird combination of some behaviors being very decoupled (because I actually planned for them) and others being super
  coupled (because I didn't plan for them)