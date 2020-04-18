#include "particle.h"

void ParticleManager::Add(const Particle& particle) {
  particles.push_back(particle);
}

void ParticleManager::Update(double t) {
  for (auto p = particles.begin(); p != particles.end(); ++p) {
    p->vel.y += 50.0*t;
    p->rect.x += p->vel.x*t;
    p->rect.y += p->vel.y*t;
    p->angle += p->rot_vel*t;
    if (!Intersects(p->rect, bounds)) {
      p->remove = true;
    }
  }
  auto new_end = std::remove_if(particles.begin(), particles.end(),
                                [](const Particle& p) { return p.remove; });
  if (new_end != particles.end()) {
    particles.erase(new_end, particles.end());
  }
}

void ParticleManager::Draw(SDL_Renderer* renderer) const {
  for (const Particle& p : particles) {
    p.sprite.DrawAngle(renderer, ToSDLRect(p.rect), p.angle);
  }
}
