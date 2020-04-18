#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL.h>

#include "geometry.h"
#include "sprite.h"

struct Particle {
  Rect rect;
  Sprite sprite;
  Vec vel;
  double angle;
  double rot_vel;
  bool remove;
};

class ParticleManager {
 public:
  ParticleManager(const Rect& bounds) : bounds(bounds) {}

  void Add(const Particle& particle);
  void Update(double t);
  void Draw(SDL_Renderer* renderer) const;

 private:
  // Once a particle exits this box, it is destroyed.
  Rect bounds;
  std::vector<Particle> particles;
};

#endif
