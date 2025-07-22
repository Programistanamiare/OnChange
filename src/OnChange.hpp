#pragma once

#include <Arduino.h>

using pin_t = uint8_t;
using mode_t = uint8_t;
using analog_t = decltype(analogRead(1));
using digital_t = decltype(digitalRead(1));

template <typename T>
struct _OnChange_t
{
  T curr;
  T prev;
};

template <typename T>
struct _OnChangeSensitive_t : _OnChange_t<T>
{
  T sensitive;
};


class DigitalInput_t
{
public:
  DigitalInput_t();
  void init(pin_t pin, mode_t mode = INPUT);
  void update(void);
  digital_t onDownChange(void) const;
  digital_t onUpChange(void) const;
  digital_t onChange(void) const;
  operator digital_t() const;

private:
  pin_t pin;
  mode_t mode;
  _OnChange_t<digital_t> state;
};


class DigitalOutput_t
{
public:
  DigitalOutput_t();
  void init(pin_t pin, digital_t init = 0);
  void update(void);
  digital_t getState(void) const;
  void setState(digital_t x);
  void toggleState();
  operator digital_t() const;
  DigitalOutput_t& operator=(digital_t state);

private:
  _OnChange_t<digital_t> state;
  pin_t pin;
};


class AnalogInput_t
{
public:
  AnalogInput_t();
  void init(pin_t pin = A1, analog_t sensitive = 1);
  void setSensitive(analog_t sensitive);
  void update(void);
  analog_t onUpChange(bool get_change = false) const;
  analog_t onDownChange(bool get_change = false) const;
  analog_t onChange(bool get_change = false) const;
  operator analog_t() const;
private:
  pin_t pin;
  _OnChangeSensitive_t<analog_t> state;
};