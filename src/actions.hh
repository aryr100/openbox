// -*- mode: C++; indent-tabs-mode: nil; -*-
#ifndef __actions_hh
#define __actions_hh

/*! @file actions.hh
  @brief The action interface for user-available actions
*/

#include "otk/display.hh"
#include "otk/point.hh"
#include "otk/rect.hh"
#include "otk/eventhandler.hh"

namespace ob {

//! The action interface for user-available actions
/*!
  When these actions are fired, hooks to the guile engine are fired so that
  guile code is run.
*/
class OBActions : public otk::OtkEventHandler {
public:
  struct MouseButtonAction {
    Window win;
    unsigned int button;
    Time time;
    MouseButtonAction() { win = 0; button = 0; time = 0; }
  };
  
private:
  // milliseconds XXX: config option
  static const unsigned int DOUBLECLICKDELAY;
  
  //! The last 2 button release processed for CLICKs
  MouseButtonAction _release;
  //! The mouse button currently being watched from a press for a CLICK
  unsigned int _button;
  //! The window the last enter action occured on (where the mouse is located)
  Window _enter_win;

  void insertPress(Window win, unsigned int button, Time time);
  
public:
  OBActions();
  virtual ~OBActions();

  virtual void buttonPressHandler(const XButtonEvent &e);
  virtual void buttonReleaseHandler(const XButtonEvent &e);
  



  //! Notify that a mouse enter action has occured on a window.
  /*!
    @param win The window on which the action was performed.
    @param modifiers The modifier state for the action.
  */
  void enter(Window win, unsigned int modifiers);

  //! Notify that a mouse leave action has occured on a window.
  /*!
    @param modifiers The modifier state for the action.
  */
  void leave(unsigned int modifiers);

  //! Notify that a mouse drag is taking place.
  /*!
    @param win The window the drag is on
    @param delta The change in position of the mouse pointer
    @param modifiers The modifier state during the drag.
  */
  void drag(Window win, otk::Point delta, unsigned int modifiers,
            unsigned int button, Time time);

  //! Notify that a key press has occured on a window.
  /*!
    @param win The window the key press was on
    @param modifiers The modifier state for the action.
    @param keycode The keycode of the key pressed.
  */
  void key(Window win, unsigned int modifiers, unsigned int keycode);
};

}

#endif // __actions_hh
