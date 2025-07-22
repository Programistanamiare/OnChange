#include "OnChange.hpp"

DigitalInput_t::DigitalInput_t() : pin{255}, mode{0} {}

void DigitalInput_t::init(pin_t pin, mode_t mode)
{
  this->pin = pin;
  this->mode = mode;
  pinMode(pin, mode);
  update();
  update();
}

void DigitalInput_t::update()
{
  state.prev = state.curr;
  state.curr = (mode == INPUT ? digitalRead(pin) : !digitalRead(pin));
}

digital_t DigitalInput_t::onChange() const
{
  return state.prev != state.curr;
}

digital_t DigitalInput_t::onDownChange() const
{
  return state.curr && !state.prev;
}

digital_t DigitalInput_t::onUpChange() const
{
  return state.prev && !state.curr;
}

DigitalInput_t::operator digital_t() const
{
  return state.curr;
}


DigitalOutput_t::DigitalOutput_t() : state(), pin{255} {}

void DigitalOutput_t::init(pin_t pin, digital_t init)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
  state.curr = state.prev = init;
  digitalWrite(pin, init);
}

digital_t DigitalOutput_t::getState() const
{
  return state.curr;
}

void DigitalOutput_t::update()
{
  if (state.curr != state.prev) 
  {
    state.prev = state.curr;
    digitalWrite(pin, state.curr);
  }
}

void DigitalOutput_t::setState(digital_t x)
{
  state.curr = x;
}

void DigitalOutput_t::toggleState()
{
  state.curr = !state.curr;
}

DigitalOutput_t::operator digital_t() const
{
  return state.curr;
}

DigitalOutput_t& DigitalOutput_t::operator=(digital_t x)
{
  this->state.curr = x;
  return *this;
}


AnalogInput_t::AnalogInput_t() : pin{A1} {}

void AnalogInput_t::init(pin_t pin, analog_t sensitive)
{
  setSensitive(sensitive);
  this->pin = pin;
}

void AnalogInput_t::setSensitive(analog_t sensitive)
{
  state.sensitive = sensitive;
}

void AnalogInput_t::update(void)
{
  state.prev = state.curr;
  analog_t new_state = analogRead(pin);
  if (abs((int)new_state - state.curr) < state.sensitive) return;
  state.curr = new_state; 
}

analog_t AnalogInput_t::onDownChange(bool get_change) const
{
  if (state.prev > state.curr && (state.prev - state.curr) >= state.sensitive)
  {
    if (get_change) return state.prev - state.curr;
    return 1;
  }
  return 0;
}

analog_t AnalogInput_t::onUpChange(bool get_change) const
{
  if (state.curr > state.prev && (state.curr - state.prev) >= state.sensitive)
  {
    if (get_change) return state.curr - state.prev;
    return 1;
  }
  return 0;
}

analog_t AnalogInput_t::onChange(bool get_change) const
{
  analog_t down = onDownChange(get_change);
  analog_t up = onUpChange(get_change);
  return down > up ? down : up;
}

AnalogInput_t::operator analog_t() const
{
  return state.curr;
}