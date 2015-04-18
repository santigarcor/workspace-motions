/*
 * controller_listener.cpp
 *
 *  Created on: Apr 18, 2015
 *      Author: santigarcor
 */

#include "controller_listener.h"

#include "Leap.h"
using namespace Leap;

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

ControllerListener::ControllerListener(float swipeAngle, float screenTapAngle){
	this->screenTapAngle = screenTapAngle;
	this->swipeAngle = swipeAngle;
	this->xdo = xdo_new(NULL);
}

/*
 * Called when the Controller connects to the Leap Motion and
 * the Leap Motion hardware device is plugged in,
 * or when this listener is added to a Controller that is already connected
 */
void ControllerListener::onConnect(const Controller &controller) {
  std::cout << "onConnect" << std::endl;
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

/*
 * Called every frame
 */
void ControllerListener::onFrame(const Controller &controller) {
	// Get the most recent frame
	const Frame frame = controller.frame();

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); hl++) {
		// Get gestures
		const GestureList gestures = frame.gestures();
		for (int g = 0; g < gestures.count(); ++g) {
			Gesture gesture = gestures[g];
			switch (gesture.type()) {
				case Gesture::TYPE_SWIPE:{
					SwipeGesture swipe = gesture;
					float angleRight = swipe.direction().angleTo(Vector(1,0,0));
					float angleLeft = swipe.direction().angleTo(Vector(-1,0,0));
					float angleUp = swipe.direction().angleTo(Vector(0,1,0));
					float angleDown = swipe.direction().angleTo(Vector(0,-1,0));
					angleRight = toDegrees(angleRight);
					angleLeft = toDegrees(angleLeft);
					angleUp = toDegrees(angleUp);
					angleDown = toDegrees(angleDown);

					if(angleRight <= swipeAngle){
						//std::cout <<" direction Right: " << swipe.direction()[0] << std::endl;
						xdo_send_keysequence_window(xdo, CURRENTWINDOW, "ctrl+alt+Left", 0);
						break;
					}else if(angleLeft <= swipeAngle){
						//std::cout <<" direction left: " << swipe.direction()[0] << std::endl;
						xdo_send_keysequence_window(xdo, CURRENTWINDOW, "ctrl+alt+Right", 0);
						break;
					}

					if(angleUp <= swipeAngle){
						//std::cout <<" direction Up: " << swipe.direction()[1] << std::endl;
						xdo_send_keysequence_window(xdo, CURRENTWINDOW, "ctrl+alt+Down", 0);
						break;
					}else if(angleDown <= swipeAngle){
						//std::cout <<" direction down: " << swipe.direction()[1] << std::endl;
						xdo_send_keysequence_window(xdo, CURRENTWINDOW, "ctrl+alt+Up", 0);
						break;
					}
					break;
				}
				case Gesture::TYPE_SCREEN_TAP:{
					ScreenTapGesture screentap = gesture;

					float angle = screentap.direction().angleTo(Vector(0,0,1));
					angle = toDegrees(angle);
					std::cout << angle << std::endl;
					if (angle <= screenTapAngle) {
						xdo_send_keysequence_window(xdo, CURRENTWINDOW, "super+s", 0);
					}

					break;
				}
				default:
					std::cout << "Unknown gesture type." << std::endl;
					break;
			}
		}
	}
}

/*
 * Called when a Leap Motion controller plugged in, unplugged,
 * or the device changes state.
 */
void ControllerListener::onDeviceChange(const Controller &controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

float ControllerListener::toDegrees(float radians){
	return radians * 180/3.14159265359f;
}
