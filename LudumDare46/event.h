#ifndef EVENT_H
#define EVENT_H

enum class EventType {
  NONE,
  WIN,
  DIE,
  JUMP,
};

struct Event {
  EventType type = EventType::NONE;
};

#endif
