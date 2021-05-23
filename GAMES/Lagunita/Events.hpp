#pragma once

#include "Buildings.hpp"

/* Represents the state of an event. */
enum class EventState : uint8_t { untriggered, justTriggered, triggered };

/* Represent an immutable event. */
class Events {
 public:
  /* This class cannot be constructed. */
  Events() = delete;

  /* Return the buildings that gets unlocked by this event. */
  static uint8_t buildingUnlocked(uint8_t id);

  /* Return the text of this event. */
  static const char* getText(uint8_t id);

  /* Update an EventState for this event's parameters. */
  static EventState update(
      EventState ev,
      uint8_t id,
      uint16_t population,
      Building::Status buildings[static_cast<uint8_t>(Building::IDs::count)]);

  static const uint8_t count = 15;
};
