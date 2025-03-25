#include "Rand.h"

#include <random>

using namespace steve;

std::default_random_engine Rand::generator;

float Rand::next_float() {
  std::uniform_real_distribution<float> dist(0.f, 1.f);
  return dist(generator);
}

float Rand::next_normal() {
  std::normal_distribution<float> dist(0.5f, 0.1f);
  return std::max(0.f, std::min(1.f, dist(generator)));
}

int Rand::next_velocity_jitter() {
  static std::uniform_int_distribution<int> vj_dist(-8, 8);
  return vj_dist(generator);
}

uint32_t Rand::next(uint32_t min, uint32_t max) {
  std::uniform_int_distribution<uint32_t> dist(min, max);
  return dist(generator);
}
uint64_t Rand::next(uint64_t min, uint64_t max) {
  std::uniform_int_distribution<uint64_t> dist(min, max);
  return dist(generator);
}
int Rand::next(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  return dist(generator);
}
float Rand::next(float min, float max) {
  std::uniform_real_distribution<float> dist(min, max);
  return dist(generator);
}
NoteValue Rand::next(NoteValue min, NoteValue max) {
  return NoteValue(Rand::next(uint64_t(min), uint64_t(max)));
}